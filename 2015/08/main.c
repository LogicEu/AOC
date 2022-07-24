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

static long A = 0;
static long B = 0;
static long AA = 0;
static long BB = 0;

static void strcnt(const char* str)
{
    long a = strlen(str);
    long b = a - 2;
    
    char s[128] = {0};
    memcpy(s, str + 1, b);

    for (long i = 0; s[i]; i++) {
        if (s[i] == '\\') {
            if (s[i + 1] == '\\' || s[i + 1] == '\"') {
                --b;
                ++i;
            }
            else if (s[i + 1] == 'x') {
                b -= 3;
                i += 3;
            }
        }
    }

    //printf("'%s' -- [ %ld - %ld = %ld ]\n", s, a, b, a - b);
    A += a;
    B += b;
}

static void strcnt2(const char* str)
{
    long a = strlen(str);
    long b = a + 2;
    
    for (long i = 0; str[i]; i++) {
        if (str[i] == '\\' || str[i] == '\"') {
           ++b; 
        }
    }

    printf("'%s' -- [ %ld - %ld = %ld ]\n", str, b, a, b - a);
    AA += a;
    BB += b;
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

    long j = 0, lines = 0;
    char str[128] = {0};

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            ++lines;
            strcnt(str);
            strcnt2(str);
        }
        else str[j++] = buf[i];
    }

    str[j] = 0;
    ++lines;
    strcnt(str);
    strcnt2(str);

    printf("Lines: %ld\n", lines);
    printf("Puzzle 1: %ld\n", A - B);
    printf("Puzzle 2: %ld - %ld = %ld\n", BB, AA, BB - AA);

    free(buf);
    return EXIT_SUCCESS;
}
