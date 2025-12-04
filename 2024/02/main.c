#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

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

struct vec2 {
    float x, y;
};

static long check(const long* arr, const long len)
{
    long safe = 1;
    if (arr[0] < arr[1]) {
        for (long i = 0; i < len - 1; ++i) {
            long d = arr[i + 1] - arr[i];
            if (arr[i] > arr[i + 1] || d < 1 || d > 3) {
                safe = 0;
                break;
            }
        }   
    } else {
        for (long i = 0; i < len - 1; ++i) {
            long d = arr[i] - arr[i + 1];
            if (arr[i] < arr[i + 1] || d < 1 || d > 3) {
                safe = 0;
                break;
            }
        }   
    }
    return safe;
}

static struct vec2 puzzle(char* str, const long line)
{
    struct vec2 r;
    long arr[8], len = 0, safe1, safe2 = 0;
    char* tok = strtok(str, " ");
    
    while (tok) {
        arr[len++] = (long)atoi(tok);
        tok = strtok(NULL, " ");
    }

    safe1 = check(arr, len);
    if (!safe1) {
        long arr2[8];
        for (long i = 0; i < len; ++i) {
            for (long j = 0, n = 0; j < len - 1; ++j) {
                n += i == j;
                arr2[j] = arr[j + n];
            }

            if (check(arr2, len - 1)) {
                safe2 = 1;
                break;
            }
        }
    } else {
        safe2 = 1;
    }

    r.x = safe1;
    r.y = safe2;
    return r;
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

    long j = 0, linecount = 0, safe1 = 0, safe2 = 0;
    struct vec2 ret;
    char str[STRLEN];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            ret = puzzle(str, linecount++);
            safe1 += ret.x;
            safe2 += ret.y;
        }
        else str[j++] = buf[i];

        if (j > STRLEN) {
            printf("Error: string buffer is too small!\n");
            return EXIT_FAILURE;
        }       
    }

    if (j) {
        str[j] = 0;
        ret = puzzle(str, linecount++);
        safe1 += ret.x;
        safe2 += ret.y;
    }

    printf("puzzle 1: %ld\n", safe1);
    printf("puzzle 2: %ld\n", safe2);

    free(buf);
    return EXIT_SUCCESS;
}
