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

static const char* abc = "abcdefghijklmnopqrstuvwxyz";

static int conditions(const char* str)
{ 
    const long len = strlen(str);
    const long abclen = strlen(abc);

    int first = 0, second = 0, third = 0;

    for (long i = 0; i < len - 2; ++i) {
        for (long j = 0; j < abclen - 2; ++j) {
            first += !memcmp(str + i, abc + j, 3);
        }
    }

    second += !strchr(str, 'i') && !strchr(str, 'o') && !strchr(str, 'l');

    int paircount = 0;
    char pairs[16][4];
    memset(pairs, 0, 16 * 4);
    
    for (long i = 0; i < len; ++i) {
        if (str[i + 1] == str[i]) {
            
            int found = 0;
            for (int j = 0; j < paircount; ++j) {
                if (str[i] == pairs[j][0]) {
                    ++found;
                    break;
                }
            }

            if (!found) {
                memset(pairs[paircount], str[i++], 2);
                pairs[paircount++][2] = 0;
            }
        }
    }

    third = (paircount > 1);

    printf("'%s'\n", str);
    printf("First: %d\nSecond: %d\nThird: %d\n", first, second, third);

    return first * second * third;
}

static void permute(char* str)
{
    const long len = strlen(str);
    int passover = 1;
    for (long i = len - 1; i >= 0; --i) {
        if (passover) {
            if (str[i] == 'z') {
                str[i] = 'a';
            } else {
                str[i] = *(strchr(abc, str[i]) + 1);
                --passover;
            }
        } else {
            break;
        }
    }
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

    char* newline = strchr(buf, '\n');
    if (newline) *newline = 0;

    while (!conditions(buf)) {
        permute(buf);
    }

    printf("Puzzle 1: %s\n", buf);

    permute(buf);
    while (!conditions(buf)) {
        permute(buf);
    }

    printf("Puzzle 2: %s\n", buf);

    free(buf);
    return EXIT_SUCCESS;
}
