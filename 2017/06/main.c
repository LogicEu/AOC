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

#define COUNT 16
#define STRLEN 128

static int highest(const uint8_t* arr)
{
    uint8_t u = 0;
    int n;
    for (int i = 0; i < COUNT; ++i) {
        if (arr[i] > u) {
            u = arr[i];
            n = i;
        }
    }
    return n;
}

static void share(uint8_t* arr, const int n)
{
    const int len = (int)arr[n];
    arr[n] = 0;
    for (int i = 0; i < len; ++i) {
        ++arr[(i + n + 1) % COUNT];
    }
}

static int puzzle(const uint8_t* input)
{
    uint8_t arr[0xfff][COUNT];
    int i, found = 0;
    memcpy(arr[0], input, COUNT);

    for (i = 1; !found; ++i) {
        memcpy(arr[i], arr[i - 1], COUNT);
        int high = highest(arr[i]);
        share(arr[i], high);

        for (int j = 0; j < i; ++j) {
            if (!memcmp(arr[i], arr[j], COUNT)) {
                ++found;
                break;
            }
        }
    }
    return i - 1;
}

static int puzzle2(const uint8_t* input)
{
    uint8_t arr[0xfff][COUNT];
    int i, j, found = 0;
    memcpy(arr[0], input, COUNT);

    for (i = 1; !found; ++i) {
        memcpy(arr[i], arr[i - 1], COUNT);
        int high = highest(arr[i]);
        share(arr[i], high);

        for (j = 0; j < i; ++j) {
            if (!memcmp(arr[i], arr[j], COUNT)) {
                ++found;
                break;
            }
        }
    }

    return i - j - 1;
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

    uint8_t arr[COUNT] = {0}, count = 0;
    char* s = strtok(buf, " \t\n");
    if (s) {
        arr[count++] = (uint8_t)atoi(s);
        while ((s = strtok(NULL, " \t\n"))) {
            arr[count++] = (uint8_t)atoi(s);
        }
    }

    printf("Puzzle 1: %d\n", puzzle(arr));
    printf("Puzzle 2: %d\n", puzzle2(arr));

    free(buf);
    return EXIT_SUCCESS;
}
