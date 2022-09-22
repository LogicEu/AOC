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

#define STRLEN 1024

static size_t puzzle(const char* str)
{
    const size_t size = strlen(str);
    size_t count = 0;
    for (size_t i = 0; i < size; ++i) {
        count += ((str[i]) == (str[(i + 1) % size])) * (str[i] - '0');
    }
    return count;
}

static size_t puzzle2(const char* str)
{
    const size_t size = strlen(str);
    const size_t hsize = size / 2;
    size_t count = 0;
    for (size_t i = 0; i < size; ++i) {
        count += ((str[i]) == (str[(i + hsize) % size])) * (str[i] - '0');
    }
    return count;
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

    printf("Puzzle 1: %ld\n", puzzle(buf));
    printf("Puzzle 2: %ld\n", puzzle2(buf)); 

    free(buf);
    return EXIT_SUCCESS;
}
