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
#include "nunchuk.h"
#include "notes.h"
#include "drawing.h"

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

#define RED_LED                  0x2
#define GREEN_LED                0x80
#define BLUE_LED                 0x40

#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif

extern int g_startTimeMS;
unsigned int g_Score = 0;

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

void EnableBuzzer(unsigned long ulBase, unsigned long ulTimer, unsigned long ulFreq)
{
    unsigned long loadVal= SYS_CLOCK / ulFreq;
    unsigned long matchVal = loadVal / 2;
    TimerLoadSet(ulBase, ulTimer, loadVal & 0xFFFF);
    TimerPrescaleSet(ulBase, ulTimer, (loadVal >> 16) & 0xFF);
    TimerMatchSet(ulBase, ulTimer, matchVal & 0xFFFF);
    TimerPrescaleMatchSet(ulBase, ulTimer, (matchVal >> 16) & 0xFF);
    TimerEnable(ulBase, ulTimer);
}

void DisableBuzzer(unsigned long ulTimer, unsigned long ulFreq)
{
    TimerDisable(ulTimer, ulFreq);
}

int CalcBuglePosition(NunchukData nd) {
    static int prevPos = MIKU_WIDTH;
    int newPos;

    uint16_t accelZ = GetAccelZ(nd) - 50;
    accelZ = CLAMP(accelZ, 400, 700);
    newPos = (700 - accelZ) / 2; // 700 is tuned parameter for player comfort

    newPos = (newPos + prevPos * 3) / 4;
    newPos = CLAMP(newPos, 0, OLED_WIDTH - miku.width - bugle.width);
    prevPos = newPos;
    return newPos;
}

/**
 * Set one LED and turn others off
 * param led: macro RED_LED, GREEN_LED, or BLUE_LED
 */
void SetLED(unsigned char led) {
    // turn all off
    GPIOPinWrite(GPIOA3_BASE, 0x2, 0); // RED
    GPIOPinWrite(GPIOA3_BASE, 0x80, 0); // GREEN
    GPIOPinWrite(GPIOA3_BASE, 0x40, 0); // BLUE

    // turn on
    GPIOPinWrite(GPIOA3_BASE, led, led);
}

/**
 * Increment points if points are earned and set the LEDs
 * 10 points given if player starts or ends on time
 * 3 points if starts or ends sort of on time
 * param dir: 0 if button pressed/mic blown and 1 if released
 * param buglePos: BUGLE!!!!! (bugle position, left side. player should align edge with note)
 *
 *
 * Note: if notes are shorter than 150ms there may be problems
 */
void CalcScore(unsigned char dir, int buglePos) {
    static int currentNote = 0; // index for demo_song
    static int completedNote = -1;
    static uint8_t noteScore = 0; // reset for every note
    long curr_time_ms = (PRCMSlowClkCtrGet() * 1000) / 32768 - g_startTimeMS;
    Note note = demo_song[currentNote];

    int compareVal; // this is either the note start time or end time

    if (dir == 0) {                         // start note
        // get the currently applicable note
        while (curr_time_ms > note.start_ms + note.length_ms || curr_time_ms > note.start_ms + 150) {
            currentNote++;
            note = demo_song[currentNote];
        }
        compareVal = note.start_ms;
    } else {                                // end note

        compareVal = note.start_ms + note.length_ms;
    }

    // score
    if (currentNote > completedNote) {
        if (buglePos > note.x - 3 && buglePos < note.x + 5) {
            long diff = abs(curr_time_ms - compareVal);
            if (diff < 75) {         // great
                g_Score += 10;
                noteScore += 10;
                SetLED(GREEN_LED);
            } else if (diff < 150) { // ok
                g_Score += 3;
                noteScore += 3;
                SetLED(BLUE_LED);
            } else {                 // bad
                SetLED(RED_LED);
            }
        }
    }

    if (dir == 1 && noteScore > 0) {
        completedNote = currentNote;
        noteScore = 0;
    }
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

    // PWM Timer (Main)
    PRCMPeripheralClkEnable(PRCM_TIMERA2, PRCM_RUN_MODE_CLK);
    PRCMPeripheralReset(PRCM_TIMERA2);
    TimerConfigure(TIMERA2_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM));

    // PWM Timer (Backing)
    PRCMPeripheralClkEnable(PRCM_TIMERA3, PRCM_RUN_MODE_CLK);
    PRCMPeripheralReset(PRCM_TIMERA3);
    TimerConfigure(TIMERA3_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM));

    // ADC
    ADCTimerConfig(ADC_BASE, 2);
    ADCTimerEnable(ADC_BASE);
    ADCEnable(ADC_BASE);
    ADCChannelEnable(ADC_BASE, ADC_CH_1);

    // GPIO LED (turn off)
    GPIOPinWrite(GPIOA3_BASE, 0x02, 0); // RED
    GPIOPinWrite(GPIOA3_BASE, 0x80, 0); // GREEN
    GPIOPinWrite(GPIOA3_BASE, 0x40, 0); // BLUE
}

void PlayBackingTrack()
{
    static int noteIdx;
    long curr_time_ms = (PRCMSlowClkCtrGet() * 1000) / 32768 - g_startTimeMS;
    int totalNotes = sizeof(demo_song_back)/sizeof(Note);

    while (noteIdx < totalNotes) {
        Note note = demo_song_back[noteIdx];
        int note_end_ms = note.start_ms + note.length_ms;
        if (curr_time_ms > note_end_ms) {
            // finished note
            noteIdx++;
        } else if (curr_time_ms >= note.start_ms) {
            // play note
            EnableBuzzer(TIMERA2_BASE, TIMER_B, note.x);
            Report("%d\n\r", note.x);
            return;
        } else {
            // no note to play
            break;
        }
    }
    DisableBuzzer(TIMERA2_BASE, TIMER_B);
}

void main()
{
    InitConfig();

    NunchukData nd;
    NunchukHandshake();

    // clear scren and draw miku
    fillScreen(BG_COLOR);
    DrawSprite((const Sprite*) &miku, OLED_WIDTH - miku.width, OLED_HEIGHT - miku.height, BG_COLOR);

    // give 4s for song to start
    g_startTimeMS = (PRCMSlowClkCtrGet() * 1000) / 32768 + 4000;

    // keep track of if button is pressed or not
    unsigned char isPressed = 0;

    while(true)
    {
        NunchukRead(&nd);

        // read ADC
        static int blow_age;
//        while(!MAP_ADCFIFOLvlGet(ADC_BASE, ADC_CH_1));
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
        long frequency = POS_TO_FREQ(bugle_pos);
        if (!nd.button_z || blow_age < BLOW_EXPIRY) {
            EnableBuzzer(TIMERA3_BASE,TIMER_B, frequency);
            if (!isPressed) {
                isPressed = 1;
                CalcScore(0, bugle_pos);
            }
        } else {
            DisableBuzzer(TIMERA3_BASE,TIMER_B);
            if (isPressed) {
                isPressed = 0;
                CalcScore(1, bugle_pos);
            }
        }
        PlayBackingTrack();
        DrawScore(g_Score);
        DrawNotes();
    }
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @
//
//*****************************************************************************


