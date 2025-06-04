#include "buzzer.h"
#include "notes.h"
#include "nunchuk.h"
#include "drawing.h"

#include "hw_memmap.h"
#include "gpio.h"
#include "hw_types.h"
#include "timer.h"
#include "prcm.h"

#define RED_LED                  0x2
#define GREEN_LED                0x80
#define BLUE_LED                 0x40

extern int g_startTimeMS;
unsigned int g_Score = 0;

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

void PlayBugle(int buglePos, unsigned char isPressed)
{
    long frequency = POS_TO_FREQ(buglePos);
    if (isPressed) {
        EnableBuzzer(TIMERA3_BASE,TIMER_B, frequency);
    } else {
        DisableBuzzer(TIMERA3_BASE,TIMER_B);
    }
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
            return;
        } else {
            // no note to play
            break;
        }
    }
    DisableBuzzer(TIMERA2_BASE, TIMER_B);
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

    if (dir == 0) {
        // start note
        // get the currently applicable note
        while (curr_time_ms > note.start_ms + note.length_ms || curr_time_ms > note.start_ms + 150) {
            currentNote++;
            note = demo_song[currentNote];
        }
        compareVal = note.start_ms;
    } else {
        // end note
        compareVal = note.start_ms + note.length_ms;
    }

    // score
    if (currentNote > completedNote) {
        if (buglePos > note.x - 3 && buglePos < note.x + 5) {
            long diff = abs(curr_time_ms - compareVal);

            // turn all LEDs off
            GPIOPinWrite(GPIOA3_BASE, 0x2, 0); // RED
            GPIOPinWrite(GPIOA3_BASE, 0x80, 0); // GREEN
            GPIOPinWrite(GPIOA3_BASE, 0x40, 0); // BLUE
            if (diff < 75) {         // great
                g_Score += 10;
                noteScore += 10;
                GPIOPinWrite(GPIOA3_BASE, GREEN_LED, GREEN_LED);
            } else if (diff < 150) { // ok
                g_Score += 3;
                noteScore += 3;
                GPIOPinWrite(GPIOA3_BASE, BLUE_LED, BLUE_LED);
            } else {                 // bad
            }
        }
    }

    if (dir == 1 && noteScore > 0) {
        completedNote = currentNote;
        noteScore = 0;
    }
}
