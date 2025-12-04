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

#define STRLEN 256

struct rgb {
    long data[3];
};

static long max(long a, long b)
{
    return a > b ? a : b;
}

static const char* depict(const char* str, long* rgb)
{
    char* s;
    long n = strtol(str, &s, 10);
    ++s;
    switch (*s) {
        case 0:
            return NULL;
        case 'r':
            rgb[0] = max(rgb[0], n);
            return s + 3;
        case 'g':
            rgb[1] = max(rgb[1], n);
            return s + 5;
        case 'b':
            rgb[2] = max(rgb[2], n);
            return s + 4;
        default:
            printf("fail! unexpected character '%c' - %s\n", *s, s);
            return NULL;
    }
}

static struct rgb puzzle(const char* str)
{
    long n;
    struct rgb rgb = {0};
    char s[STRLEN] = {0}, *d = NULL;
    while (*str != ':') ++str;
    str += 2;

    while ((str = depict(str, rgb.data)) && *str) {
        while (!isdigit(*str) && !isalpha(*str)) {
            ++str;
        }
    }

    return rgb;
}

static int ask(const long* rgb)
{
    return rgb[0] <= 12 && rgb[1] <= 13 && rgb[2] <= 14;
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

    struct rgb rgb = {0};
    long j = 0, linecount = 1, r = 0, r2 = 0, p;
    char str[STRLEN];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            rgb = puzzle(str);
            r += ask(rgb.data) * linecount;
            p = rgb.data[0] * rgb.data[1] * rgb.data[2];
            r2 += p;
            ++linecount;
        }
        else str[j++] = buf[i];

        if (j > STRLEN) {
            printf("Error: string buffer is too small!\n");
            return EXIT_FAILURE;
        }       
    }

    if (j) {
        str[j] = 0;
        rgb = puzzle(str);
        r += ask(rgb.data) * linecount;
        p = rgb.data[0] * rgb.data[1] * rgb.data[2];
        r2 += p;
    }

    printf("Puzzle 1: %ld\n", r);
    printf("Puzzle 2: %ld\n", r2);

    free(buf);
    return EXIT_SUCCESS;
}
