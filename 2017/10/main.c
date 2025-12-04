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

#define STRLEN 256
#define swap(a, b, type) do { type tmp = a; a = b; b = tmp; } while (0)

static void reverse(int* array, const int from, const int len)
{
    for (int i = 0; i < len / 2; ++i) {
        int n = (from + i) % STRLEN;
        int m = (from + len - i - 1) % STRLEN;
        swap(array[n], array[m], int);
    }
}

static int puzzle1(int* array, const int* lengths, const int lencount)
{
    int position = 0, skip = 0; 
    for (int i = 0; i < lencount; ++i) {
        reverse(array, position, lengths[i]);
        position += (lengths[i] + skip);
        position %= STRLEN;
        ++skip;
    }
    return array[0] * array[1];
}

static void hex(const uint8_t* src, char* str, const int len)
{
    static const char hextable[] = "0123456789abcdef";
    for (int i = 0; i < len; ++i) {
        str[i * 2 + 0] = hextable[src[i] >> 4];
        str[i * 2 + 1] = hextable[src[i] & 0x0f];
    }
    str[len * 2] = 0;
}

static char* puzzle2(int* array, const int* lengths, const int lencount)
{
    int position = 0, skip = 0;
    for (int j = 0; j < 64; ++j) {
        for (int i = 0; i < lencount; ++i) {
            reverse(array, position, lengths[i]);
            position += (lengths[i] + skip);
            position %= STRLEN;
            ++skip;
        }
    }
    
    char* ret = malloc(64);
    uint8_t dense[16];
    for (int i = 0; i < 16; ++i) {
        dense[i] = (uint8_t)array[i * 16];
        for (int j = 1; j < 16; ++j) {
            dense[i] ^= (uint8_t)array[i * 16 + j];
        }
    }

    hex(dense, ret, 16);
    return ret;
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

    const int n = strlen(buf) - 1;
    if (buf[n] == '\n') {
       buf[n] = 0;
    }

    int arr[STRLEN], ascii[STRLEN], lengths[STRLEN];
    int lencount = 0, asciicount = 0;

    for (int i = 0; i < STRLEN; ++i) {
        arr[i] = i;
    }

    for (int i = 0; buf[i]; ++i) {
        ascii[asciicount++] = (int)buf[i];
    }
    
    int extra[] = {17, 31, 73, 47, 23};
    for (int i = 0; i < 5; ++i) {
        ascii[asciicount++] = extra[i];
    }

    char* s = strtok(buf, ",");
    if (s) {
        lengths[lencount++] = atoi(s);
        while ((s = strtok(NULL, ","))) {
            lengths[lencount++] = atoi(s);
        }
    }

    printf("Puzzle 1: %d\n", puzzle1(arr, lengths, lencount));
   
    for (int i = 0; i < STRLEN; ++i) {
        arr[i] = i;
    }

    char* r = puzzle2(arr, ascii, asciicount);
    printf("Puzzle 2: %s\n", r);
    
    free(r);
    free(buf);
    return EXIT_SUCCESS;
}
