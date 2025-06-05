#include <stdio.h>
#include <stdint.h>

// Simplelink includes
#include "simplelink.h"

//Driverlib includes
#include "hw_types.h"
#include "pin.h"
#include "gpio.h"
#include "interrupt.h"
#include "hw_ints.h"
#include "hw_apps_rcm.h"
#include "hw_common_reg.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "spi.h"
#include "hw_memmap.h"
#include "timer.h"
#include "utils.h"
#include "uart.h"


//Common interface includes
#include "gpio_if.h"
#include "common.h"
#include "uart_if.h"
#include "timer_if.h"
#include "i2c_if.h"


// Custom includes
#include "network_utils.h"
#include "wifi.h"
#include "drawing.h"
#include "nunchuk.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1351.h"


//NEED TO UPDATE THIS FOR IT TO WORK!
#define DATE                4    /* Current Date */
#define MONTH               6     /* Month 1-12 */
#define YEAR                2025  /* Current year */
#define HOUR                12    /* Time - hours */
#define MINUTE              40    /* Time - minutes */
#define SECOND              0     /* Time - seconds */


#define APPLICATION_NAME      "SSL"
#define APPLICATION_VERSION   "SQ24"
#define SERVER_NAME           "arxlk3q779bud-ats.iot.us-east-2.amazonaws.com" // CHANGE ME
#define GOOGLE_DST_PORT       8443


#define POSTHEADER "POST /things/meow_cc3200/shadow HTTP/1.1\r\n"             // CHANGE ME
#define GETHEADER "GET /things/meow_cc3200/shadow HTTP/1.1\r\n"
#define HOSTHEADER "Host: arxlk3q779bud-ats.iot.us-east-2.amazonaws.com\r\n"  // CHANGE ME
#define CHEADER "Connection: Keep-Alive\r\n"
#define CTHEADER "Content-Type: application/json; charset=utf-8\r\n"
#define CLHEADER1 "Content-Length: "
#define CLHEADER2 "\r\n\r\n"

// DATA1 is the first part of the JSON to post, before the desired message
#define DATA1 "{" \
            "\"state\": {\r\n"                                              \
                "\"desired\" : {\r\n"                                       \
                    "\"leaderboard\" : [\""

// DATA2 is the second part of the JSON to post, after the desired message
#define DATA2 "]\"\r\n"                                            \
                "}"                                                         \
            "}"                                                             \
        "}\r\n\r\n"


#define SSID_LEN_MAX 32
#define BSSID_LEN_MAX 6
#define RECV_BUF_SIZE 1460

int set_time();
void GetPlayerName(char* playerName);
int Connect();
void UpdateLeaderboard(int iTLSSockID, unsigned int g_Score, char* newLeaderboardStr);
int GetLeaderboard(int iTLSSockID, char *acRecvbuff);
int http_post(int iTLSSockID, char* msg);
int http_get(int iTLSSockID, char* buf);