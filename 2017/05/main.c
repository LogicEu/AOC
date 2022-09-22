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

    long j = 0, linecount = 0;
    char str[STRLEN];

    int arr[0xfff] = {0}, arr2[0xfff];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            arr[linecount] = atoi(str);
            arr2[linecount++] = atoi(str);
        }
        else str[j++] = buf[i];

        if (j > STRLEN) {
            printf("Error: string buffer is too small!\n");
            return EXIT_FAILURE;
        }       
    }

    if (j) {
        str[j] = 0;
        arr[linecount] = atoi(str);
        arr2[linecount++] = atoi(str);
    }

    int k, steps = 0;
    for (int i = 0; i >= 0 && i < linecount; i += k, ++steps) {
        k = arr[i]++;
    } 
    printf("Puzzle 1: %d\n", steps);

    steps = 0;
    for (int i = 0; i >= 0 && i < linecount; i += k, ++steps) {
        k = arr2[i];
        arr2[i] += (arr2[i] >= 3) ? -1 : 1;
    } 
    printf("Puzzle 2: %d\n", steps);

    free(buf);
    return EXIT_SUCCESS;
}
