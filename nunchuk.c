#include <nunchuk.h>
#include <stdint.h>

// Driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "interrupt.h"
#include "prcm.h"
#include "gpio.h"
#include "timer.h"
#include "utils.h"

// Common interface includes
#include "uart_if.h"
#include "i2c_if.h"
#include "timer_if.h"

volatile uint8_t g_NunchukReadReady = 0;
#define DELAY_MS(MS) UtilsDelay(MS*80000.0)
#define MAX_READ_ATTEMPTS 3
#define MAX_INIT_ATTEMPTS 2

int NunchukHandshake() {
    // Reset Nunchuk VCC
    GPIOPinWrite(GPIOA0_BASE, 0x10, 0x00); // VCC Low
    DELAY_MS(50);
    GPIOPinWrite(GPIOA0_BASE, 0x10, 0x10); // VCC High

    // write 0x55 to nunchuk register 0xF0
    int ret = I2C_IF_Write(NUNCHUK_ADDR, (unsigned char[]){0xF0, 0x55}, 0x02, 0x01);
    if(ret < 0) return -1;

    // wait 3 ms
    DELAY_MS(3);

    // write 0x00 to nunchuk register 0xFB
    ret = I2C_IF_Write(NUNCHUK_ADDR, (unsigned char[]){0xFB, 0x00}, 0x02, 0x01);
    return ret;
}

int NunchukGetData(unsigned char *buf) {
    // write 0x00 to nunchuk to request data
    int ret = I2C_IF_Write(NUNCHUK_ADDR, (unsigned char[]){0x00}, 0x01, 0x01);
    if(ret < 0) return -1;

    // wait 3 ms
    DELAY_MS(3);

    // read 6 bytes of data
    ret = I2C_IF_Read(NUNCHUK_ADDR, buf, 6);
    return ret;
}

int NunchukRead(NunchukData *nd) {
    int failCount, ret;

    // Attempt to read nunchuk
    for (failCount = 0; failCount < MAX_READ_ATTEMPTS; failCount++) {
        ret = NunchukGetData((unsigned char *) nd);
        if (ret == 0) return 0; // Successful read
        else Report("Failed nunchuk read...\n\r"); // Failed read
    }

    // Too many consecutive read fails
    // Reinitialize
    Report("Attempting to initialize nunchuk...\n\r");
    for (failCount = 0; failCount < MAX_INIT_ATTEMPTS; failCount++) {
        ret = NunchukHandshake();
        if (ret == 0) {
            Report("Successful nunchuk init\n\r");
            break;
        } else {
            Report("Failed nunchuk init...\n\r");
        }
    }
    if (ret < 0) return -1; // return if init failed

    // Reattempt to read nunchuk
    for (failCount = 0; failCount < MAX_READ_ATTEMPTS; failCount++) {
        ret = NunchukGetData((unsigned char *) nd);
        if (ret == 0) return 0; // Successful read
        else Report("Failed nunchuk read...\n\r"); // Failed read
    }

    return -1; // Failed read
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
