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

#define NUM_COUNT 20

static long count_combinations(const int* array)
{
    int i, j, r;
    unsigned char* bytes = (unsigned char*)&r;
    
    int bools[24] = {0};
    long count = 0;

    for (r = 0; !((bytes[2] >> 5) & 1); ++r) {
        for (i = 2; i >= 0; --i) {
            for (j = 7; j >= 0; --j) {
                unsigned char bit = (bytes[i] >> j) & 1;
                bools[i * 8 + j] = (int)bit;
            }
        }  

        long acc = 0;
        for (i = 0; i < NUM_COUNT; ++i) {
            acc += array[i] * bools[i];
        }

        if (acc == 150) {
            ++count;

            for (i = 0; i < NUM_COUNT; ++i) {
                printf("%d ", array[i]);
            }
            printf("\n");

            for (i = 0; i < NUM_COUNT; ++i) {
                printf("%d ", bools[i]);
            }
            printf("\n");

            for (i = 0; i < NUM_COUNT; ++i) {
                printf("%d ", bools[i] * array[i]);
            }
            printf("\n----------------------------------------------\n");
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

    // Puzzle
    
    int arr[NUM_COUNT];
    int arrcount = 0;

    char str[16];
    long j = 0, linecount = 0;
    
    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;

            arr[arrcount] = atoi(str);
            printf("%ld.- '%d'\n",  linecount, arr[arrcount]);

            ++arrcount;
            ++linecount;
            
            memset(str, 0, 16);
        }
        else str[j++] = buf[i];
    }

    str[j] = 0;
    j = 0;
    arr[arrcount] = atoi(str);
    printf("%ld.- '%d'\n", linecount, arr[arrcount]);

    ++arrcount;
    ++linecount;

    for (int i = 0; i < arrcount; ++i) {
        printf("%d\n", arr[i]);
    }

    long count = count_combinations(arr);
    printf("Puzzle 1: %ld\n", count);

    free(buf);
    return EXIT_SUCCESS;
}
