#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static char* file_read(const char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (!file) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* buf = malloc(size + 1);
    fread(buf, size, 1, file);
    buf[size] = 0;
    fclose(file);
    return buf;
}

#define ABCLEN 28
#define STRLEN 128
#define COUNT 0xff

static void puzzle(const char* str, long* twocount, long* threecount)
{
    int arr[ABCLEN] = {0}, i, twos = 0, threes = 0;
    
    for (i = 0; str[i]; ++i) {
        ++arr[str[i] - 'a'];
    }

    for (i = 0; i < ABCLEN; ++i) {
        twos += arr[i] == 2;
        threes += arr[i] == 3;
    }

    *twocount += !!twos;
    *threecount += !!threes;
}

static long memcnt(const char* s1, const char* s2)
{
    long d = 0;
    while (*s1 && *s2) {
        d += (*s1++ != *s2++);
    }
    return d;
}

static char* puzzle2(const char strs[COUNT][STRLEN], const long strcount)
{
    static char ret[0xff];
    long i, j, k, n, d, r = 0;
    for (i = 0; i < strcount; ++i) {
        for (j = i + 1; j < strcount; ++j) {
            for (k = 0, d = 0; strs[i][k] && strs[j][k]; ++k) {
                if (strs[i][k] != strs[j][k]) {
                    n = k;
                    ++d;
                }
            } 
            
            if (d == 1) {
                for (k = 0; strs[i][k]; ++k) {
                    if (k != n) {
                        ret[r++] = strs[i][k];
                    }
                }
                return ret;
            }
        }
    }
    return NULL;
}

int main(const int argc, const char** argv)
{
    if (argc < 2) {
        printf("Missing input puzzle.\n");
        return EXIT_FAILURE;
    }

    char* buf = file_read(argv[1]);
    if (!buf) {
        printf("Could not open file '%s'.\n", argv[1]);
        return EXIT_FAILURE;
    }

    long j = 0, twos = 0, threes = 0, strcount = 0;
    char strs[COUNT][STRLEN];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            strs[strcount][j] = 0;
            j = 0;
            puzzle(strs[strcount++], &twos, &threes);
            if (strcount > COUNT) {
                printf("Error!\n");
                return EXIT_FAILURE;
            }
        }
        else strs[strcount][j++] = buf[i];
    }

    if (j) {
        strs[strcount][j] = 0;
        puzzle(strs[strcount], &twos, &threes);
    }

    printf("Puzzle 1: %ld\n", twos * threes);
    printf("Puzzle 2: %s\n", puzzle2(strs, strcount));

    free(buf);
    return EXIT_SUCCESS;
}
