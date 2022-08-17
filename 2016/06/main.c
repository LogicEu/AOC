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

static void sort(int* array, char* str, const int size)
{
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - 1 - i; ++j) {
            if (array[j] < array[j + 1]) {
                int tmp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = tmp;

                char ch = str[j];
                str[j] = str[j + 1];
                str[j + 1] = ch;
            }
        }
    }
}

#define COUNT 8

int main(int argc, char** argv)
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

    // Puzzle

    static const char* abc = "abcdefghijklmnopqrstuvwxyz";
    const size_t abclen = strlen(abc);

    char abctable[COUNT][32];
    int table[COUNT][32];
    for (int i = 0; i < COUNT; ++i) {
        for (int j = 0; j < 32; ++j) {
           table[i][j] = 0;
           abctable[i][j] = 0;
        } 
    }

    long j = 0, linecount = 0;
    char str[128];

    for (long i = 0; buf[i]; ++i) {
        int end = !buf[i + 1];
        if (end) {
            str[j++] = buf[i];
        }

        if (buf[i] == '\n' || end) {

            str[j] = 0;
            j = 0;
            
            for (int k = 0; k < COUNT; ++k) {
                char* ch = strchr(abc, str[k]);
                if (ch) {
                    const int index = (int)(size_t)(ch - abc);
                    ++table[k][index];
                }
            }

            ++linecount;
        }
        else str[j++] = buf[i];
    }

    printf("Puzzle 1: ");
    for (int i = 0; i < COUNT; ++i) {
        strcpy(abctable[i], abc);
        sort(table[i], abctable[i], abclen);
        printf("%c", abctable[i][0]);
    }
    printf("\n");

    printf("Puzzle 2: ");
    for (int i = 0; i < COUNT; ++i) {
        for (j = 0; table[i][j]; ++j) {}
        --j;
        printf("%c", abctable[i][j]);
    }
    printf("\n");

    free(buf);
    return EXIT_SUCCESS;
}
