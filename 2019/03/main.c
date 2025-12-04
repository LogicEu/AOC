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

#define isdigit(c) (((c) >= 'A') && ((c) <= 'Z'))
#define ABS(x) (((x) >= 0) ? (x) : -(x))

typedef struct ivec2 { int x, y; } ivec2;

static const ivec2 dirs[4] = {
    {0, 1},
    {-1, 0},
    {0, -1},
    {1, 0}
};

static int chardir(const char c)
{
    switch (c) {
        case 'R': return 0;
        case 'D': return 1;
        case 'L': return 2;
        case 'U': return 3;
    }
    printf("Something went terrible!\n");
    return -1;
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
#define PATHLEN 0xfffff
#define INTERLEN 0xff
    static const char* comma = ",";
    static const char* newline = "\n";
    char* wire1, *wire2, *tok;
    int pathcount = 0, intercount = 0, i, j, n, min, count = 0, steps[INTERLEN];
    ivec2* paths = malloc(sizeof(ivec2) * PATHLEN), inters[INTERLEN];
    wire1 = strtok(buf, newline);
    wire2 = strtok(NULL, newline);
    
    ivec2 p = {0, 0};
    tok = strtok(wire1, comma);
    while (tok) {
        printf("Wire 1: %s\n", tok);
        const ivec2 dir = dirs[chardir(*tok)];
        n = atoi(tok + 1);
        for (i = 0; i < n; ++i) {
            p.x += dir.x;
            p.y += dir.y;
            int found = 0;
            paths[pathcount++] = p;
        }
        printf("%d, %d, %d\n", n, i, pathcount);
        tok = strtok(NULL, comma);
    }

    p.x = 0, p.y = 0;
    tok = strtok(wire2, comma);
    while (tok) {
        printf("Wire 2: %s\n", tok);
        const ivec2 dir = dirs[chardir(*tok)];
        n = atoi(tok + 1);
        for (i = 0; i < n; ++i) {
            p.x += dir.x;
            p.y += dir.y;
            for (j = 0; j < pathcount; ++j) {
                if (p.x == paths[j].x && p.y == paths[j].y) {
                    inters[intercount] = p;
                    steps[intercount++] = count + i + j + 2;
                    break;
                }
            }
        }
        count += n;
        tok = strtok(NULL, comma);
    }

    min = 0xffff;
    for (i = 0; i < intercount; ++i) {
        int dist = ABS(inters[i].x) + ABS(inters[i].y);
        if (dist < min) {
            min = dist;
        }
    }
    printf("Puzzle 1: %d\n", min);

    min = 0xffff;
    for (i = 0; i < intercount; ++i) {
        if (steps[i] < min) {
            min = steps[i];
        }
    }
    printf("Puzzle 2: %d\n", min);

    free(buf);
    return EXIT_SUCCESS;
}
