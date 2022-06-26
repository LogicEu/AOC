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

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("Missing input puzzle.\n");
        return EXIT_FAILURE;
    }

    char* buf = file_read(argv[1]);
    if (!buf) {
        printf("Could not open file '%s'.\n", argv[1]);
    }

    // Puzzle
    
    long total_paper = 0;
    long total_ribbon = 0;

    long start;

    for (long i = 0; buf[i]; ++i) {
        
        start = i;
        for (; buf[i] && buf[i] != '\n'; ++i) {
           
        }

        const long line_size = i - start;
        if (line_size >= 32) {
            printf("Assumption failure: Line Size is bigger than 32.\n");
            return EXIT_FAILURE;
        }

        char line[32];

        memcpy(line, buf + start, line_size);
        line[line_size] = 0;

        // Puzzle 1

        int a, b, c;
        sscanf(line, "%dx%dx%d", &a, &b, &c);
        
        int A = a * b;
        int B = a * c;
        int C = b * c;

        int min = A;
        if (B < min) {
            min = B;
        }
        if (C < min) {
            min = C;
        }

        int surface = 2 * (A + B + C) + min;
        total_paper += (long)surface;

        // Puzzle 2 
        
        int high = a;
        if (b > high) {
           high = b;
        }
        if (c > high) {
            high = c;
        } 

        int square = a + b + c - high;
        square *= 2;

        int cube = a * b * c;

        int ribbon = square + cube;
        total_ribbon += ribbon;
    }

    printf("PUZZLE 1: %ld\n", total_paper);
    printf("PUZZLE 2: %ld\n", total_ribbon);

    free(buf);
    return EXIT_SUCCESS;
}
