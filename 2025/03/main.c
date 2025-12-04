#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

struct vector {
    char** lines;
    size_t length;
    size_t capacity;
};

static char* vector_push(struct vector* vector, const char* str)
{
    if (vector->length + 1 > vector->capacity) {
        vector->capacity = vector->capacity * 2 + !vector->capacity;
        vector->lines = realloc(vector->lines, vector->capacity * sizeof(char*));
    }

    char* dup = strdup(str);
    char** ptr = vector->lines + vector->length++;
    memcpy(ptr, &dup, sizeof(char*));
    return *ptr;
}

static void vector_free(struct vector* vector)
{
    if (vector->lines) {
        for (size_t i = 0; i < vector->length; ++i) {
            free(vector->lines[i]);
        }

        free(vector->lines);
        memset(vector, 0, sizeof(struct vector));
    }
}

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

static struct vector file_lines(const char* filename)
{
    static const char* newline = "\n";
    struct vector file = {0};
    char* buf = file_read(filename);
    if (buf) {
        char* line = strtok(buf, newline);
        while (line) {
            vector_push(&file, line);
            line = strtok(NULL, newline);
        }

        free(buf);
    }

    return file;
}

static long puzzle1(const char* str)
{
    long max = 0, n;
    char num[0xf] = {0};
    for (long i = 0; str[i]; ++i) {
        num[0] = str[i];
        for (long j = i + 1; str[j]; ++j) {
            num[1] = str[j];
            n = atol(num);
            if (n > max) {
                max = n;
            }
        }
    }

    //printf("%s -> %ld\n", str, max);
    return max;
}

static long puzzle2(const char* str)
{
    char buf[0xFF] = {0};
    const long len = strlen(str);
    long min = 0;
    for (long i = 0; i < 12; ++i) {
        char cmax = 0;
        for (long j = min; j < len - (11 - i); ++j) {
            if (str[j] > cmax) {
                cmax = str[j];
                min = j + 1;
            }
        }
        buf[i] = cmax;
    }

    //printf("%s --> %s\n", str, buf);
    return atol(buf);
}

int main(const int argc, const char** argv)
{
    if (argc < 2) {
        printf("Missing input puzzle.\n");
        return EXIT_FAILURE;
    }

    struct vector file = file_lines(argv[1]);
    if (!file.lines) {
        printf("Could not open file '%s'.\n", argv[1]);
        return EXIT_FAILURE;
    }

    long count = 0, count2 = 0;

    for (size_t i = 0; i < file.length; ++i) {
        count += puzzle1(file.lines[i]);
        count2 += puzzle2(file.lines[i]);
    }

    printf("puzzle 1: %ld\n", count);
    printf("puzzle 2: %ld\n", count2);

    vector_free(&file);
    return EXIT_SUCCESS;
}

