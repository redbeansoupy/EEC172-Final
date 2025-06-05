#include "leaderboard.h"

// OLED includes
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1351.h"
#include "glcdfont.h"

int set_time() {
    long retVal;

    g_time.tm_day = DATE;
    g_time.tm_mon = MONTH;
    g_time.tm_year = YEAR;
    g_time.tm_sec = HOUR;
    g_time.tm_hour = MINUTE;
    g_time.tm_min = SECOND;

    retVal = sl_DevSet(SL_DEVICE_GENERAL_CONFIGURATION,
                          SL_DEVICE_GENERAL_CONFIGURATION_DATE_TIME,
                          sizeof(SlDateTime),(unsigned char *)(&g_time));

    ASSERT_ON_ERROR(retVal);
    return SUCCESS;
}

int Connect() {
    // initialize global default app configuration
    g_app_config.host = SERVER_NAME;
    g_app_config.port = GOOGLE_DST_PORT;

    //Connect the CC3200 to the local access point
    int lRetVal = connectToAccessPoint();
    //Set time so that encryption can be used
    lRetVal = set_time();
    if(lRetVal < 0) {
        UART_PRINT("Unable to set time in the device");
        LOOP_FOREVER();
    }
    //Connect to the website with TLS encryption
    lRetVal = tls_connect();
    if(lRetVal < 0) {
        ERR_PRINT(lRetVal);
    }

    return lRetVal;
}

int GetLeaderboard(int iTLSSockID, char *acRecvbuff) {
    acRecvbuff = http_get(iTLSSockID, acRecvbuff);
    
    // find the reported leaderboard
    const char matchReported[8] = "reported";
    const char matchLeaderboard[11] = "leaderboard";
    uint8_t reported = 0;
    uint8_t leaderboard = 0;
    int idx = 0;

    while (reported < 8) {
        if (acRecvbuff[idx] == matchReported[reported]) {
            reported++;
        } else if (acRecvbuff[idx] == '\0') {
            return -1;
        } else {
            reported = 0;
        }
        idx++;
    }
    
    while (leaderboard < 11) {
        if (acRecvbuff[idx] == matchLeaderboard[leaderboard]) {
            leaderboard++;
        } else if (acRecvbuff[idx] == '\0') {
            return -1;
        } else {
            leaderboard = 0;
        }
        idx++;
    }
    idx += 3; // go to start of string

    return idx;
}

void GetPlayerName(char* playerName) {
    NunchukData nd;
    NunchukRead(&nd);
}

void UpdateLeaderboard(int iTLSSockID, unsigned int g_Score) {
    char leaderboardStr[RECV_BUF_SIZE];
    char *lbPtr = &leaderboardStr[0];

    // get leaderboard
    int idx = GetLeaderboard(iTLSSockID, leaderboardStr);
    Report("index of leaderboard: %d\n\r", idx);
    if (idx < 0) {
        return;
    }

    // parse scores
    char names[5][6];
    char scores_char[5][5];
    unsigned int scores[5];
    uint8_t playerPlaced = 1; // 1 if player placed in top 5
    uint8_t i;
    for (i = 0; i < 5; i++) {
        uint8_t j;
        for (j = 0; j < 2; j++) {
            uint8_t k;
            for (k = 0; k < 5; k++) {
                if (*lbPtr == ' ') {
                    lbPtr++;
                    break;
                } else if (*lbPtr == '"') {
                    goto post;
                } else {
                    if (j == 0) {
                        names[i][k] = lbPtr[0];
                    } else {
                        scores_char[i][k] = lbPtr[0];
                    }
                    lbPtr++;
                }
            }
        }

        // parse score into int
        scores[i] = 0;
        for (j = 0; j < 4; j++) {
            scores[i] *= 10;
            scores[i] += scores_char[i][j] - '0';
        }
        if (g_Score > scores[i]) {
            playerPlaced = 1;
            scores[i + 1] = scores[i];
            scores[i] = g_Score;
            memcpy(names[i + 1], names[i], 5);

            // ask for player's name
            char playerName[6];
            GetPlayerName(playerName);
            memcpy(names[i], playerName, 5);

            i++;
        }
    
    }
    post:
}

int http_post(int iTLSSockID, char* msg){
    char acSendBuff[512];
    char acRecvbuff[1460];
    char cCLLength[200];
    char* pcBufHeaders;
    int lRetVal = 0;

    pcBufHeaders = acSendBuff;
    strcpy(pcBufHeaders, POSTHEADER);
    pcBufHeaders += strlen(POSTHEADER);
    strcpy(pcBufHeaders, HOSTHEADER);
    pcBufHeaders += strlen(HOSTHEADER);
    strcpy(pcBufHeaders, CHEADER);
    pcBufHeaders += strlen(CHEADER);
    strcpy(pcBufHeaders, "\r\n\r\n");

    char awsMsgCpy[1024];
    memcpy(awsMsgCpy, msg, 1024);
    int dataLength = strlen(DATA1) + strlen(awsMsgCpy) + strlen(DATA2);

    strcpy(pcBufHeaders, CTHEADER);
    pcBufHeaders += strlen(CTHEADER);
    strcpy(pcBufHeaders, CLHEADER1);

    pcBufHeaders += strlen(CLHEADER1);
    sprintf(cCLLength, "%d", dataLength);

    strcpy(pcBufHeaders, cCLLength);
    pcBufHeaders += strlen(cCLLength);
    strcpy(pcBufHeaders, CLHEADER2);
    pcBufHeaders += strlen(CLHEADER2);

    // json start
    strcpy(pcBufHeaders, DATA1);
    pcBufHeaders += strlen(DATA1);

    // message from IR remote
    strcpy(pcBufHeaders, awsMsgCpy);
    pcBufHeaders += strlen(awsMsgCpy);

    // end json
    strcpy(pcBufHeaders, DATA2);
    pcBufHeaders += strlen(DATA2);


    int testDataLength = strlen(pcBufHeaders);

    UART_PRINT(acSendBuff);

    // sent GET request
    lRetVal = sl_Send(iTLSSockID, acSendBuff, strlen(acSendBuff), 0);
    if(lRetVal < 0) {
        UART_PRINT("GET failed. Error Number: %i\n\r",lRetVal);
        sl_Close(iTLSSockID);
        GPIO_IF_LedOn(MCU_RED_LED_GPIO);
        return lRetVal;
    }

    // get response
    lRetVal = sl_Recv(iTLSSockID, &acRecvbuff[0], sizeof(acRecvbuff), 0);
    if(lRetVal < 0) {
        UART_PRINT("Received failed. Error Number: %i\n\r",lRetVal);
        //sl_Close(iSSLSockID);
        GPIO_IF_LedOn(MCU_RED_LED_GPIO);
           return lRetVal;
    }
    else {
        acRecvbuff[lRetVal+1] = '\0';
        UART_PRINT(acRecvbuff);
        UART_PRINT("\n\r\n\r");
    }

    return 0;
}

int http_get(int iTLSSockID, char* acRecvbuff){
    char acSendBuff[512];
    // char acRecvbuff[1460];
    char cCLLength[200];
    char* pcBufHeaders;
    int lRetVal = 0;

    pcBufHeaders = acSendBuff;
    strcpy(pcBufHeaders, GETHEADER);
    pcBufHeaders += strlen(GETHEADER);
    strcpy(pcBufHeaders, HOSTHEADER);
    pcBufHeaders += strlen(HOSTHEADER);
    strcpy(pcBufHeaders, CHEADER);
    pcBufHeaders += strlen(CHEADER);
    strcpy(pcBufHeaders, "\r\n\r\n");

    UART_PRINT(acSendBuff);


    //
    // Send the packet to the server */
    //
    lRetVal = sl_Send(iTLSSockID, acSendBuff, strlen(acSendBuff), 0);
    if(lRetVal < 0) {
        UART_PRINT("POST failed. Error Number: %i\n\r",lRetVal);
        sl_Close(iTLSSockID);
        GPIO_IF_LedOn(MCU_RED_LED_GPIO);
        return lRetVal;
    }
    UART_PRINT("successfully sent GET request\n\r");
    lRetVal = sl_Recv(iTLSSockID, &acRecvbuff[0], 1460UL, 0);
    if(lRetVal < 0) {
        UART_PRINT("Received failed. Error Number: %i\n\r",lRetVal);
        //sl_Close(iSSLSockID);
        GPIO_IF_LedOn(MCU_RED_LED_GPIO);
           return lRetVal;
    }
    else {
        acRecvbuff[lRetVal+1] = '\0';
        UART_PRINT("\n\r\n\r\n\r RECEIVED FROM GET: \n\r");
        // int i = 0;
        // for (i = 0; i < lRetVal; i++) {
        //     UART_PRINT("%c", acRecvbuff[i]);
        // }
        UART_PRINT(acRecvbuff);
        UART_PRINT("\n\r\n\r");

        return acRecvbuff;
    }
}


