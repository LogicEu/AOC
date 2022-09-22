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

#define STRLEN 16
#define COUNT 128

static long puzzle1(const int* arr, const int* sec, const int* sgns)
{
    static int security[COUNT], signs[COUNT];
    memcpy(security, sec, COUNT * sizeof(int));
    memcpy(signs, sgns, COUNT * sizeof(int));
    
    long severity = 0, t = 0;
    for (int i = 0; i < COUNT; ++i) {
        if (arr[i] && !security[i]) {
            severity += i * arr[i];
            t = 1;
        }

        for (int j = 0; j < COUNT; ++j) {
            if (arr[j]) {
                if (!security[j] || security[j] == arr[j] - 1) {
                    signs[j] = (signs[j] + 1) % 2;
                }
                security[j] += signs[j] * 2 - 1;
            }
        }
    }

    return severity + !severity * t;
}

static int check(const int* arr, const int* sec, const int* sgns)
{
    static int security[COUNT], signs[COUNT];
    memcpy(security, sec, COUNT * sizeof(int));
    memcpy(signs, sgns, COUNT * sizeof(int));
    
    for (int i = 0; i < COUNT; ++i) {
        if (arr[i] && !security[i]) {
            return 1;
        }

        for (int j = 0; j < COUNT; ++j) {
            if (arr[j]) {
                if (!security[j] || security[j] == arr[j] - 1) {
                    signs[j] = (signs[j] + 1) % 2;
                }
                security[j] += signs[j] * 2 - 1;
            }
        }
    }

    return 0;
}
static long puzzle2(const int* arr)
{
    static int security[COUNT] = {0}, signs[COUNT] = {0};
    long i, caught = 1;
    for (i = 0; caught; ++i) {
        caught = check(arr, security, signs);
        for (int j = 0; j < COUNT; ++j) {
            if (arr[j]) {
                if (!security[j] || security[j] == arr[j] - 1) {
                    signs[j] = (signs[j] + 1) % 2;
                }
                security[j] += signs[j] * 2 - 1;
            }
        }
    }
    return i - 1;
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

    long j = 0, n, m;
    char str[STRLEN];
    int firewall[COUNT] = {0};

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            sscanf(str, "%ld: %ld", &n, &m);
            firewall[n] = m;
        }
        else str[j++] = buf[i];
    }

    if (j) {
        str[j] = 0;
        sscanf(str, "%ld: %ld", &n, &m);
        firewall[n] = m;
    }

    int security[COUNT] = {0}, signs[COUNT] = {0};
    printf("Puzzle 1: %ld\n", puzzle1(firewall, security, signs));
    printf("Puzzle 2: %ld\n", puzzle2(firewall));

    free(buf);
    return EXIT_SUCCESS;
}
