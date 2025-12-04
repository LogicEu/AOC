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

static int mintcode(int* arr, const int count, const int* input, 
                    int* out, const int from)
{
    int m[4], i, j, k, n, step, *a, *b, *c, incount = 0;
    for (i = from; i < count; i += step) {
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
                //printf("Intcode Output: %d\n", *a);
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
            default:
                printf("Error (Opcode %d)!\n", m[0]);
                exit(EXIT_FAILURE);
        }
    }
    return -2;
}

static int intcode(const int* array, const int arrcount, const int* input)
{
    int arr[ARRLEN], out;
    memcpy(arr, array, arrcount * sizeof(int));
    mintcode(arr, arrcount, input, &out, 0);
    return out;
}

#define swap(a, b, type) do { type _swap_tmp = a; a = b; b = _swap_tmp; } while (0)
#define PROGLEN 5

static int try(const int* arr, const int arrcount, const char* input)
{
    int val = 0;
    for (int i = 0; input[i]; ++i) {
        int in[2] = {input[i] - '0', val};
        val = intcode(arr, arrcount, in);
    }
    return val;
}

static int try2(const int* arr, const int arrcount, const char* input)
{
    int progs[PROGLEN][ARRLEN], rs[PROGLEN], in[2], out, i, r;
    inlimit = 2;
    for (i = 0; i < PROGLEN; ++i) {
        memcpy(progs[i], arr, arrcount * sizeof(int));
    }

    out = r = 0;
    for (i = 0; i < 5 && r >= 0; ++i) {
        in[0] = input[i] - '0';
        in[1] = out;
        r = mintcode(progs[i], arrcount, in, &out, 0);
        rs[i] = r;
    }

    inlimit = 1;
    for (i = 0; r >= 0 ; i = (i + 1) % 5) {
        in[0] = out;
        r = mintcode(progs[i], arrcount, in, &out, rs[i]);
        rs[i] = r;
    }

    return out;
}

static void reverse(char* start, char* end)
{
    --end;
    while (start < end) {
        swap(*start, *end, char);
        ++start;
        --end;
    }
}

static int permute(char *first, char *last) { 
    if (last - first < 2) {
        return 0; 
    }
    
    char *i = last - 1;
    while (1) { 
        char *ii = i--; 
        if (*i < *ii) { 
            char *j = last; 
            while (*i >= *--j); 
            swap(*i, *j, char); 
            reverse(ii, last); 
            return 1; 
        } 
        if (i == first) { 
            reverse(first, last); 
            return 0; 
        } 
    } 
} 

static int puzzle1(const int* arr, const int arrcount, const char* in)
{
    const int len = strlen(in);
    int max = -100000, r;
    char input[16];
    memcpy(input, in, len + 1);
    do {
        r = try(arr, arrcount, input);
        if (r > max) {
            max = r;
        }
    } while (permute(input, input + len));
    return max;
}

static int puzzle2(const int* arr, const int arrcount, const char* in)
{
    const int len = strlen(in);
    int max = -100000, r;
    char input[16];
    memcpy(input, in, len + 1);
    do {
        r = try2(arr, arrcount, input);
        if (r > max) {
            max = r;
        }
    } while (permute(input, input + len));
    return max;
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
    int arr[ARRLEN], arrcount = 0;
    while (tok) {
        arr[arrcount++] = atoi(tok);
        if (arrcount > ARRLEN) {
            printf("Stack overflow!\n");
            return EXIT_FAILURE;
        }
        tok = strtok(NULL, comma);
    }
    free(buf);

    printf("Puzzle 1: %d\n", puzzle1(arr, arrcount, "01234"));
    printf("Puzzle 2: %d\n", puzzle2(arr, arrcount, "56789"));
    return EXIT_SUCCESS;
}
