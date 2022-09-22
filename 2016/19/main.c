#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM 3004953

static long puzzle1(void)
{
    long i = 0, j, ret;
    char nums[NUM];
    memset(nums, 1, NUM);

    while (1) {
        if (nums[i]) {
            
            j = (i + 1) % NUM;
            while (!nums[j]) {
                j = (j + 1) % NUM;
            }
            
            if (i == j) {
                ret = i + 1;
                break;
            }
            nums[j] = 0;

        }
        i = (i + 1) % NUM;
    }
    return ret;
}

static long puzzle2(void)
{
    long i = 0, j, len = NUM, ret;
    char nums[NUM];
    memset(nums, 1, NUM);

    while (1) {
        if (nums[i]) {
            
            j = (i + 1) % NUM;
            const long hlen = len / 2;

            long skips = !!nums[j];
            while (skips < hlen) {
                j = (j + 1) % NUM;
                skips += !!nums[j];
            }
            
            nums[j] = 0;
            --len;

            printf("%ld, %ld\n", len, j); 
            if (i == j || len == 1) {
                ret = i + 1;
                break;
            }
        }
        i = (i + 1) % NUM;
    }
    return ret;
}

int main(void)
{
    printf("Puzzle 1: %ld\n", puzzle1());
    printf("Puzzle 2: %ld\n", puzzle2());
    return EXIT_SUCCESS;
}
