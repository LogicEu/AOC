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

static size_t mycount(char** lines, size_t x, size_t y, size_t width, size_t height)
{
    struct ivec2 {
        size_t x, y;
    } dirs[8] = {
        {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}
    };

    size_t count = 0;
    for (size_t i = 0; i < 8; ++i) {
        struct ivec2 p = {x + dirs[i].x, y + dirs[i].y};
        if (p.x >= 0 && p.x < width && p.y >= 0 && p.y < height) {
            count += lines[p.y][p.x] == '@';
        }
    }
    return count;
}

static size_t puzzle(const struct vector file)
{
    size_t count = 0;
    const size_t len = strlen(file.lines[0]);
    for (size_t y = 0; y < file.length; ++y) {
        for (size_t x = 0; x < len; ++x) {
            if (file.lines[y][x] == '@') {
                size_t n = mycount(file.lines, x, y, len, file.length);
                count += n < 4;
            }
        }
    }

    return count;
}

static size_t step(struct vector* file)
{
    size_t count = 0;
    const size_t len = strlen(file->lines[0]);
    for (size_t y = 0; y < file->length; ++y) {
        for (size_t x = 0; x < len; ++x) {
            if (file->lines[y][x] == '@') {
                size_t n = mycount(file->lines, x, y, len, file->length);
                if (n < 4) {
                    file->lines[y][x] = '.';
                    ++count;
                }
            }
        }
    }

    return count;
}

static size_t puzzle2(struct vector* file)
{
    size_t count = 0;
    size_t val = step(file);
    while (val) {
        count += val;
        val = step(file);
    }
    return count;
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

    printf("puzzle 1: %zu\n", puzzle(file));
    printf("puzzle 2: %zu\n", puzzle2(&file));

    vector_free(&file);
    return EXIT_SUCCESS;
}
