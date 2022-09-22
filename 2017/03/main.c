#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 0xff
#define HLEN (LEN / 2)
#define ARRAT(arr, x, y) arr[((y) + HLEN) * LEN + (x) + HLEN]

static long calc(const long input)
{
    long sign = 1, dist = 2, maxdist = 1;
    long x = 0, y = 0;
    for (long i = 1; i < input; ++i) {
        int q = (dist > maxdist);
        x += sign * q;
        y += sign * !q;
        --dist;
        if (!dist) {
            ++maxdist;
            dist = maxdist * 2;
            sign = -sign;
        }
    }
    return labs(x) + labs(y);
}

static long lookneighbours(const long* arr, const long x, const long y)
{
    long count = 0;
    count += ARRAT(arr, x + 1, y);
    count += ARRAT(arr, x + 1, y - 1);
    count += ARRAT(arr, x, y - 1);
    count += ARRAT(arr, x - 1, y - 1);
    count += ARRAT(arr, x - 1, y);
    count += ARRAT(arr, x - 1, y + 1);
    count += ARRAT(arr, x, y + 1);
    count += ARRAT(arr, x + 1, y + 1);
    return count;
}

static long calc2(const long input)
{
    long arr[LEN * LEN] = {0};

    long sign = 1, dist = 2, maxdist = 1;
    long x = 0, y = 0;
    ARRAT(arr, x, y) = 1;

    for (long i = 2; i < input; ++i) {
        int q = (dist > maxdist);
        x += sign * q;
        y += sign * !q;
        --dist;
        if (!dist) {
            ++maxdist;
            dist = maxdist * 2;
            sign = -sign;
        }
        
        ARRAT(arr, x, y) = lookneighbours(arr, x, y);
        if (ARRAT(arr, x, y) > input) {
            return ARRAT(arr, x, y);
        }
    }

    return 0;
}

int main(const int argc, const char** argv)
{
    long input = 361527;
    if (argc > 1) {
        input = atol(argv[1]);
    }
    long r = calc(input);
    printf("Puzzle 1: %ld\n", r);
    
    long r2 = calc2(input);
    printf("Puzzle 2: %ld\n", r2);
    
    return EXIT_SUCCESS;
}
