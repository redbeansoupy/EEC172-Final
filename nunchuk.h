// nunchuk.h
#ifndef __NUNCHUK_H__
#define __NUNCHUK_H__

#include <stdint.h>

#define NUNCHUK_ADDR 0x52

extern volatile uint8_t g_NunchukReadReady;

typedef struct {
    uint8_t joystick_x;
    uint8_t joystick_y;
    uint8_t accel_x_high;
    uint8_t accel_y_high;
    uint8_t accel_z_high;
    uint8_t button_z   : 1;
    uint8_t button_c   : 1;
    uint8_t accel_x_low : 2;
    uint8_t accel_y_low : 2;
    uint8_t accel_z_low : 2;
} NunchukData;

int NunchukHandshake(void);
int NunchukGetData(unsigned char *buf);
int CalculatePosition(NunchukData nd, int currentPos);
int NunchukRead(NunchukData *nd);

uint16_t GetAccelX(const NunchukData nd);
uint16_t GetAccelY(const NunchukData nd);
uint16_t GetAccelZ(const NunchukData nd);

#endif // NUNCHUK_H
