#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "nunchuk.h"

#define SYS_CLOCK 80000000

void EnableBuzzer(unsigned long ulBase, unsigned long ulTimer, unsigned long ulFreq);
void DisableBuzzer(unsigned long ulTimer, unsigned long ulFreq);
void PlayBugle(int buglePos, unsigned char isPressed);
void PlayBackingTrack(unsigned char reset);
void PlayMenuMusic(unsigned char reset);
void CalcScore(unsigned char dir, int buglePos, unsigned char reset);

#endif // __BUZZER_H__
