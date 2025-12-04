#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define swap(a, b, type) do { type tmp = a; a = b; b = tmp; } while (0)

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

static inline void spin(char* arr, const int n)
{
    char buf[32];
    memcpy(buf, arr, 16);
    memcpy(arr, buf + 16 - n, n);
    memcpy(arr + n, buf, 16 - n);
}

static inline void partner(char* arr, const char a, const char b)
{
    int n = strchr(arr, a) - arr;
    int m = strchr(arr, b) - arr;
    swap(arr[n], arr[m], char);
}

static void op(char* arr, const char* str)
{
    switch (str[0]) {
        case 's': {
            int n = atoi(str + 1);
            spin(arr, n);
            break;
        }
        case 'x': {
            int n, m;
            sscanf(str, "x%d/%d", &n, &m);
            swap(arr[n], arr[m], char);
            break;
        }
        case 'p': {
            char a, b;
            sscanf(str, "x%c/%c", &a, &b);
            partner(arr, a, b);
            break;
        }
    }
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

    static const char* div = ", \n";
    
    char arr[32];
    for (int i = 0; i < 16; ++i) {
        arr[i] = i + 'a';
    }
    arr[16] = 0;

    char* s = strtok(buf, s);
    op(arr, s);
    while ((s = strtok(NULL, s))) {
        op(arr, s);
    }

    printf("Puzzle 1: %s\n", arr);

    free(buf);
    return EXIT_SUCCESS;
}
