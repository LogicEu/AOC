#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

typedef struct ivec2 {
    int x;
    int y;
} ivec2;

static const char* passcode = "qljzarfv";

static const ivec2 vault = {3, 3};
static char trail[0xffff];
static char mintrail[0xffff];
static char maxtrail[0xffff];
static int mincount = 0xffff;
static int maxcount = 0;

static inline void md5Print(char* buf)
{
    char buffer[4];
    unsigned char* p = md5String(buf);
    sprintf(buf, "%02x", p[0]);
    for (unsigned int i = 1; i < 16; ++i) {
        sprintf(buffer, "%02x", p[i]);
        strcat(buf, buffer);
    }
    free(p);
}

static void md5Run(char* buf, const ivec2 p, const int trailcount)
{
    static const char wasd[4] = {'U', 'D', 'L', 'R'};
    static const ivec2 dirs[4] = {
        {0, -1},
        {0, +1},
        {-1, 0},
        {+1, 0}
    };

    if (trailcount >= mincount) {
        return;
    }

    if (p.x == vault.x && p.y == vault.y) {
        strcpy(mintrail, trail);
        mincount = trailcount;
        return;
    }

    char mem[4];
    memcpy(mem, buf, 4);

    for (int i = 0; i < 4; ++i) {
        if (!(mem[i] > 'a' && mem[i] < 'g')) {
            continue;
        }

        const ivec2 v = {p.x + dirs[i].x, p.y + dirs[i].y};
        if (v.x >= 0 && v.y >= 0 && v.x < 4 && v.y < 4) {
            trail[trailcount] = wasd[i];
            sprintf(buf, "%s%s", passcode, trail);
            md5Print(buf);
            md5Run(buf, v, trailcount + 1);
            trail[trailcount] = 0;
        }
    }
}


static void md5Run2(char* buf, const ivec2 p, const int trailcount)
{
    static const char wasd[4] = {'U', 'D', 'L', 'R'};
    static const ivec2 dirs[4] = {
        {0, -1},
        {0, +1},
        {-1, 0},
        {+1, 0}
    };

    if (p.x == vault.x && p.y == vault.y) {
        if (trailcount > maxcount) {
            maxcount = trailcount;
            strcpy(maxtrail, trail);
        }
        return;
    }

    char mem[4];
    memcpy(mem, buf, 4);

    for (int i = 0; i < 4; ++i) {
        if (!(mem[i] > 'a' && mem[i] < 'g')) {
            continue;
        }

        const ivec2 v = {p.x + dirs[i].x, p.y + dirs[i].y};
        if (v.x >= 0 && v.y >= 0 && v.x < 4 && v.y < 4) {
            trail[trailcount] = wasd[i];
            sprintf(buf, "%s%s", passcode, trail);
            md5Print(buf);
            md5Run2(buf, v, trailcount + 1);
            trail[trailcount] = 0;
        }
    }
}

int main(void)
{
    char buf[0xffff];
    const ivec2 p = {0, 0};
    
    strcpy(buf, passcode);
    md5Print(buf);
    md5Run(buf, p, 0);
    printf("Puzzle 1: %s\n", mintrail);
    
    strcpy(buf, passcode);
    md5Print(buf);
    md5Run2(buf, p, 0);
    printf("Puzzle 2: %zu\n", strlen(maxtrail));
    
    return EXIT_SUCCESS;
}
