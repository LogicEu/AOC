#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* file_read(const char* filename)
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

typedef struct ivec2 {
    int x;
    int y;
} ivec2;

static int grid[1000][1000] = {0};

void grid_init(void)
{
    for (int y = 0; y < 1000; ++y) {
        for (int x = 0; x < 1000; ++x) {
            grid[y][x] = 0;
        }
    }
}

void grid_toggle(const ivec2 start, const ivec2 end)
{
    for (int y = start.y; y <= end.y; ++y) {
        for (int x = start.x; x <= end.x; ++x) {
            if (grid[y][x]) {
                grid[y][x] = 0;
            } else {
                grid[y][x] = 1;
            }
        }
    }
}

void grid_on(const ivec2 start, const ivec2 end)
{
    for (int y = start.y; y <= end.y; ++y) {
        for (int x = start.x; x <= end.x; ++x) {
            grid[y][x] = 1;
        }
    }
}

void grid_off(const ivec2 start, const ivec2 end)
{
    for (int y = start.y; y <= end.y; ++y) {
        for (int x = start.x; x <= end.x; ++x) {
            grid[y][x] = 0;
        }
    }
}

void grid_update(const char* str)
{
    ivec2 start, end;
    
    if (strstr(str, "toggle")) {
        sscanf(str, "toggle %d,%d through %d,%d", &start.x, &start.y, &end.x, &end.y);
        return grid_toggle(start, end);
    }
    
    if (strstr(str, "turn on")) {
        sscanf(str, "turn on %d,%d through %d,%d", &start.x, &start.y, &end.x, &end.y);
        return grid_on(start, end);
    }
    
    if (strstr(str, "turn off")) {
        sscanf(str, "turn off %d,%d through %d,%d", &start.x, &start.y, &end.x, &end.y);
        return grid_off(start, end);
    }
}

void grid_toggle2(const ivec2 start, const ivec2 end)
{
    for (int y = start.y; y <= end.y; ++y) {
        for (int x = start.x; x <= end.x; ++x) {
            grid[y][x] += 2;
        }
    }
}

void grid_on2(const ivec2 start, const ivec2 end)
{
    for (int y = start.y; y <= end.y; ++y) {
        for (int x = start.x; x <= end.x; ++x) {
            ++grid[y][x];
        }
    }
}

void grid_off2(const ivec2 start, const ivec2 end)
{
    for (int y = start.y; y <= end.y; ++y) {
        for (int x = start.x; x <= end.x; ++x) {
            if (grid[y][x]) {
                --grid[y][x];
            }
        }
    }
}

void grid_update2(const char* str)
{
    ivec2 start, end;
    
    if (strstr(str, "toggle")) {
        sscanf(str, "toggle %d,%d through %d,%d", &start.x, &start.y, &end.x, &end.y);
        return grid_toggle2(start, end);
    }
    
    if (strstr(str, "turn on")) {
        sscanf(str, "turn on %d,%d through %d,%d", &start.x, &start.y, &end.x, &end.y);
        return grid_on2(start, end);
    }
    
    if (strstr(str, "turn off")) {
        sscanf(str, "turn off %d,%d through %d,%d", &start.x, &start.y, &end.x, &end.y);
        return grid_off2(start, end);
    }
}

long grid_count(void)
{
    long count = 0;
    for (int y = 0; y < 1000; ++y) {
        for (int x = 0; x < 1000; ++x) {
            count += grid[y][x];
        }
    }
    return count;
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

    // Puzzle 1
    grid_init();

    char str[128];
    long j = 0;

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j++] = 0;
            grid_update(str);
            j = 0;
        }
        else str[j++] = buf[i];
    }

    str[j++] = 0;
    grid_update(str);

    long count = grid_count();
    printf("Light on - Stage 1: %ld\n", count);

    // Puzzle 2
    grid_init();
    j = 0;

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j++] = 0;
            grid_update2(str);
            j = 0;
        }
        else str[j++] = buf[i];
    }

    str[j++] = 0;
    grid_update2(str);

    count = grid_count();
    printf("Light on - Stage 2: %ld\n", count);

    free(buf);
    return EXIT_SUCCESS;
}
