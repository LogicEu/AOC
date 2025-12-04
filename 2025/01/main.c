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

static long calc(long* num, const char* str)
{
    long sign = str[0] == 'R' ? 1 : -1;
    long n = atol(str + 1), count = 0;
    while (n--) {
        *num += sign;
        if (*num < 0) {
            *num = 99;
        } else if (*num > 99) {
            *num = 0;
        }
        count += !*num;
    }

    //printf("%ld\n", *num);
    return count;
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

    long n, num = 50, count = 0, count2 = 0;
    long j = 0, linecount = 0;
    char str[STRLEN];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            count2 += calc(&num, str);
            count += !num;
        }
        else str[j++] = buf[i];

        if (j > STRLEN) {
            printf("Error: string buffer is too small!\n");
            return EXIT_FAILURE;
        }       
    }

    if (j) {
        str[j] = 0;
        count2 += calc(&num, str);
        count += !num;
    }

    printf("puzzle 1: %ld\n", count);
    printf("puzzle 2: %ld\n", count2);

    free(buf);
    return EXIT_SUCCESS;
}
