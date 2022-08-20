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

#define WIDTH 50
#define HEIGHT 6

static void shiftrow(char** pixels, const int y) 
{
    char ch = pixels[y][WIDTH - 1];
    for (int x = WIDTH - 1; x >= 1; --x) {
        pixels[y][x] = pixels[y][x - 1];
    }
    pixels[y][0] = ch;
}

static void shiftcolumn(char** pixels, const int x) 
{
    char ch = pixels[HEIGHT - 1][x];
    for (int y = HEIGHT - 1; y >= 1; --y) {
        pixels[y][x] = pixels[y - 1][x];
    }
    pixels[0][x] = ch;
}

static void oparr(char** pixels, const char* str)
{
    int X, Y;
    if (strstr(str, "rect ")) {
        sscanf(str, "rect %dx%d",  &X, &Y);
        printf("rect %dx%d\n", X, Y);
        for (int y = 0; y < Y; ++y) {
            for (int x = 0; x < X; ++x) {
                pixels[y][x] = 1;
            }
        }
    }
    else if (strstr(str, " column ")) {
        sscanf(str, "rotate column x=%d by %d", &X, &Y);
        printf("rotate column x=%d by %d\n", X, Y);
        for (int i = 0; i < Y; ++i) {
            shiftcolumn(pixels, X);
        }
    }
    else if (strstr(str, " row ")) {
        sscanf(str, "rotate row y=%d by %d", &X, &Y);
        printf("rotate row y=%d by %d\n", X, Y);
        for (int i = 0; i < Y; ++i) {
            shiftrow(pixels, X);
        }
    }
}

int main(int argc, char** argv)
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

    // Puzzle

    long j = 0, linecount = 0;
    char str[128];

    char** pixels = malloc(sizeof(char*) * HEIGHT);
    for (int y = 0; y < HEIGHT; ++y) {
        pixels[y] = malloc(WIDTH);
        memset(pixels[y], 0, WIDTH);
    }

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            oparr(pixels, str);
            ++linecount;
        }
        else str[j++] = buf[i];
    }

    if (j) {
        str[j] = 0;
        oparr(pixels, str);
    }

    long count = 0;
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            count += !!pixels[y][x];
        }
    }

    printf("Puzzle 1: %ld\n", count);
    
    printf("Puzzle 2:\n");
    printf("--------------------------------------------------\n");
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            char c = pixels[y][x] ? '#' : ' ';
            printf("%c", c);
        }
        printf("\n");
    }
    printf("--------------------------------------------------\n");

    free(buf);
    return EXIT_SUCCESS;
}
