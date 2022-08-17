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

static int checkstr(const char* str)
{
    const int size = strlen(str);
    int inout = 0, check = 0;
    for (int i = 1; i < size - 1; ++i) {
        inout += str[i] == '[';
        inout -= str[i] == ']';
        if (str[i - 1] != str[i] && str[i - 1] == str[i + 2] && str[i] == str[i + 1]) {
            if (inout) return 0;
            else ++check; 
        }
    }

    return !!check;
}

static int checkstr2(const char* str)
{
    const int size = strlen(str);
    int inout = 0, check = 0;
    for (int i = 1; i < size - 1; ++i) {
        inout += str[i] == '[';
        inout -= str[i] == ']';
        if (!inout && str[i - 1] != str[i] && str[i - 1] == str[i + 1]) {
            int inner = 0;
            for (int j = 1; j < size - 1; ++j) {
                inner += str[j] == '[';
                inner -= str[j] == ']';
                if (inner && str[j] == str[i + 1] && str[j + 1] == str[i] &&
                    str[j - 1] == str[j + 1]) {
                    return 1;
                }
            }
        }
    }

    return 0;
}

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

    long j = 0, linecount = 0, count = 0, count2 = 0;
    char str[1024];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            count += checkstr(str);
            count2 += checkstr2(str);
            ++linecount;
        }
        else str[j++] = buf[i];
    }

    if (j) {
        str[j] = 0;
        count += checkstr(str);
        count += checkstr2(str);
    }

    printf("Puzzle 1: %ld\n", count);
    printf("Puzzle 2: %ld\n", count2);

    free(buf);
    return EXIT_SUCCESS;
}
