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
#include <config.h>
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
#include "gpio_if.h"

// OLED includes
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1351.h"
#include "glcdfont.h"

// game header files
#include "nunchuk.h"
#include "buzzer.h"
#include "notes.h"
#include "drawing.h"

//*****************************************************************************
//                      MACRO DEFINITIONS
//*****************************************************************************
#define APPLICATION_VERSION     "1.0.0"
#define APP_NAME                "Bugle Champion"

#define BLOW_EXPIRY              6
#define BLOW_THRESHOLD           0xB00
#define CLAMP(val, min, max) (val < min) ? (min) : (val > max) ? (max) : (val)


extern unsigned int g_startTimeMS;
extern unsigned int g_Score;

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

void main()
{
    InitConfig();

    NunchukData nd;
    NunchukHandshake();

    // game loop (title -> game -> score)
    while (true) {
    // ============== TITLE SCREEN ===============================

    DrawSprite((const Sprite*) &title, 0, 0, 0x0000);

    while(true) {
        NunchukRead(&nd);
        if (nd.button_z == 0) {
            break;
        }
    }

    // ============== TODO: SONG SELECT ==========================




    // ============== GAME !!!! ==================================

    // clear screen and draw miku
    fillScreen(BG_COLOR);
    DrawSprite((const Sprite*) &miku, OLED_WIDTH - miku.width, OLED_HEIGHT - miku.height, BG_COLOR);

    // give 4s for song to start
    g_startTimeMS = (PRCMSlowClkCtrGet() * 1000) / 32768 + 4000;

    // store end time
    Note lastNote = demo_song_back[sizeof(demo_song_back) / sizeof(Note) - 1];
    unsigned long end = g_startTimeMS + lastNote.start_ms + lastNote.length_ms + 1000;

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
        int buglePos = CalcBuglePosition(nd);
        DrawBugle(buglePos);

        //
        // static keep track of if button is pressed or not
        static unsigned char wasPressed = 0;
        unsigned char isPressed = !nd.button_z || blow_age < BLOW_EXPIRY;
        if (isPressed && !wasPressed) {
            wasPressed  = 1;
            CalcScore(0, buglePos); // posedge
        } else if (!isPressed && wasPressed) {
            wasPressed = 0;
            CalcScore(1, buglePos); // negedge
        }

        // play/stop buzzer
        PlayBugle(buglePos, isPressed);
        PlayBackingTrack();
        DrawScore(g_Score);
        DrawNotes();

        // check if last note
        unsigned long now = (PRCMSlowClkCtrGet() * 1000) / 32768;
        if (now > end) {
            break;
        }

    }



    // ============== TODO: LEADERBOARD =============================
}
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @
//
//*****************************************************************************


