#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static int count_combinations(const int* array)
{
    int i, j, r;
    unsigned char* bytes = (unsigned char*)&r;
    
    int bools[24] = {0};
    int count = 0;

    for (r = 0; !((bytes[2] >> 4) & 1); ++r) {
        for (i = 2; i >= 0; --i) {
            for (j = 7; j >= 0; --j) {
                bools[i * 8 + j] = (int)((bytes[i] >> j) & 1);
            }
        }  

        int acc = 0;
        for (i = 0; i < NUM_COUNT; ++i) {
            acc += array[i] * bools[i];
        }

        if (acc == 150) {
            ++count;
        }
    }

    return count;
}

static int count_combinations_min(const int* array)
{
    int i, j, r;
    unsigned char* bytes = (unsigned char*)&r;

    int min = 99999999;
    
    int bools[24] = {0};
    int count = 0;

    for (r = 0; !((bytes[2] >> 4) & 1); ++r) {
        int bits = 0;
        for (i = 2; i >= 0; --i) {
            for (j = 7; j >= 0; --j) {
                int n = (bytes[i] >> j) & 1;
                bits += n;
                bools[i * 8 + j] = n;
            }
        }  

        if (bits > min) {
            continue;
        }

        int acc = 0;
        for (i = 0; i < NUM_COUNT; ++i) {
            acc += array[i] * bools[i];
        }

        if (acc == 150) {
            if (bits < min) {
                count = 1;
                min = bits;
            }
            else ++count;
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
    char str[16];
    long j = 0, linecount = 0;
    
    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            arr[linecount++] = atoi(str);
        }
        else str[j++] = buf[i];
    }

    str[j] = 0;
    arr[linecount++] = atoi(str);

    long count = count_combinations(arr);
    printf("Puzzle 1: %ld\n", count);

    count = count_combinations_min(arr);
    printf("Puzzle 2: %ld\n", count);

    free(buf);
    return EXIT_SUCCESS;
}
