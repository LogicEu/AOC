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

typedef struct ivec2 {
    long x, y;
} ivec2;

static ivec2 res = {0};
static const ivec2 dirs[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

static void mark_cell(
    long* cells, const char* buf, ivec2* sum, const ivec2 p, const char c)
{
    long i = p.y * res.x + p.x;
    if (cells[i]) {
        return;
    }

    ++sum->x;
    cells[i] = (long)c;

    for (i = 0; i < 4; ++i) {
        ivec2 pos = {p.x + dirs[i].x, p.y + dirs[i].y};
        if (pos.x < 0 || pos.y < 0 || pos.x >= res.x || pos.y >= res.y ||
            buf[pos.y * (res.x + 1) + pos.x] != c) {
            ++sum->y;
        } else {
            mark_cell(cells, buf, sum, pos, c);
        }
    }
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

    for (long i = 0; buf[i]; ++i) {
        res.y += buf[i] == '\n';
        res.x += !res.y;
    }

    ++res.y;
    printf("x: %ld, y: %ld\n", res.x, res.y);

    const long size = res.x * res.y;
    long* cells = calloc(size, sizeof(long));

    ivec2 p = {0}, sum = {0};
    long counter = 0;

    for (long i = 0; i < size; ++i) {
        if (!cells[i]) {
            p.y = i / res.x;
            p.x = i % res.x;
            long j = p.y * (res.x + 1) + p.x;
            mark_cell(cells, buf, &sum, p, buf[j]);
            counter += sum.x * sum.y;
            sum.x = 0;
            sum.y = 0;
        }
    }

    printf("puzzle 1: %ld\n", counter);

    free(cells);
    free(buf);
    return EXIT_SUCCESS;
}
