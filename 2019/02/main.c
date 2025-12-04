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

#define ARRLEN 0xff

static int func(const int* array, const int count, const int noun, const int verb)
{
    int arr[ARRLEN], i;
    memcpy(arr, array, count * sizeof(int));
    arr[1] = noun;
    arr[2] = verb;
    for (i = 0; i < count; i += 4) {
        if (arr[i] == 99) {
            break;
        } else if (arr[i] == 1) {
            arr[arr[i + 3]] = arr[arr[i + 1]] + arr[arr[i + 2]];
        } else if (arr[i] == 2) {
            arr[arr[i + 3]] = arr[arr[i + 1]] * arr[arr[i + 2]];
        }
    }
    return arr[0];
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

    /* puzzle */
    int arr[ARRLEN], count = 0, j = 0, i, found = 0;
    for (i = 0; buf[i]; ++i) {
        if (buf[i] == ',') {
            buf[i] = 0;
            arr[count++] = atoi(buf + j);
            j = i + 1;
        }
    }
    arr[count++] = atoi(buf + j);
    free(buf);

    printf("Puzzle 1: %d\n", func(arr, count, 12, 2));

    for (i = 0; !found && i < 100; ++i) {
        for (j = 0; j < 100; ++j) {
            if (func(arr, count, i, j) == 19690720) {
                ++found;
                break;
            }
        }
    }
    printf("Puzzle 2: %d\n", 100 * (i - 1) + j);
    return EXIT_SUCCESS;
}
