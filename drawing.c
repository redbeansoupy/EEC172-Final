#include "drawing.h"
#include "notes.h"
#include "nunchuk.h"
#include "uart_if.h"

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1351.h"

unsigned int g_startTimeMS;
unsigned int g_songIdx;

void DrawSprite (const Sprite *sprite, int xPos, int yPos, unsigned char bg_color) {
    int w = sprite->width;
    int h = sprite->height;
    const unsigned char *data = sprite->pixel_data;

    int x, y;
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            int idx = (y * w + x) * sprite->bytes_per_pixel;
            unsigned short color = data[idx] | (data[idx + 1] << 8);

            if (color != bg_color) {
                drawPixel(x + xPos, y + yPos, color);
            }
        }
    }
}

void DrawSongInfo(int songIdx)
{
    unsigned char c, x = 0;

    // song name
    const char* song_name = song_names[songIdx];
    while ((c = song_name[x++]) != '\0') {
        drawChar(22+6*x, 110, c, 0xFFFFF, BG_COLOR, 1);
    }

    // artist name
    const char* artist_name = artist_names[songIdx];
    x = 0;
    while ((c = artist_name[x++]) != '\0') {
        drawChar(22+6*x, 120, c, 0xFFFFF, BG_COLOR, 1);
    }
}

void ClearSongInfo(int songIdx)
{
    unsigned char x = 0;

    // song name
    const char* song_name = song_names[songIdx];
    while (song_name[x++] != '\0') {
        drawChar(22+6*x, 110, 0, 0xFFFFF, BG_COLOR, 1);
    }

    // artist name
    const char* artist_name = artist_names[songIdx];
    x = 0;
    while (artist_name[x++] != '\0') {
        drawChar(22+6*x, 120, 0, 0xFFFFF, BG_COLOR, 1);
    }
}

void DrawBugle(int currPos)
{
    static int prevPos = OLED_WIDTH - MIKU_WIDTH - BUGLE_WIDTH - BUGLE_WIDTH;
    if (currPos != prevPos) {
        DrawSprite((const Sprite*)&bugle, currPos, OLED_HEIGHT - miku.height + 2, 0x0000);

        // cover up bugle
        if (currPos > prevPos) {
            fillRect(prevPos, OLED_HEIGHT-miku.height+2, currPos - prevPos, bugle.height, BG_COLOR);
        } else {
            fillRect(currPos + BUGLE_WIDTH, OLED_HEIGHT-miku.height+2, prevPos - currPos, 4, BG_COLOR);
            fillRect(currPos + BUGLE_WIDTH, OLED_HEIGHT-miku.height+6, prevPos - currPos, 2, BUGLE_COLOR);
            fillRect(currPos + BUGLE_WIDTH, OLED_HEIGHT-miku.height+8, prevPos - currPos, 4, BG_COLOR);
        }
    }
    prevPos = currPos;
}

void DrawNotes(unsigned char reset)
{
    static int noteIdx;
    int numFinishedNotes = 0;
    int totalNotes = songs_main_sizes[g_songIdx];
    static int prevTails[1024];
    long curr_time_ms = (PRCMSlowClkCtrGet() * 1000) / 32768 - g_startTimeMS;
    if (reset) {
        noteIdx = 0;
        int i;
        for (i=0; i>1024; i++) {
            prevTails[i] = 0;
        }
        return;
    }

    int i;
    for (i = 0; i < MAX_NOTES; i++) {
        if (noteIdx + i >= totalNotes) break;
        Note note = songs_main[g_songIdx][noteIdx + i];
        int prev_tail = prevTails[noteIdx + i];
        int note_end_ms = note.start_ms + note.length_ms;
        if (curr_time_ms > note_end_ms) {
            // finished note
            fillRect(note.x - 1, prev_tail, 3, TRACK_HEIGHT-prev_tail, BG_COLOR);
            numFinishedNotes++;
        } else if (curr_time_ms >= note.start_ms - TRACK_DURATION) {
            // note is visible
            int new_head = (curr_time_ms - note.start_ms + TRACK_DURATION) * (110.0 / TRACK_DURATION);
            new_head = (new_head < TRACK_HEIGHT) ? new_head : 110;

            int new_tail = TRACK_HEIGHT - (note_end_ms - curr_time_ms) * (110.0 / TRACK_DURATION);
            new_tail = (new_tail >= 0) ? new_tail : 0;

            fillRect(note.x - 1, new_tail, 3, new_head-new_tail, MAGENTA);
            if (new_tail > 0){
                fillRect(note.x - 1, prev_tail, 3, new_tail-prev_tail, BG_COLOR);
            }

            prevTails[noteIdx + i] = new_tail;
        }
    }
    noteIdx += numFinishedNotes;
}

void DrawScore(unsigned int uScore) {
    char digits[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    unsigned char x = OLED_WIDTH - 19;
    unsigned char y = OLED_HEIGHT - miku.height - 8;
    unsigned char hundred = uScore / 100;
    unsigned char ten = (uScore / 10) % 10;
    unsigned char one = uScore % 10;

    drawChar(x, y, digits[hundred], 0xFFFFF, BG_COLOR, 1);
    drawChar(x + 6, y, digits[ten], 0xFFFFF, BG_COLOR, 1);
    drawChar(x + 12, y, digits[one],  0xFFFFF, BG_COLOR, 1);
}
