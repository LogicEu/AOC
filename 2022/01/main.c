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

#define STRLEN 128

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

    long arr[256], arrcount = 0;
    long j = 0;
    char str[STRLEN];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            arr[arrcount++] = atoi(str);
        }
        else str[j++] = buf[i];

        if (j > STRLEN) {
            printf("Error: string buffer is too small!\n");
            return EXIT_FAILURE;
        }       
    }

    if (j) {
        str[j] = 0;
        arr[arrcount++] = atoi(str);
    }

    long puzzle, puzzle2;
    for (long i = 0; i < arrcount; ++i) {
        for (long j = i + 1; j < arrcount; ++j) {
            if (arr[i] + arr[j] == 2020) {
                puzzle = arr[i] * arr[j];
                break;
            }
        }
    }

    for (long i = 0; i < arrcount; ++i) {
        for (long j = i + 1; j < arrcount; ++j) {
            for (long k = j + 1; k < arrcount; ++k) {
                if (arr[i] + arr[j] + arr[k] == 2020) {
                    puzzle2 = arr[i] * arr[j] * arr[k];
                    break;
                }
            }
        }
    }

    printf("Puzzle 1: %ld\n", puzzle);
    printf("Puzzle 2: %ld\n", puzzle2);
    
    free(buf);
    return EXIT_SUCCESS;
}
