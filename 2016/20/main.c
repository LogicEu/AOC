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

typedef struct ivec2 {
    unsigned int x;
    unsigned int y;
} ivec2;

static inline ivec2 catch(const char* str)
{
    ivec2 v;
    sscanf(str, "%u-%u", &v.x, &v.y);
    return v;
}

static inline void puzzle1(const ivec2* ranges, const long count)
{
    unsigned int min = 999999999, mark;
    for (long i = 0; i < count; ++i) {
        if (ranges[i].x < min) {
            min = ranges[i].x;
            mark = ranges[i].y + 1;
        }
    }
    
    int found = 1;
    while (found) {
        found = 0;
        for (long i = 0; i < count; ++i) {
            if (ranges[i].x <= mark && ranges[i].y > mark) {
                mark = ranges[i].y + 1;
                ++found;
                break;
            }
        }

        if (!found) {
            printf("Puzzle 1: %u\n", mark);
        }
    }
}

static unsigned int nearest(const ivec2* ranges, const long count, const unsigned index)
{
    unsigned int min = 99999999, mark;
    for (long i = 0; i < count; ++i) {
        if (ranges[i].x > i) {
            unsigned int dif = ranges[i].x - index;
            if (dif < min) {
                min = dif;
                mark = ranges[i].x;
            }
        }
    }
    return mark;
}

static inline void puzzle2(const ivec2* ranges, const long count)
{
    unsigned int min = 99999999, mark;
    for (long i = 0; i < count; ++i) {
        if (ranges[i].x < min) {
            min = ranges[i].x;
            mark = ranges[i].y + 1;
        }
    }
    
    long acc = (long)min;
    while (mark && mark < UINT32_MAX) {
        int found = 0;
        for (long i = 0; i < count; ++i) {
            if (ranges[i].x <= mark && ranges[i].y > mark) {
                mark = ranges[i].y + 1;
                ++found;
                break;
            }
        }

        if (!found) {
            unsigned int n = nearest(ranges, count, mark);
            acc += n - mark;
            mark = n;
        }
    }
    printf("Puzzle 2: %ld\n", acc);
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

    ivec2 ranges[1024];

    long j = 0, linecount = 0;
    char str[STRLEN];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            ranges[linecount++] = catch(str);
        }
        else str[j++] = buf[i];

        if (j > STRLEN) {
            printf("Error: string buffer is too small!\n");
            return EXIT_FAILURE;
        }       
    }

    if (j) {
        str[j] = 0;
        ranges[linecount++] = catch(str);
    }

    puzzle1(ranges, linecount);
    puzzle2(ranges, linecount);

    free(buf);
    return EXIT_SUCCESS;
}
