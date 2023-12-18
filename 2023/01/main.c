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

static char findn(const char* str)
{
    static const char* numbers[9] = {
        "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
    };

    long i, j, found;
    for (i = 0; i < 9; ++i) {
        for (j = 0, found = 1; numbers[i][j]; ++j) {
            if (numbers[i][j] != str[j]) {
                found = 0;
                break;
            }
        }
        
        if (found) {
            return i + 1;
        }
    }

    return 0;
}

static void calc(char* s, const char c)
{
    if (!s[0]) {
        s[0] = c;
    }
    s[1] = c;
}

static long puzzle(const char* str)
{
    char s[4] = {0};
    while (*str) {
        if (isdigit(*str)) {
            calc(s, *str);
        }
        ++str;
    }
    return atol(s);
}

static long puzzle2(const char* str)
{
    char s[4] = {0}, c;
    while (*str) {
        if (isdigit(*str)) {
            calc(s, *str);
        } else if ((c = findn(str))) {
            calc(s, c + '0');
        }
        ++str;
    }
    return atol(s);
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

    char str[STRLEN];
    long n, m, j = 0, linecount = 0, r = 0, r2 = 0;
    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            ++linecount;
            n = puzzle(str);
            m = puzzle2(str);
            r += n;
            r2 += m;
        }
        else str[j++] = buf[i];

        if (j > STRLEN) {
            printf("Error: string buffer is too small!\n");
            return EXIT_FAILURE;
        }       
    }

    if (j) {
        str[j] = 0;
        n = puzzle(str);
        m = puzzle2(str);
        r += n;
        r2 += m;
    }

    printf("Puzzle 1: %ld\n", r);
    printf("Puzzle 2: %ld\n", r2);

    free(buf);
    return EXIT_SUCCESS;
}

