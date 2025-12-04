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

#define STRLEN 128
#define ARRLEN 0xfffff

static long sum(const long* arr, const long len)
{
    long i, count = 0;
    for (i = 0; i < len; ++i) {
        count += arr[i];
    }
    return count;
}

static long freq(const long* arr, const long len)
{
    long* freqs = malloc(sizeof(long) * ARRLEN);
    long i, j, freqcount = 0, count = 0;
    freqs[freqcount++] = count;
    while (1) {
        for (i = 0; i < len; ++i)  {
            count += arr[i];
            for (j = 0; j < freqcount; ++j) {
                if (count == freqs[j]) {
                    free(freqs);
                    return count;
                }
            }
            freqs[freqcount++] = count;
        }
    }
    free(freqs);
    return 0;
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

    long n, i, k, j = 0, found = 0, count = 0, arr[0xffff], arrcount;
    char str[STRLEN];

    for (i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            arr[arrcount++] = atoi(str);
        }
        else str[j++] = buf[i];
    }

    if (j) {
        str[j] = 0;
        n = atoi(str);
        arr[arrcount++] = atoi(str);
    }

    printf("Puzzle 1: %ld\n", sum(arr, arrcount));
    printf("Puzzle 2: %ld\n", freq(arr, arrcount));

    free(buf);
    return EXIT_SUCCESS;
}
