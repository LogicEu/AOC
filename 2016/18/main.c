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

#define STRLEN 128

static inline long countstr(const char* str)
{
    long count = 0;
    for (long i = 0; str[i]; ++i) {
        count += str[i] == '.';
    }
    return count;
}

static long puzzle(char* str, const int size)
{
    long count = countstr(str);
    char buf[STRLEN] = {0};
    for (int j = 0; j < size - 1; ++j) {
        for (int i = 0; str[i]; ++i) {
            char left = (i > 0) ? str[i - 1] : '.'; 
            char right = str[i + 1] ? str[i + 1] : '.'; 
            char center = str[i];

            if ((left == '^' && center == '^' && right == '.') || 
                (center == '^' && right == '^' && left == '.') ||
                (left == '^' && center == '.' && right == '.') ||
                (right == '^' && left == '.' && center == '.')) {
                buf[i] = '^';
            }
            else {
                buf[i] = '.';
                ++count;
            }
        }
        strcpy(str, buf);
    }
    return count;
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

    long j = 0;
    char str[STRLEN];

    for (long i = 0; buf[i]; ++i) {
        str[j++] = buf[i];
        if (j > STRLEN) {
            printf("Error: string buffer is too small!\n");
            return EXIT_FAILURE;
        }       
    }

    if (j) {
        str[j] = 0;
        char s[STRLEN];
        strcpy(s, str);
        printf("Puzzle 1: %ld\n", puzzle(s, 40));

        strcpy(s, str);
        printf("Puzzle 2: %ld\n", puzzle(s, 400000));
    }

    free(buf);
    return EXIT_SUCCESS;
}
