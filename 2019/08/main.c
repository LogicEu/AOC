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

#define WIDTH 25
#define HEIGHT 6
#define LAYCOUNT 100

    const int width = WIDTH, height = HEIGHT;
    const int wxh = width * height;
    int layers[LAYCOUNT][HEIGHT][WIDTH], i, x, y, index = 0;
    for (i = 0; buf[index]; ++i) {
        const int ii = i * wxh;
        for (y = 0; y < height; ++y) {
            const int yy = y * width;
            for (x = 0; x < width; ++x) {
                layers[i][y][x] = buf[ii + yy + x] - '0';
            }
        }
        index += wxh;
    }
    free(buf);

    const int laycount = i;
    int min = 999999, zeros, minindex;
    for (i = 0; i < laycount; ++i) {
        zeros = 0;
        for (y = 0; y < height; ++y) {
            for (x = 0; x < width; ++x) {
                zeros += layers[i][y][x] == 0;
            }
        }
        if (zeros < min) {
            min = zeros;
            minindex = i;
        }
    }
    
    int ones = 0, twos = 0;
    for (y = 0; y < height; ++y) {
        for (x = 0; x < width; ++x) {
            ones += layers[minindex][y][x] == 1;
            twos += layers[minindex][y][x] == 2;
        }
    }

    printf("Puzzle 1: %d\nPuzzle 2:\n", ones * twos);

    for (y = 0; y < height; ++y) {
        for (x = 0; x < width; ++x) {
            for (i = 0; layers[i][y][x] == 2; ++i);
            printf("%c", layers[i][y][x] == 0 ? ' ' : '#');
        }
        printf("\n");
    }

    return EXIT_SUCCESS;
}
