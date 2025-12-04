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
static long puzzle(const char* str)
{
    return atol(str) / 3 - 2;
}

static long puzzle2(long m)
{
    for (long f = m / 3 - 2; f > 0; f = f / 3 - 2) {
        m += f;
    }
    return m;
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

    long j = 0, count = 0, count2 = 0, n;
    char str[STRLEN];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            n = puzzle(str);
            count += n;
            n = puzzle2(n);
            count2 += n;
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
        count += n;
        n = puzzle2(n);
        count2 += n;
    }

    printf("Puzzle 1: %ld\n", count);
    printf("Puzzle 2: %ld\n", count2);

    free(buf);
    return EXIT_SUCCESS;
}
