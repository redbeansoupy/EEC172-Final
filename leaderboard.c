#include "leaderboard.h"

// OLED includes
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1351.h"
#include "glcdfont.h"

const char leaderboardStrings[3][19] = {"\"leaderboard0\" : \"",
        "\"leaderboard1\" : \"",
        "\"leaderboard2\" : \""
};


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

int GetLeaderboard(int iTLSSockID, char *acRecvbuff, unsigned int g_songIdx) {
    int ret = 1;
    while (ret != 0) {
        ret = http_get(iTLSSockID, acRecvbuff);
    }
    
    // find the desired leaderboard
    const char matchDesired[7] = "desired";
    char matchLeaderboard[12] = "leaderboard0";
    uint8_t desired = 0;
    uint8_t leaderboard = 0;
    int idx = 0;

    // change name of leaderboard based on song idx
    switch (g_songIdx) {
        case 1:
            matchLeaderboard[11] = '1';
            break;
        case 2:
            matchLeaderboard[11] = '2';
            break;
        default:
            break;
    }
    Report("matchLeaderboard: %s\n\r", matchLeaderboard);
    Report("matchDesired: %s\n\r", matchDesired);


    while (desired < 7) {
        if (acRecvbuff[idx] == matchDesired[desired]) {
            desired++;
        } else if (acRecvbuff[idx] == '\0') {
            return -1;
        } else {
            desired = 0;
        }
        idx++;
    }
    
    while (leaderboard < 12) {
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
    Report("getting player name\n\r");
    const char alphabet[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 
                            'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 
                            'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    NunchukData nd;
    

    uint8_t charIdx = 0;
    uint8_t alphaIdx = 0;
    static unsigned char wasLeft, wasRight, wasPressed;
    
    fillScreen(BG_COLOR);
    DrawMsg(15, 10, "what's your name?");
    DrawMsg(20, 90, "use joystick to");
    DrawMsg(25, 98, "select letter");
    DrawMsg(10, 110, "press z to confirm");

    for (charIdx = 0; charIdx < 3; charIdx++) {
        while (true) {
            NunchukRead(&nd);

            // joystick left
            if (nd.joystick_y < 40 && !wasLeft) {
                wasLeft = 1;
                if (alphaIdx-- == 0) alphaIdx = 25;
            } else if (nd.joystick_y >= 40 && wasLeft) {
                wasLeft = 0;
            }
    
            // joystick right
            if (nd.joystick_y > 207 && !wasRight) {
                wasRight = 1;
                if (++alphaIdx == 26) alphaIdx = 0;
            } else if (nd.joystick_y <= 207 && wasRight) {
                wasRight = 0;
            }

            drawChar(charIdx * 12 + 45, 50, alphabet[alphaIdx], 0xFFFFF, BG_COLOR, 2);
            if (nd.button_z == 0 && !wasPressed) {
                playerName[charIdx] = alphabet[alphaIdx];
                wasPressed = 1;
                break;
            } else if (nd.button_z == 1) {
                wasPressed = 0;
            }

        }
    }
    playerName[3] = '\0';
    Report("Got player name: %s\n\r", playerName);

}

void UpdateLeaderboard(int iTLSSockID, unsigned int g_Score, unsigned int g_songIdx, char* newLeaderboardStr) {
    char leaderboardStr[RECV_BUF_SIZE];
    Report("Using score %d\n\r", g_Score);
    
    // get leaderboard
    int idx = GetLeaderboard(iTLSSockID, leaderboardStr, g_songIdx);
    Report("index of leaderboard: %d\n\r", idx);
    if (idx < 0) {
        return;
    }
    char *lbPtr = &leaderboardStr[idx];
    
    // parse scores
    char names[5][5];
    char scores_char[5][5];
    unsigned int scores[5];
    int playerPlace = -1; // 0 for first place
    int i; // i needs to be signed
    int lbLen = 0;

    for (i = 0; i < 5; i++) {
        uint8_t j;
        for (j = 0; j < 2; j++) {
            uint8_t k;
            for (k = 0; k < 5; k++) {
                if (*lbPtr == ' ') {
                    lbPtr++;
                    if (j == 0) {
                        names[i][k] = '\0';
                        Report("Placed null on names[%d][%d]\n\r", i, k);
                    } else {
                        scores_char[i][k] = '\0';
                        Report("Placed null on scores_char[%d][%d]\n\r", i, k);
                        lbLen += 1;
                    }
                    break;
                } else if (*lbPtr == '"') {
                    Report("Detected end of str: i = %d, j = %d, k = %d\n\r", i, j, k);
                    if (j == 0) {
                        names[i][k] = '\0';
                        Report("Placed null on names[%d][%d]\n\r", i, k);

                    } else {
                        scores_char[i][k] = '\0';
                        Report("Placed null on scores_char[%d][%d]\n\r", i, k);
                        lbLen += 1;
                    }
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
            if (scores_char[i][j] == '\0') break;
            scores[i] *= 10;
            scores[i] += scores_char[i][j] - '0';
        }
        Report("scores[%d]: %u\n\r", i, scores[i]);
        if (g_Score > scores[i] && playerPlace < 0) {
            playerPlace = i;
            Report("Player placed at %d\n\r", i);
        }
    }
    
    post:
    // move data if player placed
    if (lbLen < 5 && playerPlace < 0) {
        playerPlace = i;
        Report("Player placed at %d\n\r", i); 
        lbLen += 1;
    }
    if (playerPlace >= 0) {
        for (i = lbLen - 2; i >= playerPlace; i--) {
            Report("moved score, lbLen = %u, i = %u\n\r", lbLen, i);
            scores[i + 1] = scores[i];
            memcpy(scores_char[i + 1], scores_char[i], 5);
            memcpy(names[i + 1], names[i], 5);
        }
        scores[playerPlace] = g_Score;

        // ask for player's name
        char playerName[6];
        GetPlayerName(playerName);
        memcpy(names[playerPlace], playerName, 4);

        char digits[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
        unsigned char hundred = g_Score / 100;
        unsigned char ten = (g_Score % 100) / 10;
        unsigned char one = g_Score % 10;
        scores_char[playerPlace][0] = hundred + '0';
        scores_char[playerPlace][1] = ten + '0';
        scores_char[playerPlace][2] = one + '0';
        scores_char[playerPlace][3] = '\0';
    }

    // create the new leaderboardStr
    if (playerPlace < 0) {
        memcpy(newLeaderboardStr, &leaderboardStr[idx], 1460);
    } else {
        // generate the string for drawing
        idx = 0;
        for (i = 0; i < lbLen; i++) {
            uint8_t j;
            for (j = 0; j < 5; j++) {
                if (names[i][j] == '\0') break;
                newLeaderboardStr[idx] = names[i][j];
                idx++;
            }
            newLeaderboardStr[idx] = ' ';
            idx++;
            for (j = 0; j < 5; j++) {
                if (scores_char[i][j] == '\0') break;
                newLeaderboardStr[idx] = scores_char[i][j];
                idx++;
            }
            newLeaderboardStr[idx] = ' ';
            idx++;
        }
    }
    newLeaderboardStr[idx] = '"'; // for how the draw function parses the string
    newLeaderboardStr[idx + 1] = '\0';

    int ret = 1;
    while (ret != 0) {
        ret = http_post(iTLSSockID, newLeaderboardStr, g_songIdx);
    }        
}

int http_post(int iTLSSockID, char* msg, unsigned int g_songIdx){
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
    strcpy(awsMsgCpy, msg);
    int dataLength = strlen(DATA1) + strlen(leaderboardStrings[g_songIdx]) + strlen(awsMsgCpy) + strlen(DATA2);

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
    strcpy(pcBufHeaders, leaderboardStrings[g_songIdx]);
    pcBufHeaders += strlen(leaderboardStrings[g_songIdx]);

    // leaderboard contents
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
        UART_PRINT(acRecvbuff);
        UART_PRINT("\n\r\n\r");

        return 0;
    }
}


