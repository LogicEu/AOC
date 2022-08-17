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

static inline int triangle(const int a, const int b, const int c)
{
    return (a < b + c) && (b < a + c) && (c < a + b);
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

    int t[3][3];
    int a, b, c;
    int count = 0, count2 = 0;

    long j = 0, linecount = 0;
    char str[128];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;

            sscanf(str, "%d %d %d", &a, &b, &c);
            count += triangle(a, b, c);

            const int n = linecount % 3;
            t[0][n] = a;
            t[1][n] = b;
            t[2][n] = c;

            if (n == 2) {
                for (int k = 0; k < 3; ++k) {
                    count2 += triangle(t[k][0], t[k][1], t[k][2]);
                }
            }

            ++linecount;
        }
        else str[j++] = buf[i];
    }

    str[j] = 0;
    sscanf(str, "%d %d %d", &a, &b, &c);
    count += triangle(a, b, c);
 
    const int n = linecount % 3;
    t[0][n] = a;
    t[1][n] = b;
    t[2][n] = c;   
    
    for (int i = 0; i < 3; ++i) {
        count2 += triangle(t[i][0], t[i][1], t[i][2]);
    }

    printf("Puzzle 1: %d\n", count);
    printf("Puzzle 2: %d\n", count2);

    free(buf);
    return EXIT_SUCCESS;
}
