#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static int dmax = 0;

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

static inline void vop(const char c, int* v)
{
    if (c == 'n') {
        ++(*v);
    }
    else if (c == 's') {
        --(*v);
    }
}

static inline void op(const char* str, int* h, int* v)
{
    vop(str[0], v);
    if (str[1] == 'e') {
        ++(*h);
    }
    else if (str[1] == 'w') {
        --(*h);
    }
    else vop(str[0], v);
}


static inline int dist(const int h, const int v)
{
    int ah = abs(h), av = abs(v), i;
    for (i = 0; ah; ++i) { 
        if (av) {
            --av;
        }
        --ah;
    }

    for (; av > 0; ++i) {
        av -= 2;
    }

    if (i > dmax) {
        dmax = i;
    }

    return i;
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

    int h = 0, v = 0, d;
    char* s = strtok(buf, ",");
    if (s) {
        op(s, &h, &v);
        d = dist(h, v);
        while ((s = strtok(NULL, ","))) {
            op(s, &h, &v);
            d = dist(h, v);
        }
    }

    printf("Puzzle 1: %d\n", d);
    printf("Puzzle 2: %d\n", dmax);

    free(buf);
    return EXIT_SUCCESS;
}
