#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const long factorA = 16807, factorB = 48271, factorC = 2147483647;

static inline int bincmp(long A, long B)
{   
    char bufA[0xff], bufB[0xff];
    uint8_t* a = (uint8_t*)&A, *b = (uint8_t*)&B, n = 0;
    for (int i = 1; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {
            bufA[n] = ((a[i] >> j) & 1) + '0';
            bufB[n] = ((b[i] >> j) & 1) + '0';
            ++n;
        }
    }
    bufA[n] = bufB[n] = 0;
    return !memcmp(bufA, bufB, 16);
}

static long puzzle1(long A, long B)
{
    long i, count = 0;
    for (i = 0; i < 40000000; ++i) {
        A = (A * factorA) % factorC;
        B = (B * factorB) % factorC;
        count += bincmp(A, B);
    }
    return count;
}

static long puzzle2(long A, long B)
{
    long i, count = 0;
    for (i = 0; i < 5000000; ++i) {
        
        A = (A * factorA) % factorC;
        B = (B * factorB) % factorC;
        
        while (A % 4) {
            A = (A * factorA) % factorC;
        }
        
        while (B % 8) {
            B = (B * factorB) % factorC;
        }

        count += bincmp(A, B);
    }
    return count;
}

int main(void)
{
    long A = 883, B = 879;
    //printf("Puzzle 1: %ld\n", puzzle1(A, B));
    printf("Puzzle 2: %ld\n", puzzle2(A, B));
    return EXIT_SUCCESS;
}
