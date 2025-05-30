//*****************************************************************************
//
// Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/ 
// 
// 
//  Redistribution and use in source and binary forms, with or without 
//  modification, are permitted provided that the following conditions 
//  are met:
//
//    Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the 
//    documentation and/or other materials provided with the   
//    distribution.
//
//    Neither the name of Texas Instruments Incorporated nor the names of
//    its contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************

//*****************************************************************************
//
// Application Name     - I2C
// Application Overview - The objective of this application is act as an I2C
//                        diagnostic tool. The demo application is a generic
//                        implementation that allows the user to communicate
//                        with any I2C device over the lines.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup i2c_demo
//! @{
//
//*****************************************************************************

// Standard includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// Driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "rom.h"
#include "rom_map.h"
#include "adc.h"
#include "gpio.h"
#include "spi.h"
#include "interrupt.h"
#include "prcm.h"
#include "utils.h"
#include "uart.h"
#include "timer.h"

// Common interface includes
#include "uart_if.h"
#include "i2c_if.h"
#include "timer_if.h"
#include "pinmux.h"
#include "gpio_if.h"

// OLED includes
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1351.h"
#include "glcdfont.h"
#include "oled_test.h"

// game header files
#include "notes.h"
#include "nunchuk.h"
#include "sprites.h"

//*****************************************************************************
//                      MACRO DEFINITIONS
//*****************************************************************************
#define APPLICATION_VERSION     "1.0.0"
#define APP_NAME                "Bugle Champion"

#define SYS_CLOCK                80000000
#define SPI_IF_BIT_RATE          4000000
#define NUNCHUK_ADDR             0x52
#define BLOW_EXPIRY              6
#define BLOW_THRESHOLD           0xB00
#define CLAMP(val, min, max) (val < min) ? (min) : (val > max) ? (max) : (val)

#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif

extern int g_startTimeMS;

//****************************************************************************
//                      LOCAL FUNCTION DEFINITIONS
//****************************************************************************

// Reset the g_NunchukReadReady flag
void
ReadTimerHandler()
{
    Timer_IF_InterruptClear(TIMERA0_BASE);
    g_NunchukReadReady = 1;
}

void OLED_Config()
{
    MAP_PRCMPeripheralClkEnable(PRCM_GSPI,PRCM_RUN_MODE_CLK);
    MAP_PRCMPeripheralReset(PRCM_GSPI);
    MAP_SPIReset(GSPI_BASE);
    MAP_SPIConfigSetExpClk(GSPI_BASE,MAP_PRCMPeripheralClockGet(PRCM_GSPI),
                     SPI_IF_BIT_RATE,SPI_MODE_MASTER,SPI_SUB_MODE_0,
                     (SPI_SW_CTRL_CS |
                     SPI_4PIN_MODE |
                     SPI_TURBO_OFF |
                     SPI_CS_ACTIVELOW |
                     SPI_WL_8));
    MAP_GPIOPinWrite(GPIOA1_BASE, 0x4, 0x04); // CS HIGH
    MAP_SPIEnable(GSPI_BASE);
    MAP_SPICSEnable(GSPI_BASE);
    MAP_GPIOPinWrite(GPIOA1_BASE, 0x4, 0x00); // CS LOW
}

//*****************************************************************************
//
//! Board Initialization & Configuration
//!
//! \param  None
//!
//! \return None
//
//*****************************************************************************
static void
BoardInit(void)
{
/* In case of TI-RTOS vector table is initialize by OS itself */
#ifndef USE_TIRTOS
    //
    // Set vector table base
    //
#if defined(ccs)
    MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif
    //
    // Enable Processor
    //
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);

    PRCMCC3200MCUInit();
}

void EnableBuzzer(long freq)
{
    long loadVal= SYS_CLOCK / freq;
    long matchVal = loadVal / 2;
    TimerLoadSet(TIMERA2_BASE, TIMER_B, loadVal & 0xFFFF);
    TimerPrescaleSet(TIMERA2_BASE, TIMER_B, (loadVal >> 16) & 0xFF);
    TimerMatchSet(TIMERA2_BASE, TIMER_B, matchVal & 0xFFFF);
    TimerPrescaleMatchSet(TIMERA2_BASE, TIMER_B, (matchVal >> 16) & 0xFF);
    TimerEnable(TIMERA2_BASE,TIMER_B);
}

void DisableBuzzer()
{
    TimerDisable(TIMERA2_BASE,TIMER_B);
}

int CalcBuglePosition(NunchukData nd) {
    static int prevPos = MIKU_WIDTH;
    int newPos;

    uint16_t accelZ = GetAccelZ(nd) - 10;
    accelZ = CLAMP(accelZ, 460, 820);

    if (accelZ > 660) {
        newPos = prevPos + ((accelZ - 660) >> 3);
    } else if (accelZ < 620) {
        newPos = prevPos - ((620 - accelZ) >> 3);
    } else {
        newPos = prevPos;
    }

    newPos = CLAMP(newPos, miku.width, OLED_WIDTH - bugle.width);
    prevPos = newPos;
    return newPos;
}

void InitConfig()
{
    // board configurations
    BoardInit();
    PinMuxConfig();

    // OLED
    OLED_Config();
    Adafruit_Init();

    // UART0 Terminal
    InitTerm();
    ClearTerm();

    // I2C
    I2C_IF_Open(I2C_MASTER_MODE_STD);

    // I2C Timer
    Timer_IF_Init(PRCM_TIMERA0, TIMERA0_BASE, TIMER_CFG_ONE_SHOT, TIMER_A, 0);
    Timer_IF_IntSetup(TIMERA0_BASE, TIMER_A, ReadTimerHandler);

    // PWM Timer
    PRCMPeripheralClkEnable(PRCM_TIMERA2, PRCM_RUN_MODE_CLK);
    PRCMPeripheralReset(PRCM_TIMERA2);
    TimerConfigure(TIMERA2_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM));

    // ADC
    ADCTimerConfig(ADC_BASE, 2);
    ADCTimerEnable(ADC_BASE);
    ADCEnable(ADC_BASE);
    ADCChannelEnable(ADC_BASE, ADC_CH_1);

    // GPIO
    GPIOPinWrite(GPIOA0_BASE, 0x10, 0x10); // Nunchuk VCC On
}

void main()
{
    InitConfig();

    NunchukData nd;
    NunchukRead(&nd);

    // clear and draw miku
    fillScreen(BG_COLOR);
    DrawSprite((const Sprite*) &miku, 0, OLED_HEIGHT - miku.height, BG_COLOR);
    g_startTimeMS = (PRCMSlowClkCtrGet() * 1000) / 32768;

    while(true)
    {
        NunchukRead(&nd);
//        double frequencies[] = {
//            261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.0, 415.3, 440.0, 466.16, 493.88, // OCTAVE 4
//            523.25, 554.37, 587.33, 622.25, 659.25, 698.46, 739.99, 783.99, 830.61, 880.0, 932.33, 987.77 // OCTAVE 5
//        };

        // read ADC
        static int blow_age;
        while(!MAP_ADCFIFOLvlGet(ADC_BASE, ADC_CH_1));
        unsigned int volume = (ADCFIFORead(ADC_BASE, ADC_CH_1) >> 2) & 0xFFF;

        if (volume >= BLOW_THRESHOLD) {
            blow_age = 0;
        } else if (blow_age < BLOW_EXPIRY) {
            blow_age++;
        }

        // draw bugle
        int bugle_pos = CalcBuglePosition(nd);
        DrawBugle(bugle_pos);

        // play/stop buzzer
        long frequency = (float) bugle_pos * (750.0 / 128.0) + 250;
        if (!nd.button_z || blow_age < BLOW_EXPIRY) {
            EnableBuzzer(frequency);
        } else {
            DisableBuzzer();
        }

        DrawNotes();
    }
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @
//
//*****************************************************************************


