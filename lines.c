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

    printf("File: '%s' (%zu lines)\n", argv[1], file.length);
    for (size_t i = 0; i < file.length; ++i) {
        printf("'%s'\n", file.lines[i]);
    }

    vector_free(&file);
    return EXIT_SUCCESS;
}
