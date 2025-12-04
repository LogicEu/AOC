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

#define ARRLEN 0xfff

static int* mode(int* n, const int i, const int m)
{
    switch (m) {
        case 0: return n + n[i];
        case 1: return n + i;
    }
    printf("Failed (Mode %d)!\n", m);
    exit(EXIT_FAILURE);
}

static int intcode(const int* array, const int count, const int input)
{
    int arr[ARRLEN], m[4], i, j, k, n, step, *a, *b, *c;
    memcpy(arr, array, count * sizeof(int));
    for (i = 0; i < count; i += step) {
        j = 0, n = 100;
        for (m[j++] = arr[i] % n; j < 4; ++j) {
            k = n * 10;
            m[j] = (arr[i] % k) / n;
            n = k;
        }
        switch (m[0]) {
            case 99: return arr[0];
            case 1:
                a = mode(arr, i + 1, m[1]);
                b = mode(arr, i + 2, m[2]);
                c = mode(arr, i + 3, m[3]);
                *c = *a + *b;
                step = 4;
                break;
            case 2:
                a = mode(arr, i + 1, m[1]);
                b = mode(arr, i + 2, m[2]);
                c = mode(arr, i + 3, m[3]);
                *c = *a * *b;
                step = 4;
                break;
            case 3:
                a = mode(arr, i + 1, m[1]);
                *a = input;
                step = 2;
                break;
            case 4:
                a = mode(arr, i + 1, m[1]);
                printf("Intcode Output: %d\n", *a);
                step = 2;
                break;
            case 5:
                a = mode(arr, i + 1, m[1]);
                if (*a) {
                    b = mode(arr, i + 2, m[2]);
                    i = *b;
                    step = 0;
                    
                } else step = 3;
                break;
            case 6:
                a = mode(arr, i + 1, m[1]);
                if (!*a) {
                    b = mode(arr, i + 2, m[2]);
                    i = *b;
                    step = 0;
                    
                } else step = 3;
                break;
            case 7:
                a = mode(arr, i + 1, m[1]);
                b = mode(arr, i + 2, m[2]);
                c = mode(arr, i + 3, m[3]);
                *c = (*a < *b);
                step = 4;
                break;
            case 8:
                a = mode(arr, i + 1, m[1]);
                b = mode(arr, i + 2, m[2]);
                c = mode(arr, i + 3, m[3]);
                *c = (*a == *b);
                step = 4;
                break;
            default:
                printf("Error (Opcode %d)!\n", m[0]);
                exit(EXIT_FAILURE);
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
            if (count + 1 >= ARRLEN) {
                printf("Stack overflow!\n");
                return EXIT_FAILURE;
            }
        }
    }
    arr[count++] = atoi(buf + j);
    free(buf);

    printf("Puzzle 1: %d\n", intcode(arr, count, 1));
    printf("Puzzle 2: %d\n", intcode(arr, count, 5));
    return EXIT_SUCCESS;
}
