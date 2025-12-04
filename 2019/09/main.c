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

static long base = 0;

static long* mode(long* n, const long i, const long m)
{
    switch (m) {
        case 0: return n + n[i];
        case 1: return n + i;
        case 2: return n + n[i] + base;
    }
    printf("Failed (Mode %ld)!\n", m);
    exit(EXIT_FAILURE);
}

static int intcode(long* arr, const long* input, long* out, const long from)
{
    long m[4], i, j, k, n, step, *a, *b, *c, incount = 0;
    for (i = from; i < ARRLEN; i += step) {
        j = 0, n = 100;
        for (m[j++] = arr[i] % n; j < 4; ++j) {
            k = n * 10;
            m[j] = (arr[i] % k) / n;
            n = k;
        }
        switch (m[0]) {
            case 99: return -1;
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
                *a = input[incount++];
                step = 2;
                break;
            case 4:
                a = mode(arr, i + 1, m[1]);
                *out = *a;
                return i + 2;
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
            case 9:
                a = mode(arr, i + 1, m[1]);
                base += *a;
                step = 2;
                break;
            default:
                printf("Error (Opcode %ld)!\n", m[0]);
                exit(EXIT_FAILURE);
        }
    }
    printf("Arrays have to be larger!\n");
    exit(EXIT_FAILURE);
    return -2;
}

static long intrun(long* arr, const long* input)
{
    long output, i, n = 0;
    base = 0;
    for (i = 0; i >= 0; i = n) {
        n = intcode(arr, input, &output, n);
    }
    return output;
}

static long puzzle1(const long* array, const long input)
{
    long arr[ARRLEN];
    memcpy(arr, array, ARRLEN * sizeof(long));
    return intrun(arr, &input);
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

    static const char* comma = ",";
    char* tok = strtok(buf, comma);
    long arr[ARRLEN] = {0}, arrcount = 0;
    while (tok) {
        arr[arrcount++] = atol(tok);
        if (arrcount > ARRLEN) {
            printf("Stack overflow!\n");
            return EXIT_FAILURE;
        }
        tok = strtok(NULL, comma);
    }
    free(buf);

    printf("Puzzle 1: %ld\n", puzzle1(arr, 1));
    printf("Puzzle 2: %ld\n", puzzle1(arr, 2));

    return EXIT_SUCCESS;
}
