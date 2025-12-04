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

#define STRLEN 0xFF

struct ivec2 {
    long x, y;
};

static const struct ivec2 dirs[8] = {
    {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}
};

static long puzzle1(char grid[STRLEN][STRLEN], const long len)
{
    long count = 0;
    for (long y = 0; y < len; ++y) {
        for (long x = 0; x < len; ++x) {
            if (grid[y][x] == 'X') {
                for (long i = 0; i < 8; ++i) {
                    const struct ivec2 d = dirs[i];
                    if (y + d.y * 3 < len && y + d.y * 3 >= 0 &&
                        x + d.x * 3 < len || x + d.x * 3 >= 0) {
                        count += grid[y + d.y * 1][x + d.x * 1] == 'M' &&
                                grid[y + d.y * 2][x + d.x * 2] == 'A' &&
                                grid[y + d.y * 3][x + d.x * 3] == 'S';
                    }
                }
            }
        }
    }

    return count;
}

static long puzzle2(char grid[STRLEN][STRLEN], const long len)
{
    long count = 0;
    for (long y = 0; y < len; ++y) {
        for (long x = 0; x < len; ++x) {
            if (grid[y][x] == 'A' &&
                y - 1 >= 0 && y + 1 < len && x - 1 >= 0 && x + 1 < len) {
                long n = 0;
                for (long i = 1; i < 8; i += 2) {
                    const struct ivec2 d = dirs[i];
                    n += grid[y + d.y][x + d.x] == 'M' && grid[y - d.y][x - d.x] == 'S';
                }

                count += n == 2;
            }
        }
    }

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

    long j = 0, linecount = 0;
    char grid[STRLEN][STRLEN];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            grid[linecount++][j] = 0;
            j = 0;
        }
        else grid[linecount][j++] = buf[i];

        if (j > STRLEN || linecount > STRLEN) {
            printf("Error: string buffer is too small!\n");
            return EXIT_FAILURE;
        }       
    }

    if (j) {
        grid[linecount++][j] = 0;
    }

    long p1 = puzzle1(grid, linecount);
    long p2 = puzzle2(grid, linecount);

    printf("puzzle 1: %ld\n", p1);
    printf("puzzle 2: %ld\n", p2);

    free(buf);
    return EXIT_SUCCESS;
}
