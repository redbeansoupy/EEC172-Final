#include <nunchuk.h>
#include <stdint.h>

// Driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "interrupt.h"
#include "prcm.h"
#include "timer.h"

// Common interface includes
#include "i2c_if.h"
#include "timer_if.h"

volatile uint8_t g_NunchukReadReady = 0;

int NunchukHandshake() {
    // write 0x55 to nunchuk register 0xF0
    int ret = I2C_IF_Write(NUNCHUK_ADDR, (unsigned char[]){0xF0, 0x55}, 0x02, 0x01);
    if(ret < 0) return -1;

    // wait 20 ms
    g_NunchukReadReady = 0;
    Timer_IF_Start(TIMERA0_BASE, TIMER_A, 20);
    while (!g_NunchukReadReady){}

    // write 0x00 to nunchuk register 0xFB
    ret = I2C_IF_Write(NUNCHUK_ADDR, (unsigned char[]){0xFB, 0x00}, 0x02, 0x01);
    return ret;
}

int NunchukGetData(unsigned char *buf) {
    // write 0x00 to nunchuk to request data
    int ret = I2C_IF_Write(NUNCHUK_ADDR, (unsigned char[]){0x00}, 0x01, 0x01);
    if(ret < 0) return -1;

    // wait 20 ms
    g_NunchukReadReady = 0;
    Timer_IF_Start(TIMERA0_BASE, TIMER_A, 20);
    while (!g_NunchukReadReady){}

    // read 6 bytes of data
    ret = I2C_IF_Read(NUNCHUK_ADDR, buf, 6);
    return ret;
}

uint16_t GetAccelX(const NunchukData nd) {
    uint16_t accelX = ((uint16_t) nd.accel_x_high << 2) | nd.accel_x_low;
    return accelX;
}

uint16_t GetAccelY(const NunchukData nd) {
    uint16_t accelY = ((uint16_t) nd.accel_y_high << 2) | nd.accel_y_low;
    return accelY;
}

uint16_t GetAccelZ(const NunchukData nd) {
    uint16_t accelZ = ((uint16_t) nd.accel_z_high << 2) | nd.accel_z_low;
    return accelZ;
}
