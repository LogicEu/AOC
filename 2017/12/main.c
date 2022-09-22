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

#define COUNT 2000
#define STRLEN 128

typedef struct line_t {
    char str[STRLEN];
} line_t;

static const char* arrow = " <-> ";

static long rec(line_t* lines, const long index, long n)
{
    static const char* div = " ,";

    char* s = strstr(lines[index].str, arrow);
    if (!s) {
        return n;
    } 

    line_t lbuf[16];
    long lcount = 0;

    ++n;
    s = strtok(s, div);
    while ((s = strtok(NULL, div))) {
        strcpy(lbuf[lcount++].str, s);
    }

    for (long i = 0; i < lcount; ++i) {
        int k = atoi(lbuf[i].str);
        if (k != index) {
            n = rec(lines, k, n);
        }
    }
    
    return n;
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
    line_t lines[COUNT];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            lines[linecount++].str[j] = 0;
            j = 0;
        }
        else lines[linecount].str[j++] = buf[i];
    }

    if (j) {
        lines[linecount++].str[j] = 0;
    }

    printf("Puzzle 1: %ld\n", rec(lines, 0, 0));
    
    long count = 1;
    for (long i = 1; i < linecount; ++i) {
        if (strstr(lines[i].str, arrow)) {
            rec(lines, i, 0);
            ++count;
        }
    }
    printf("Puzzle 2: %ld\n", count);

    free(buf);
    return EXIT_SUCCESS;
}
