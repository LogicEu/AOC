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

static void puzzle(const char* str)
{
    long scope = 0, sum = 0, garbage = 0, chars = 0;
    for (long i = 0; str[i]; ++i) {
        if (!garbage) {
            if (str[i] == '{' && !garbage) {
                ++scope;
                sum += scope;
            }
            else if (str[i] == '}') {
                --scope;
            }
            garbage += str[i] == '<';
        } else {
            if (str[i] == '!') {
                ++i;
                continue;
            }
            if (str[i] == '>') {
                --garbage;
                continue;
            }
            ++chars;
        }
    }

    printf("Puzzle 1: %ld\n", sum);
    printf("Puzzle 2: %ld\n", chars);
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

    puzzle(buf);
    
    free(buf);
    return EXIT_SUCCESS;
}
