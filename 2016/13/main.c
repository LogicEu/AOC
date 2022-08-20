#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM 1364

typedef struct ivec2 {
    int x;
    int y;
} ivec2;

static const ivec2 dest = {31, 39};
static const long maxsteps = 50;
static long minsteps = 0xffff;
static long puzzle2;

static int countbits(const long n)
{
    int count = 0;
    unsigned char* bytes = (unsigned char*)&n;
    for (int i = sizeof(long) - 1; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {
            count += (bytes[i] >> j) & 1;
        }
    }
    return count;
}

static ivec2 getdir(const int rot)
{
    static const ivec2 dirs[4] = {
        {0, 1},
        {1, 0},
        {0, -1},
        {-1, 0}
    };
    return dirs[rot];
}

static inline int check(const ivec2 v)
{
    long n = v.x * v.x + 3 * v.x + 2 * v.x * v.y + v.y + v.y * v.y + NUM;
    return countbits(n) % 2;
}

static void step(const ivec2 v, const long steps) 
{
    static ivec2 trail[0xffff];
    static long steptrail[0xffff];
    static long trailcount = 0;

    int found = 0;
    for (long i = 0; i < trailcount; ++i) {
        if (!memcmp(&v, trail + i, sizeof(ivec2))) {
            if (steps < steptrail[i]) {
                steptrail[i] = steps;
                ++found;
                break;
            }
            else return;
        }
    }

    if (!found) {
        trail[trailcount] = v;
        steptrail[trailcount] = steps;
        ++trailcount;
    }

    if (steps >= minsteps) {
        return;
    }

    if (v.x == dest.x && v.y == dest.y) {
        minsteps = steps;
        return;
    }

    for (int i = 0; i < 4; ++i) {
        const ivec2 dir = getdir(i);
        const ivec2 p = {v.x + dir.x, v.y + dir.y};
        if (p.x >= 0 && p.y >= 0 && !check(p)) {
            step(p, steps + 1);
        }
    }
}

static void step2(const ivec2 v, const long steps) 
{
    static ivec2 trail[0xffff];
    static long steptrail[0xffff];
    static long trailcount = 0;

    int found = 0;
    for (long i = 0; i < trailcount; ++i) {
        if (!memcmp(&v, trail + i, sizeof(ivec2))) {
            if (steps < steptrail[i]) {
                steptrail[i] = steps;
                ++found;
                break;
            }
            else return;
        }
    }

    if (!found) {
        trail[trailcount] = v;
        steptrail[trailcount] = steps;
        ++trailcount;
    }

    if (steps >= maxsteps) {
        return;
    }

    for (int i = 0; i < 4; ++i) {
        const ivec2 dir = getdir(i);
        const ivec2 p = {v.x + dir.x, v.y + dir.y};
        if (p.x >= 0 && p.y >= 0 && !check(p)) {
            step2(p, steps + 1);
        }
    }

    puzzle2 = trailcount;
}

int main(void)
{
    const ivec2 p = {1, 1};
    
    step(p, 0);
    printf("Puzzle 1: %ld\n", minsteps);

    step2(p, 0);
    printf("Puzzle 2: %ld\n", puzzle2);
    
    return EXIT_SUCCESS;
}
