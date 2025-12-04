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

#define LINECOUNT 1024
#define ISSYM(c) (c != '.' && !isdigit(c))

static char grid[256][256] = {0};
static long gridvals[256][256] = {0};

static int mark(const char c, const long x, const long y, const long n)
{
    if (ISSYM(c)) {
        if (c == '*') {
            ++grid[y][x];
            gridvals[y][x] = gridvals[y][x] == 0 ? n : gridvals[y][x] * n;
        }
        return 1;
    }
    return 0;
}

static long search(
    const char* str, const long val, const long y, const long start, const long end)
{
    long count = 0, i;
    for (i = start; i <= end; ++i) {
        count += mark(str[i], i, y, val);
    }
    return count;
}

static long find(
    char** lines, const long w, const long h, 
    const long x, const long y, const long n, const long val)
{
    long count = 0;
    const float startx = x > 0 ? x - 1 : 0;
    const float endx = x + n < w ? x + n : w - 1; 
    
    count += x > 0 && ISSYM(lines[y][x - 1]);
    count += x + n < w && ISSYM(lines[y][x + n]);

    if (x > 0) {
        count += mark(lines[y][x - 1], x - 1, y, val);
    }

    if (x + n < w) {
        count += mark(lines[y][x + n], x + n, y, val);
    }

    if (y > 0) {
        count += search(lines[y - 1], val, y - 1, startx, endx);
    }

    if (y + 1 < h) {
        count += search(lines[y + 1], val, y + 1, startx, endx);
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

    char* lines[LINECOUNT];
    long linecount = 0, linelen;
    lines[linecount++] = buf;
    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            buf[i] = 0;
            lines[linecount++] = buf + i + 1;
        }

        if (linecount >= LINECOUNT) {
            fprintf(stderr, "Line count over %d\n", LINECOUNT);
            return EXIT_FAILURE;
        }
    }

    linelen = strlen(lines[0]);
    printf("Width: %ld\nHeight: %ld\n", linelen, linecount);

    static const long difs[8][2] = {
        {-1, -1}, {0, -1}, {1, -1}, {1, 0}, 
        {1, 1}, {0, 1}, {-1, 1}, {-1, 0}
    };

    long count = 0;
    for (long y = 0; y < linecount; ++y) {
        for (long x = 0; x < linelen; ++x) {
            if (isdigit(lines[y][x])) {
                long n = 1, k = atol(lines[y] + x);
                while (isdigit(lines[y][x + n])) {
                    ++n;
                }

                if (find(lines, linelen, linecount, x, y, n, k)) {
                    count += k;
                }
                x += n - 1;
            }
        }
    }
    
    printf("Puzzle 1: %ld\n", count);

    count = 0;
    for (long y = 0; y < linecount; ++y) {
        for (long x = 0; x < linelen; ++x) {
            if (grid[y][x] == 2) {
                count += gridvals[y][x];
            }
        }
    }
    
    printf("Puzzle 2: %ld\n", count);

    free(buf);
    return EXIT_SUCCESS;
}
