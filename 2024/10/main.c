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

#define SIZE 40

struct ivec2 {
    int x, y;
};

const struct ivec2 dirs[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

static long trailcount(const int grid[SIZE][SIZE], const int size,
    const struct ivec2 pos, struct ivec2* visited, size_t* vcount)
{
    long val = 0, b = 0;

    for (int i = 0; i < 4; ++i) {
        const struct ivec2 p = {pos.x + dirs[i].x, pos.y + dirs[i].y};
        if (p.x >= 0 && p.x < size && p.y >= 0 && p.y < size && 
            grid[p.y][p.x] == grid[pos.y][pos.x] + 1) {
            int found = 0;
            for (size_t j = 0; j < *vcount; ++j) {
                if (p.x == visited[j].x && p.y == visited[j].y) {
                    ++found;
                    break;
                }
            }

            if (found) {
                continue;
            }

            visited[vcount[0]++] = p;
            if (grid[p.y][p.x] == 9) {
                ++val;
            } else {
                val += trailcount(grid, size, p, visited, vcount);
            }
        }
    }

    return val;
}

static long trailcount2(const int grid[SIZE][SIZE], const int size, 
    const struct ivec2 pos)
{
    long val = 0, b = 0;

    for (int i = 0; i < 4; ++i) {
        const struct ivec2 p = {pos.x + dirs[i].x, pos.y + dirs[i].y};
        if (p.x >= 0 && p.x < size && p.y >= 0 && p.y < size && 
            grid[p.y][p.x] == grid[pos.y][pos.x] + 1) {

            if (grid[p.y][p.x] == 9) {
                ++val;
            } else {
                val += trailcount2(grid, size, p);
            }
        }
    }

    return val;
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

    int grid[SIZE][SIZE];
    struct ivec2 visited[SIZE * SIZE] = {0};
    long xsize = 0, ysize = 0, sum = 0, sum2 = 0;

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            ++ysize;
            xsize = 0;
        }
        else grid[ysize][xsize++] = buf[i] - '0';

        if (xsize > SIZE || ysize > SIZE) {
            printf("Error: string buffer is too small!\n");
            return EXIT_FAILURE;
        }       
    }

    if (xsize) {
        ++ysize;
    }

    for (long y = 0; y < ysize; ++y) {
        for (long x = 0; x < xsize; ++x) {
            if (grid[y][x] == 0) {
                struct ivec2 pos = {x, y};
                size_t vcount = 0;
                long n = trailcount(grid, ysize, pos, visited, &vcount);
                long m = trailcount2(grid, ysize, pos);
                sum += n;
                sum2 += m;
            }
        }
    }

    printf("puzzle 1: %ld\n", sum);
    printf("puzzle 2: %ld\n", sum2);

    free(buf);
    return EXIT_SUCCESS;
}
