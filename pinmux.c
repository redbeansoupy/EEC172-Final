//*****************************************************************************
// pin_mux_config.c
//
// configure the device pins for different signals
//
// Copyright (c) 2016, Texas Instruments Incorporated - http://www.ti.com/ 
// All rights reserved.
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

// This file was automatically generated on 4/5/2025 at 8:08:20 PM
// by TI PinMux version 1.18.1+3343
//
//*****************************************************************************

#include <pinmux.h>
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_gpio.h"
#include "pin.h"
#include "gpio.h"
#include "prcm.h"

//*****************************************************************************
void PinMuxConfig(void)
{


    //
    // Set unused pins to PIN_MODE_0 with the exception of JTAG pins 16,17,19,20
    //
    PinModeSet(PIN_15, PIN_MODE_0);
    PinModeSet(PIN_18, PIN_MODE_0);
    PinModeSet(PIN_50, PIN_MODE_0);
    PinModeSet(PIN_52, PIN_MODE_0);
    PinModeSet(PIN_55, PIN_MODE_0);
    PinModeSet(PIN_57, PIN_MODE_0);
    PinModeSet(PIN_60, PIN_MODE_0);
    
    //
    // Enable Peripheral Clocks 
    //
    PRCMPeripheralClkEnable(PRCM_GPIOA0, PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_GPIOA1, PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_GPIOA3, PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_GSPI, PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_I2CA0, PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_UARTA0, PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_ADC, PRCM_RUN_MODE_CLK);

    PinTypeUART(PIN_55, PIN_MODE_3); // PIN_55 - UART0 UART0_TX
    PinTypeUART(PIN_57, PIN_MODE_3); // PIN_57 - UART0 UART0_RX

    PinTypeI2C(PIN_01, PIN_MODE_1); // PIN_01 - I2C0 I2C_SCL
    PinTypeI2C(PIN_17, PIN_MODE_9); // PIN_17 - I2C0 I2C_SDA
    PinTypeGPIO(PIN_59, PIN_MODE_0, false); // PIN_59 - GPIO NUNCHUK VCC
    GPIODirModeSet(GPIOA0_BASE, 0x10, GPIO_DIR_MODE_OUT);
    PinConfigSet(PIN_59, PIN_STRENGTH_6MA, PIN_TYPE_STD);

    PinTypeGPIO(PIN_03, PIN_MODE_0, false); // PIN_03 - GPIO OLED RESET
    GPIODirModeSet(GPIOA1_BASE, 0x10, GPIO_DIR_MODE_OUT);
    PinTypeSPI(PIN_05, PIN_MODE_7); // PIN_05 - SPI0 GSPI_CLK
    PinTypeSPI(PIN_06, PIN_MODE_7); // PIN_06 - SPI0 GSPI_MISO
    PinTypeSPI(PIN_07, PIN_MODE_7); // PIN_07 - SPI0 GSPI_MOSI
    PinTypeSPI(PIN_08, PIN_MODE_7); // PIN_08 - SPI0 GSPI_CS
    PinTypeADC(PIN_58, PIN_MODE_255); // PIN_08 - ADC_CH1
    PinTypeGPIO(PIN_61, PIN_MODE_0, false); // PIN_61 - GPIO OLED CS
    GPIODirModeSet(GPIOA0_BASE, 0x40, GPIO_DIR_MODE_OUT);
    PinTypeGPIO(PIN_62, PIN_MODE_0, false); // PIN_62 - GPIO OLED D/C#
    GPIODirModeSet(GPIOA0_BASE, 0x80, GPIO_DIR_MODE_OUT);
    PinTypeTimer(PIN_64, PIN_MODE_3); // PIN_64 - MAIN BUZZER TIMERPWM5 GT_PWM05
    PinTypeTimer(PIN_02, PIN_MODE_3); // PIN_02 - BACKING BUZZER TIMERPWM7 GT_PWM07

//    PinTypeGPIO(PIN_21, PIN_MODE_0, false); // PIN_21 - RED LED
//    GPIODirModeSet(GPIOA3_BASE, 0x2, GPIO_DIR_MODE_OUT);
    PinTypeGPIO(PIN_45, PIN_MODE_0, false); // PIN_45 - GREEN LED
    GPIODirModeSet(GPIOA3_BASE, 0x80, GPIO_DIR_MODE_OUT);
    PinTypeGPIO(PIN_53, PIN_MODE_0, false); // PIN_53 - BLUE LED
    GPIODirModeSet(GPIOA3_BASE, 0x40, GPIO_DIR_MODE_OUT);

}
