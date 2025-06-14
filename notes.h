/* SONGS: 
 *
 * Title Screen - Miracle Paint by OSTER project 
 * Based on arrangement by user Musesc0rer on musescore
 * Arrangement: https://musescore.com/user/941151/scores/1475131
 * User Profile: https://musescore.com/user/941151
 * 
 * Track 1: Toreador March by Georges Bizet
 * Based on arrangement by user NECROMANCER on onlinesequencer.net
 * Arrangement: https://onlinesequencer.net/1347793
 * User Profile: https://onlinesequencer.net/members/36567
 * 
 * Track 2: Ditto by NewJeans
 * Based on arrangement by user blueberrieszn IS BACK on musescore 
 * Arrangement: https://musescore.com/user/36964839/scores/9351625
 * User Profile: https://musescore.com/user/36964839
 * 
 * Track 3: World is Mine by ryo (supercell) 
 * Based on arrangement by user legend57961 on musescore
 * Arrangement: https://musescore.com/user/235731/scores/237176
 * User Profile: https://musescore.com/user/235731
 */
#ifndef __NOTES_H__
#define __NOTES_H__

#include "hw_types.h"
#include "prcm.h"

#define MAX_NOTES 10
#define POS_TO_FREQ(POS) POS * (875.0 / 100.0) + 250
#define FREQ_TO_POS(FREQ) (FREQ - 250) / (875.0 / 100.0)

typedef struct {
    int x;
    int start_ms;
    int length_ms;
} Note;

// note positions
#define  C4   FREQ_TO_POS(262)
#define  CS4  FREQ_TO_POS(277)
#define  D4   FREQ_TO_POS(294)
#define  DS4  FREQ_TO_POS(311)
#define  E4   FREQ_TO_POS(330)
#define  F4   FREQ_TO_POS(349)
#define  FS4  FREQ_TO_POS(370)
#define  G4   FREQ_TO_POS(392)
#define  GS4  FREQ_TO_POS(415)
#define  A4   FREQ_TO_POS(440)
#define  AS4  FREQ_TO_POS(466)
#define  B4   FREQ_TO_POS(494)
#define  C5   FREQ_TO_POS(523)
#define  CS5  FREQ_TO_POS(554)
#define  D5   FREQ_TO_POS(587)
#define  DS5  FREQ_TO_POS(622)
#define  E5   FREQ_TO_POS(659)
#define  F5   FREQ_TO_POS(698)
#define  FS5  FREQ_TO_POS(740)
#define  G5   FREQ_TO_POS(784)
#define  GS5  FREQ_TO_POS(831)
#define  A5   FREQ_TO_POS(880)
#define  AS5  FREQ_TO_POS(932)
#define  B5   FREQ_TO_POS(988)
#define  C6   FREQ_TO_POS(1047)
#define  CS6   FREQ_TO_POS(1109)

static const Note ditto_main[] = {
    {FS5, 104, 169},
    {FS5, 315, 372},
    {GS5, 740, 371},
    {A5, 1164, 371},
    {FS5, 1588, 371},
    {CS6, 2012, 774},
    {FS5, 3496, 169},
    {FS5, 3707, 372},
    {GS5, 4132, 371},
    {A5, 4556, 371},
    {CS6, 4980, 371},
    {B5, 5404, 573},
    {A5, 6040, 169},
    {GS5, 6251, 373},
    {E5, 6675, 171},
    {E5, 6888, 573},
    {CS5, 7524, 170},
    {CS5, 7735, 171},
    {E5, 8795, 271},
    {E5, 9113, 272},
    {E5, 9432, 169},
    {E5, 9643, 272},
    {CS5, 9962, 270},
    {D5, 10280, 169},
    {E5, 10492, 270},
    {E5, 10809, 271},
    {E5, 11339, 271},
    {E5, 11657, 272},
    {FS5, 11975, 171},
    {E5, 12187, 272},
    {CS5, 12611, 774},
    {FS5, 13883, 371},
    {GS5, 14307, 371},
    {A5, 14731, 371},
    {CS6, 15155, 372},
    {FS5, 15579, 272},
    {FS5, 15897, 272},
    {FS5, 16215, 170},
    {FS5, 16436, 369},
    {FS5, 17275, 271},
    {FS5, 17593, 271},
    {FS5, 17910, 172},
    {FS5, 18136, 364},
    {E5, 18970, 273},
    {E5, 19288, 273},
    {E5, 19607, 170},
    {E5, 19823, 374},
    {E5, 20667, 271},
    {E5, 20985, 271},
    {E5, 21302, 172},
    {E5, 21515, 271},
    {CS5, 21832, 272},
    {CS5, 22151, 171},
    {D5, 22362, 272},
    {D5, 22680, 272},
    {D5, 22999, 170},
    {D5, 23210, 378},
    {D5, 24059, 271},
    {D5, 24377, 271},
    {D5, 24694, 172},
    {D5, 24907, 276},
    {FS5, 25224, 272},
    {E5, 25543, 171},
    {CS5, 25754, 272},
    {CS5, 26072, 272},
    {CS5, 26391, 170},
    {CS5, 26602, 378},
    {FS5, 27451, 371},
    {GS5, 27875, 371},
    {A5, 28299, 372},
    {CS6, 28722, 373},
    {FS5, 29146, 272},
    {FS5, 29464, 272},
    {FS5, 29783, 170},
    {FS5, 29994, 378},
    {FS5, 30843, 270},
    {FS5, 31160, 271},
    {FS5, 31478, 172},
    {FS5, 31691, 270},
    {A5, 32008, 272},
    {GS5, 32327, 171},
    {E5, 32538, 272},
    {E5, 32857, 270},
    {E5, 33175, 170},
    {E5, 33386, 378},
    {E5, 34235, 270},
    {E5, 34552, 272},
    {E5, 34870, 171},
    {E5, 35083, 270},
    {CS5, 35400, 272},
    {CS5, 35719, 170},
    {D5, 35930, 272},
    {D5, 36249, 270},
    {D5, 36567, 170},
    {D5, 36778, 378},
    {D5, 37627, 270},
    {D5, 37944, 272},
    {D5, 38262, 171},
    {D5, 38475, 270},
    {FS5, 38792, 272},
    {E5, 39111, 170},
    {CS5, 39322, 272},
    {CS5, 39641, 270},
    {CS5, 39959, 169},
    {CS5, 40276, 272},
    {A4, 40595, 170},
    {FS4, 40806, 368}
};

static const Note ditto_back[] = {
    {110, 315, 200},
    {139, 527, 200},
    {165, 740, 199},
    {220, 951, 200},
    {277, 1163, 200},
    {330, 1375, 201},
    {277, 1587, 100},
    {220, 1694, 99},
    {165, 1799, 100},
    {139, 1905, 100},
    {73, 2011, 200},
    {110, 2224, 199},
    {147, 2435, 200},
    {185, 2648, 199},
    {220, 2859, 201},
    {147, 3071, 200},
    {220, 3283, 201},
    {147, 3495, 200},
    {73, 3707, 200},
    {110, 3919, 200},
    {147, 4132, 200},
    {185, 4343, 201},
    {220, 4555, 200},
    {147, 4767, 201},
    {220, 4979, 200},
    {147, 5191, 200},
    {69, 5403, 200},
    {104, 5616, 199},
    {139, 5827, 200},
    {165, 6040, 199},
    {208, 6251, 201},
    {139, 6463, 200},
    {208, 6675, 200},
    {139, 6887, 200},
    {92, 7099, 200},
    {139, 7311, 200},
    {185, 7524, 199},
    {220, 7735, 201},
    {277, 7947, 200},
    {185, 8159, 201},
    {277, 8371, 200},
    {185, 8583, 200},
    {62, 8795, 200},
    {92, 9007, 200},
    {123, 9219, 200},
    {147, 9431, 200},
    {185, 9643, 201},
    {123, 9855, 200},
    {185, 10067, 200},
    {123, 10279, 200},
    {82, 10491, 200},
    {123, 10703, 200},
    {165, 10915, 200},
    {208, 11127, 201},
    {247, 11339, 200},
    {165, 11551, 201},
    {247, 11763, 200},
    {165, 11975, 200},
    {110, 12187, 200},
    {139, 12399, 200},
    {165, 12611, 200},
    {220, 12823, 200},
    {277, 13035, 201},
    {165, 13247, 200},
    {277, 13459, 200},
    {165, 13671, 200},
    {185, 13883, 200},
    {208, 14094, 201},
    {220, 14306, 200},
    {247, 14518, 202},
    {277, 14730, 201},
    {247, 14942, 201},
    {220, 15154, 201},
    {208, 15366, 201},
    {73, 15578, 403},
    {147, 16002, 202},
    {73, 16214, 402},
    {73, 16638, 201},
    {147, 16850, 402},
    {73, 17275, 401},
    {147, 17699, 200},
    {73, 17910, 402},
    {73, 18334, 201},
    {147, 18547, 402},
    {69, 18970, 402},
    {139, 19394, 202},
    {69, 19606, 402},
    {69, 20030, 201},
    {139, 20242, 402},
    {92, 20667, 401},
    {139, 21090, 201},
    {92, 21302, 402},
    {92, 21726, 201},
    {139, 21938, 403},
    {62, 22362, 402},
    {123, 22786, 202},
    {62, 22998, 402},
    {62, 23422, 201},
    {123, 23634, 402},
    {82, 24059, 401},
    {165, 24482, 201},
    {82, 24694, 402},
    {82, 25118, 201},
    {165, 25330, 403},
    {55, 25754, 402},
    {139, 26178, 202},
    {55, 26390, 402},
    {55, 26814, 201},
    {139, 27026, 402},
    {55, 27451, 401},
    {139, 27874, 201},
    {55, 28086, 402},
    {55, 28510, 201},
    {139, 28722, 403},
    {73, 29146, 402},
    {147, 29570, 202},
    {73, 29782, 402},
    {73, 30206, 201},
    {147, 30419, 401},
    {73, 30843, 401},
    {147, 31266, 201},
    {73, 31478, 402},
    {73, 31902, 201},
    {147, 32114, 403},
    {69, 32538, 402},
    {139, 32962, 201},
    {69, 33174, 402},
    {69, 33598, 201},
    {139, 33811, 401},
    {92, 34235, 401},
    {139, 34658, 201},
    {92, 34870, 402},
    {92, 35294, 201},
    {139, 35506, 403},
    {62, 35930, 402},
    {123, 36354, 201},
    {62, 36566, 403},
    {62, 36990, 201},
    {123, 37203, 401},
    {82, 37627, 401},
    {165, 38050, 200},
    {82, 38262, 402},
    {82, 38687, 199},
    {165, 38898, 402},
    {55, 39322, 402},
    {139, 39746, 201},
    {55, 39958, 403},
    {55, 40382, 200},
    {139, 40595, 401},
    {55, 41018, 402},
    {139, 41442, 200},
    {55, 41654, 603},
    {55, 42290, 982},
};

static const Note world_mine_main[] = {
    {F5, 0, 348},
    {C6, 395, 348},
    {F5, 1581, 348},
    {DS5, 1976, 744},
    {F5, 2767, 348},
    {G5, 3162, 348},
    {GS5, 3557, 348},
    {G5, 3953, 348},
    {F5, 4348, 348},
    {DS5, 4743, 348},
    {C6, 5138, 744},
    {DS5, 5929, 348},
    {F5, 6324, 349},
    {C6, 6720, 743},
    {F5, 7510, 349},
    {F5, 7906, 348},
    {DS5, 8301, 348},
    {F5, 8696, 348},
    {F5, 9091, 546},
    {DS5, 9684, 151},
    {F5, 9882, 546},
    {GS5, 10673, 743},
    {C6, 11463, 3906},
    {DS5, 15812, 150},
    {F5, 16009, 151},
    {C5, 16207, 151},
    {C5, 16405, 150},
    {C5, 16602, 151},
    {DS5, 16997, 546},
    {C5, 17590, 151},
    {C5, 17986, 150},
    {C5, 18183, 151},
    {DS5, 18579, 348},
    {F5, 18974, 348},
    {C5, 19567, 151},
    {C5, 19765, 150},
    {DS5, 20160, 546},
    {C5, 20753, 150},
    {C5, 21148, 151},
    {C5, 21346, 150},
    {DS5, 21741, 151},
    {F5, 21939, 546},
    {C5, 22532, 150},
    {C5, 22729, 151},
    {C5, 22927, 151},
    {DS5, 23322, 546},
    {C5, 23915, 151},
    {C5, 24311, 150},
    {C5, 24508, 151},
    {DS5, 24904, 348},
    {D5, 25299, 348},
    {CS5, 25694, 1139},
    {DS5, 27275, 1535},
};

static const Note world_mine_back[] = {
    {69, 395, 196},
    {104, 592, 197},
    {156, 790, 197},
    {208, 988, 196},
    {311, 1185, 197},
    {415, 1383, 197},
    {554, 1581, 196},
    {831, 1778, 197},
    {78, 1976, 197},
    {117, 2174, 196},
    {156, 2371, 197},
    {196, 2569, 197},
    {311, 2767, 196},
    {392, 2964, 197},
    {554, 3162, 197},
    {784, 3360, 196},
    {87, 3557, 197},
    {131, 3755, 196},
    {156, 3953, 196},
    {196, 4150, 197},
    {349, 4348, 196},
    {415, 4545, 197},
    {523, 4743, 197},
    {698, 4941, 196},
    {932, 5138, 395},
    {831, 5534, 394},
    {784, 5929, 394},
    {831, 6324, 395},
    {1047, 6720, 789},
    {466, 7510, 790},
    {415, 8301, 789},
    {392, 9091, 790},
    {415, 9882, 790},
    {466, 10673, 789},
    {622, 11463, 790},
    {1245, 12254, 3161},
    {87, 16207, 197},
    {87, 16405, 196},
    {87, 16602, 197},
    {104, 16997, 592},
    {87, 17590, 197},
    {87, 17986, 196},
    {87, 18183, 197},
    {104, 18579, 394},
    {73, 18974, 394},
    {87, 19567, 197},
    {87, 19765, 196},
    {104, 20160, 592},
    {87, 20753, 196},
    {87, 21148, 197},
    {87, 21346, 196},
    {104, 21741, 197},
    {117, 21939, 592},
    {87, 22532, 196},
    {87, 22729, 197},
    {87, 22927, 197},
    {104, 23322, 592},
    {87, 23915, 197},
    {87, 24311, 196},
    {87, 24508, 197},
    {104, 24904, 394},
    {73, 25299, 394},
    {349, 25694, 197},
    {415, 25892, 196},
    {466, 26089, 197},
    {494, 26287, 197},
    {466, 26485, 196},
    {415, 26682, 197},
    {622, 26880, 197},
    {587, 27078, 196},
    {554, 27275, 197},
    {622, 27473, 197},
    {622, 27671, 196},
    {622, 27868, 197},
    {698, 28066, 197},
    {698, 28264, 196},
    {523, 28461, 197},
    {622, 28659, 197},
    {698, 28857, 196},
    {698, 29054, 592},
};

static const Note toreador_march_main[] = {
    {B5, 0, 223},
    {CS6, 510, 223},
    {B5, 765, 224},
    {GS5, 1020, 224},
    {GS5, 1530, 224},
    {GS5, 2040, 224},
    {FS5, 2295, 224},
    {GS5, 2551, 223},
    {A5, 2806, 223},
    {GS5, 3061, 223},
    {A5, 4081, 224},
    {FS5, 4591, 224},
    {B5, 4846, 224},
    {GS5, 5102, 223},
    {E5, 6122, 224},
    {CS5, 6632, 224},
    {FS5, 6887, 224},
    {B4, 7142, 224},
    {FS5, 8163, 224},
    {FS5, 9183, 224},
    {CS6, 9438, 224},
    {B5, 9693, 224},
    {A5, 9948, 224},
    {GS5, 10204, 223},
    {FS5, 10459, 223},
    {GS5, 10714, 224},
    {A5, 10969, 224},
    {GS5, 11224, 224},
    {DS5, 11989, 224},
    {DS5, 12244, 224},
    {GS5, 12755, 223},
    {GS5, 13265, 224},
    {G5, 13775, 224},
    {AS5, 14030, 224},
    {GS5, 14285, 224},
};

static const Note toreador_march_back[] = {
    {262, 0, 251},
    {175, 507, 254},
    {262, 1017, 254},
    {175, 1527, 254},
    {262, 2038, 253},
    {175, 2548, 254},
    {262, 3058, 254},
    {175, 3571, 251},
    {233, 4078, 254},
    {196, 4589, 253},
    {220, 5099, 254},
    {175, 5609, 254},
    {175, 6119, 254},
    {147, 6629, 254},
    {131, 7142, 251},
    {131, 7650, 254},
    {196, 8160, 254},
    {131, 8670, 254},
    {196, 9180, 254},
    {131, 9691, 253},
    {175, 10201, 254},
    {131, 10714, 251},
    {175, 11221, 254},
    {131, 11732, 253},
    {165, 12242, 253},
    {131, 12752, 254},
    {165, 13262, 254},
    {123, 13772, 254},
    {110, 14285, 251},
    {131, 14793, 254},
    {110, 15303, 254},
    {880, 15303, 254},
};

static const Note miracle_paint_A[] = {
    {208, 8462, 251},
    {131, 8726, 144},
    {156, 8991, 125},
    {156, 9123, 62},
    {131, 9389, 61},
    {156, 9652, 63},
    {117, 9935, 251},
    {87, 10199, 107},
    {104, 10313, 144},
    {117, 10464, 107},
    {131, 10578, 250},
    {131, 10842, 63},
    {139, 10908, 63},
    {156, 10975, 268},
    {131, 11257, 108},
    {156, 11371, 144},
    {175, 11521, 108},
    {262, 11786, 108},
    {175, 11901, 82},
    {156, 11988, 84},
    {117, 12076, 84},
    {123, 12165, 63},
    {131, 12231, 62},
    {156, 12297, 125},
    {123, 12429, 62},
    {131, 12495, 63},
    {156, 12561, 63},
    {330, 12628, 62},
    {349, 12694, 143},
    {311, 12958, 143},
    {262, 13373, 108},
    {233, 13487, 62},
    {208, 13620, 268},
    {175, 13903, 107},
    {156, 14016, 144},
    {131, 14166, 234},
    {117, 14414, 61},
    {123, 14479, 62},
    {131, 14545, 62},
    {104, 14677, 269},
    {87, 14960, 108},
    {104, 15074, 83},
    {87, 15162, 83},
    {104, 15251, 83},
    {123, 15339, 83},
    {131, 15426, 84},
    {156, 15514, 84},
    {131, 15603, 83},
    {123, 15691, 84},
    {117, 15780, 83},
    {104, 15868, 142},
    {104, 16018, 108},
    {156, 16283, 233},
    {311, 16529, 62},
    {156, 16596, 61},
    {311, 16661, 62},
    {156, 16727, 63},
    {311, 16793, 63},
    {156, 16860, 20},
    {311, 16882, 19},
    {156, 16904, 83},
    {104, 17190, 63},
    {104, 17587, 62},
    {92, 17983, 251},
    {104, 18380, 62},
    {104, 18663, 359},
    {87, 19306, 62},
    {104, 19721, 233},
    {92, 19967, 62},
    {87, 20034, 62},
    {82, 20100, 188},
    {69, 20365, 142},
    {104, 20515, 358},
    {139, 20893, 251},
    {98, 21422, 62},
    {98, 21819, 62},
    {92, 22084, 62},
    {92, 22347, 64},
    {104, 22612, 62},
    {104, 22895, 359},
    {104, 23274, 143},
    {104, 23424, 108},
    {87, 23689, 251},
    {69, 23953, 108},
    {104, 24067, 143},
    {117, 24218, 107},
    {123, 24332, 250},
    {123, 24596, 251},
    {117, 24860, 251},
    {104, 25125, 251},
    {104, 25654, 62},
    {104, 26051, 62},
    {92, 26448, 251},
    {104, 26844, 62},
    {104, 27127, 359},
    {87, 27770, 62},
    {104, 28185, 233},
    {92, 28431, 63},
    {87, 28498, 62},
    {82, 28564, 187},
    {52, 28761, 64},
    {69, 28828, 143},
    {104, 28979, 358},
    {139, 29357, 251},
    {98, 29622, 501},
    {78, 30151, 502},
    {92, 30680, 502},
    {110, 31209, 250},
    {117, 31623, 359},
    {92, 32002, 251},
    {117, 32267, 250},
    {139, 32530, 189},
    {165, 32730, 62},
    {175, 32796, 250},
    {156, 33060, 143},
    {131, 33210, 107},
    {104, 33476, 358},
    {87, 33854, 250},
    {131, 34250, 63},
    {131, 34532, 359},
    {82, 34912, 391},
    {123, 35328, 358},
    {82, 35706, 250},
    {78, 35969, 144},
    {65, 36234, 144},
    {117, 36384, 108},
    {117, 36649, 359},
    {196, 37094, 62},
    {208, 37160, 125},
    {196, 37292, 143},
    {156, 37442, 252},
    {131, 37707, 107},
    {104, 37821, 251},
    {87, 38085, 392},
    {131, 38500, 359},
    {87, 38879, 251},
    {117, 39144, 251},
    {104, 39407, 251},
    {98, 39672, 251},
    {104, 39937, 251},
    {117, 40400, 62},
    {131, 40466, 144},
    {156, 40616, 108},
    {117, 40881, 107},
    {131, 40994, 144},
    {156, 41145, 108},
    {196, 41259, 144},
    {208, 41409, 108},
    {220, 41524, 143},
    {233, 41674, 252},
    {196, 41939, 107},
    {156, 42053, 251},
    {62, 42317, 377},
    {117, 42714, 62},
    {117, 42997, 251},
    {104, 43375, 251},
    {139, 43791, 359},
    {123, 44169, 251},
    {117, 44433, 144},
    {117, 44698, 251},
    {104, 44962, 143},
    {117, 45227, 251},
    {98, 45491, 502},
    {104, 46021, 251},
    {117, 46284, 251},
    {123, 46549, 503},
    {123, 47078, 144},
    {78, 47229, 107},
    {104, 47343, 125},
    {156, 47475, 62},
    {156, 47758, 252},
    {123, 48023, 107},
    {156, 48136, 251},
    {185, 48401, 251},
    {156, 48665, 252},
    {58, 48930, 83},
    {44, 49018, 83},
    {65, 49106, 84},
    {78, 49194, 83},
    {65, 49283, 82},
    {78, 49371, 82},
    {104, 49459, 83},
    {131, 49546, 84},
    {156, 49635, 83},
    {175, 49723, 377},
    {196, 50120, 62},
    {139, 50516, 251},
    {147, 50781, 251},
    {87, 51046, 251},
    {117, 51310, 251},
    {147, 51575, 251},
    {139, 51839, 377},
    {117, 52236, 63},
    {117, 52518, 359},
    {131, 52898, 251},
    {82, 53162, 251},
    {110, 53426, 251},
    {131, 53691, 251},
    {92, 53955, 393},
    {123, 54370, 359},
    {92, 54748, 251},
    {69, 55164, 107},
    {73, 55278, 143},
    {117, 55428, 252},
    {110, 55693, 107},
    {87, 55807, 143},
    {65, 55957, 108},
    {73, 56071, 251},
    {98, 56337, 250},
    {87, 56600, 251},
    {82, 56865, 251},
    {87, 57130, 501},
    {87, 57658, 125},
    {82, 57791, 62},
    {78, 58073, 362},
    {117, 58452, 251},
    {147, 58716, 251},
    {131, 58980, 251},
    {87, 59245, 502},
    {62, 59773, 246},
    {73, 60039, 251},
    {87, 60303, 251},
    {69, 60568, 251},
    {78, 60832, 251},
    {69, 61097, 250},
    {82, 61362, 375},
    {131, 61758, 62},
    {131, 62041, 358},
    {123, 62419, 136},
    {294, 62570, 107},
    {247, 62684, 83},
    {196, 62772, 84},
    {175, 62860, 83},
    {147, 62948, 83},
    {131, 63037, 82},
    {123, 63125, 83},
    {98, 63212, 84},
    {73, 63300, 84},
    {62, 63389, 83},
    {117, 63477, 251},
    {131, 63742, 250},
    {117, 64007, 143},
    {110, 64157, 107},
    {98, 64421, 108},
    {131, 64800, 251},
    {65, 65064, 83},
    {73, 65152, 84},
    {87, 65241, 83},
    {98, 65329, 83},
    {117, 65417, 83},
    {131, 65505, 146},
    {262, 65727, 124},
    {1175, 65859, 84},
    {1047, 65948, 83},
    {784, 66036, 83},
    {831, 66124, 62},
    {880, 66190, 62},
    {988, 66256, 33},
    {1047, 66289, 31},
    {831, 66322, 63},
    {784, 66388, 63},
    {698, 66455, 125},
    {554, 66587, 62},
    {587, 66653, 143},
    {523, 66803, 108},
    {392, 66917, 144},
    {440, 67068, 107},
    {494, 67226, 41},
    {523, 67270, 104},
    {587, 67446, 63},
    {523, 67579, 62},
    {698, 67682, 170},
};

static const Note miracle_paint_B[] = {
    {831, 0, 148},
    {831, 264, 148},
    {831, 528, 96},
    {831, 679, 113},
    {831, 943, 113},
    {831, 1208, 113},
    {831, 1450, 113},
    {831, 1586, 150},
    {831, 1851, 150},
    {831, 2115, 148},
    {831, 2380, 170},
    {831, 2644, 150},
    {831, 2822, 112},
    {932, 3059, 264},
    {740, 3324, 113},
    {1109, 3438, 150},
    {1047, 3588, 113},
    {932, 3853, 113},
    {831, 3967, 147},
    {831, 4231, 149},
    {831, 4496, 159},
    {831, 4760, 150},
    {831, 4964, 113},
    {831, 5175, 113},
    {831, 5440, 113},
    {831, 5704, 113},
    {831, 5851, 149},
    {932, 6083, 264},
    {831, 6347, 148},
    {831, 6612, 137},
    {831, 6876, 150},
    {831, 7054, 112},
    {831, 7291, 113},
    {740, 8085, 263},
    {932, 8349, 113},
    {831, 8463, 150},
    {622, 8614, 113},
    {698, 8728, 150},
    {831, 8878, 264},
    {622, 9143, 113},
    {698, 9257, 150},
    {1047, 9407, 264},
    {1109, 9672, 113},
    {1175, 9786, 150},
    {1245, 9936, 113},
    {932, 10201, 113},
    {831, 10315, 150},
    {932, 10465, 113},
    {1047, 10579, 150},
    {698, 10730, 113},
    {740, 10844, 150},
    {784, 10994, 264},
    {659, 11259, 113},
    {523, 11373, 264},
    {932, 11637, 264},
    {831, 11902, 150},
    {831, 12052, 113},
    {622, 12317, 113},
    {698, 12431, 150},
    {622, 12581, 113},
    {831, 12695, 150},
    {622, 12846, 113},
    {698, 12960, 150},
    {831, 13110, 113},
    {622, 13375, 113},
    {698, 13489, 150},
    {1047, 13639, 264},
    {1109, 13904, 113},
    {1175, 14018, 150},
    {1245, 14168, 113},
    {1047, 14433, 113},
    {932, 14697, 312},
    {831, 15076, 793},
    {622, 16549, 113},
    {587, 16663, 150},
    {622, 16813, 113},
    {932, 16927, 264},
    {784, 17192, 150},
    {784, 17342, 113},
    {698, 17607, 113},
    {698, 17871, 113},
    {622, 18136, 113},
    {622, 18400, 264},
    {622, 18665, 113},
    {587, 18779, 150},
    {622, 18929, 113},
    {831, 19043, 264},
    {698, 19308, 264},
    {831, 19572, 264},
    {932, 19837, 264},
    {988, 20101, 264},
    {932, 20366, 264},
    {831, 20630, 264},
    {988, 20895, 264},
    {932, 21159, 529},
    {784, 21688, 529},
    {622, 22217, 529},
    {523, 22746, 529},
    {831, 23275, 529},
    {698, 23955, 113},
    {831, 24069, 150},
    {988, 24219, 378},
    {932, 24598, 150},
    {831, 24748, 113},
    {831, 25127, 264},
    {932, 25391, 264},
    {784, 25656, 150},
    {784, 25806, 113},
    {698, 26071, 113},
    {698, 26335, 113},
    {622, 26600, 113},
    {622, 26864, 264},
    {622, 27129, 113},
    {587, 27243, 150},
    {622, 27393, 113},
    {831, 27507, 264},
    {698, 27772, 264},
    {831, 28036, 264},
    {932, 28301, 264},
    {988, 28565, 264},
    {932, 28830, 264},
    {831, 29094, 264},
    {932, 29359, 264},
    {1047, 29623, 529},
    {831, 30152, 529},
    {1047, 30681, 529},
    {1397, 31210, 412},
    {1245, 31625, 907},
    {1397, 32797, 264},
    {1245, 33062, 150},
    {1047, 33212, 113},
    {831, 33477, 113},
    {831, 33741, 113},
    {1245, 33855, 264},
    {1109, 34120, 150},
    {1109, 34270, 113},
    {831, 34649, 264},
    {1245, 34913, 264},
    {1109, 35178, 264},
    {1047, 35442, 264},
    {1109, 35707, 264},
    {1109, 35971, 264},
    {1047, 36236, 150},
    {1047, 36386, 642},
    {698, 37294, 264},
    {659, 37558, 264},
    {698, 37823, 264},
    {1047, 38087, 264},
    {698, 38352, 264},
    {831, 38616, 264},
    {1047, 38881, 150},
    {932, 39031, 378},
    {831, 39410, 264},
    {784, 39674, 264},
    {831, 39939, 264},
    {784, 40203, 264},
    {831, 40468, 150},
    {784, 40618, 113},
    {622, 40883, 113},
    {523, 41147, 642},
    {622, 41790, 264},
    {698, 42055, 264},
    {740, 42319, 529},
    {932, 42848, 529},
    {831, 43377, 264},
    {554, 43642, 264},
    {1109, 43906, 264},
    {988, 44171, 264},
    {932, 44435, 150},
    {932, 44700, 149},
    {880, 44964, 150},
    {932, 45115, 112},
    {1245, 45379, 377},
    {1109, 45758, 264},
    {988, 46022, 264},
    {932, 46287, 264},
    {932, 46551, 264},
    {740, 46816, 149},
    {740, 46966, 642},
    {622, 48288, 113},
    {740, 48403, 149},
    {831, 48553, 113},
    {932, 48667, 1470},
    {1047, 50140, 113},
    {880, 50783, 264},
    {587, 51048, 263},
    {698, 51312, 264},
    {880, 51577, 263},
    {880, 51841, 412},
    {831, 52256, 642},
    {784, 52899, 264},
    {523, 53164, 263},
    {659, 53428, 264},
    {784, 53692, 264},
    {784, 53957, 412},
    {698, 54372, 642},
    {587, 55279, 264},
    {698, 55544, 264},
    {880, 55808, 264},
    {784, 56073, 264},
    {523, 56337, 264},
    {554, 56602, 264},
    {932, 56866, 264},
    {880, 57131, 264},
    {784, 57395, 264},
    {698, 57660, 264},
    {1175, 57924, 529},
    {1047, 58453, 264},
    {880, 58718, 264},
    {784, 58982, 264},
    {698, 59247, 264},
    {466, 59776, 150},
    {587, 59926, 113},
    {880, 60040, 150},
    {784, 60191, 377},
    {784, 60569, 264},
    {880, 60834, 264},
    {932, 61098, 264},
    {1047, 61363, 528},
    {880, 61892, 528},
    {1397, 62421, 528},
    {1047, 63214, 264},
    {932, 63479, 264},
    {1047, 63743, 264},
    {932, 64008, 150},
    {880, 64158, 113},
    {784, 64423, 377},
    {587, 65066, 264},
    {659, 65330, 150},
    {698, 65481, 263},
};

static const char* song_names[] = {
    "ToreadorMarch",
    "Ditto",
    "World is Mine"
};

static const char* artist_names[] = {
    "Georges Bizet",
    "NewJeans",
    "Hatsune Miku"
};

static const Note* songs_main[] = {
    toreador_march_main,
    ditto_main,
    world_mine_main
};

static const Note* songs_back[] = {
    toreador_march_back,
    ditto_back,
    world_mine_back
};

static const int songs_main_sizes[] = {
    sizeof(toreador_march_main) / sizeof(Note),
    sizeof(ditto_main) / sizeof(Note),
    sizeof(world_mine_main) / sizeof(Note)
};

static const int songs_back_sizes[] = {
    sizeof(toreador_march_back) / sizeof(Note),
    sizeof(ditto_back) / sizeof(Note),
    sizeof(world_mine_back) / sizeof(Note)
};

static const int songCount = sizeof(songs_main) / sizeof(Note*);

#endif // __NOTES_H__
