#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int numcheck2(const char* s)
{
    int adj = 0, i, j;
    for (i = 0; s[i + 1]; ++i) {
        if (s[i] > s[i + 1]) {
            return 0;
        }
    }
    for (i = 0; s[i]; ++i) {
        for (j = 0; s[j + i] == s[j + 1 + i]; ++j);
        adj += j == 1;
        i = i + j;
    }
    return !!adj;
}

static int numcheck(const char* s)
{
    int adj = 0, i;
    for (i = 0; s[i + 1]; ++i) {
        adj += s[i] == s[i + 1];
        if (s[i] > s[i + 1]) {
            return 0;
        }
    }
    return !!adj;
}

int main(void)
{
    char numstr[0xf];
    int count = 0, count2 = 0;
    const int min = 357253, max = 892942;
    for (int i = min; i <= max; ++i) {
        sprintf(numstr, "%d", i);
        count += numcheck(numstr);
        count2 += numcheck2(numstr);
    }
    printf("Puzzle 1: %d\n", count);
    printf("Puzzle 2: %d\n", count2);
    return EXIT_SUCCESS;
}
