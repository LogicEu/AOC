#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

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

#define STRLEN 64

struct array {
    void* data;
    size_t bytes;
    size_t size;
    size_t cap;
};

static struct array array_create(const size_t bytes)
{
    struct array arr = {NULL, bytes, 0, 0};
    return arr;
}

static void* array_push(struct array* arr, const void* data)
{
    if (arr->size + 1 >= arr->cap) {
        arr->cap = !arr->cap + arr->cap * 2;
        arr->data = realloc(arr->data, arr->cap * arr->bytes);
    }

    void* ptr = (char*)arr->data + arr->size * arr->bytes;
    memcpy(ptr, data, arr->bytes);
    ++arr->size;
    return ptr;
}

static void* array_insert(struct array* arr, const void* data, const size_t index)
{
    if (arr->size + 1 >= arr->cap) {
        arr->cap = !arr->cap + arr->cap * 2;
        arr->data = realloc(arr->data, arr->cap * arr->bytes);
    }

    char* ptr = (char*)arr->data + index * arr->bytes;
    memmove(ptr + arr->bytes, ptr, (arr->size - index) * arr->bytes);
    memcpy(ptr, data, arr->bytes);
    ++arr->size;
    return ptr;
}

static void array_free(struct array* arr)
{
    if (arr->data) {
        free(arr->data);
        arr->size = 0;
        arr->cap = 0;
    }
}

static void* array_at(const struct array* arr, const size_t index)
{
    return (char*)arr->data + index * arr->bytes;
}

static void printarr(const struct array* arr)
{
    for (size_t i = 0; i < arr->size; ++i) {
        printf("%s ", array_at(arr, i));
    }
    printf("\n");
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

    struct array arr = array_create(STRLEN);
    char* tok = strtok(buf, " \n"), s[STRLEN] = {0};
    while (tok) {
        sprintf(s, "%d", atoi(tok));
        array_push(&arr, s);
        tok = strtok(NULL, " \n");
    }

    for (size_t i = 0; i < 25; ++i) {
        //printarr(&arr);
        printf("%zu\n", i);
        for (size_t j = 0; j < arr.size; ++j) {
            char* str = array_at(&arr, j);
            size_t len = strlen(str);
            int n = atoi(str);
            if (n == 0) {
                str[0] = '1';
            } else if (len % 2 == 0) {
                len /= 2;
                memcpy(s, str + len, len + 1);
                str[len] = 0;
                
                while (len > 1 && s[0] == '0') {
                    memmove(s, s + 1, len--);
                }

                array_insert(&arr, s, ++j);
            } else {
                sprintf(str, "%d", n * 2024);
            }
        }
    }

    printf("puzzle 1: %zu\n", arr.size);

    free(buf);
    return EXIT_SUCCESS;
}
