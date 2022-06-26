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

    long floor = 0, basement = 0;
    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '(') {
            ++floor;
        }
        else if (buf[i] == ')') {
            --floor;
        }

        if (!basement && floor < 0) {
            printf("PUZZLE 2: First basement visit at position: %ld\n", i + 1);
            ++basement;
        }
    }

    printf("PUZZLE 1: Floor: %ld\n", floor);

    // End puzzles

    free(buf);
    return EXIT_FAILURE;
}
