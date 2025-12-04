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
#define LEN 1000
#define ARRLEN (LEN * LEN)
#define COUNT 256

static long arrcount(const long* arr)
{
    long i, count = 0;
    for (i = 0; i < ARRLEN; ++i) {
        count += arr[i] > 1;
    }
    return count;
}

static void puzzle(const char* str, long* arr)
{
    long id, x, y, px, py, w, h;
    sscanf(str, "#%ld @ %ld,%ld: %ldx%ld", &id, &px, &py, &w, &h);
    const long width = w + px, height = h + py;
    for (y = py; y < height; ++y) {
        for (x = px; x < width; ++x) {
            ++arr[y * LEN + x];
        }
    }
}

static long arrcheck(const char strs[COUNT][STRLEN], const long* arr, const
        long strcount)
{
    long id, x, y, px, py, w, h, i, count;
    for (i = 0; i < strcount; ++i) {
        sscanf(strs[i], "#%ld @ %ld,%ld: %ldx%ld", &id, &px, &py, &w, &h);
        count = 0;
        const long width = w + px, height = h + py;
        for (y = py; y < height; ++y) {
            for (x = px; x < width; ++x) {
                count += arr[y * LEN + x] > 1;
            }
        }
        if (!count) {
            return id;
        }
    }
    return -1;
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

    long* arr = calloc(sizeof(long), ARRLEN), j = 0, strcount = 0;
    char strs[0xfff][STRLEN];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            strs[strcount][j] = 0;
            j = 0;
            puzzle(strs[strcount++], arr);
        }
        else strs[strcount][j++] = buf[i];
    }

    if (j) {
        strs[strcount][j] = 0;
        puzzle(strs[strcount++], arr);
    }

    printf("Puzzle 1: %ld\n", arrcount(arr));
    printf("Puzzle 2: %ld\n", arrcheck(strs, arr, strcount));

    free(arr);
    free(buf);
    return EXIT_SUCCESS;
}
