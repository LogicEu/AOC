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

long puzzle(char** grid, int width, int height, int xslope, int yslope)
{ 
    long y, x = xslope, count = 0;
    for (y = yslope; y < height; y += yslope) {
        count += (grid[y][x] == '#');
        x = (x + xslope) % (width);
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

#define W 32
#define H 323
    
    long j = 0, linecount = 0, width;
    char** grid = malloc(sizeof(char*) * H);
    grid[0] = malloc(W);

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            grid[linecount++][j] = 0;
            grid[linecount] = malloc(W);
            width = j;
            j = 0;
        }
        else grid[linecount][j++] = buf[i];

        if (j > W || linecount > H) {
            printf("Error: string buffer is too small!\n");
            return EXIT_FAILURE;
        }       
    }

    if (j) {
        grid[linecount++][j] = 0;
    }

    long s1, s2, s3, s4, s5;
    s1 = puzzle(grid, width, linecount, 3, 1);
    printf("Puzzle 1: %ld\n", s1);

    s2 = puzzle(grid, width, linecount, 1, 1);
    s3 = puzzle(grid, width, linecount, 5, 1);
    s4 = puzzle(grid, width, linecount, 7, 1);
    s5 = puzzle(grid, width, linecount, 1, 2);

    printf("Puzzle 2: %ld\n", s1 * s2 * s3 * s4 * s5);

    for (long i = 0; i < linecount; ++i) {
        free(grid[i]);
    }
    free(grid);
    free(buf);
    return EXIT_SUCCESS;
}
