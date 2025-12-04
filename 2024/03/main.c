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

#define STRLEN 0xFFF

static long puzzle1(const char* buf)
{
    long total = 0;
    for (long i = 0; buf[i]; ++i) {
        if (memcmp(buf + i, "mul(", 4) == 0) {
            long n, m;
            int r = sscanf(buf + i + 4, "%ld,%ld", &n, &m);
            if (r == 2) {
                i += 5;
                while ((buf[i] >= '0' && buf[i] <= '9') || buf[i] == ',') {
                    ++i;
                }

                if (buf[i] == ')') {
                    long x = n * m;
                    total += x;
                }
            }
        }
    }
    return total;
}

static long puzzle2(const char* buf)
{
    long total = 0, enabled = 1;
    for (long i = 0; buf[i]; ++i) {

        if (memcmp(buf + i, "do()", 4) == 0) {
            enabled = 1;
            continue;
        }

        if (memcmp(buf + i, "don't()", 7) == 0) {
            enabled = 0;
            continue;
        }

        if (enabled && memcmp(buf + i, "mul(", 4) == 0) {
            long n, m;
            int r = sscanf(buf + i + 4, "%ld,%ld", &n, &m);
            if (r == 2) {
                i += 5;
                while ((buf[i] >= '0' && buf[i] <= '9') || buf[i] == ',') {
                    ++i;
                }

                if (buf[i] == ')') {
                    long x = n * m;
                    total += x;
                }
            }
        }
    }
    return total;
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

    printf("puzzle 1: %ld\n", puzzle1(buf));
    printf("puzzle 2: %ld\n", puzzle2(buf));

    free(buf);
    return EXIT_SUCCESS;
}
