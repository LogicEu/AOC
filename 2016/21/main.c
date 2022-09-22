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

#define STRLEN 0xffff

static const char* input = "abcdefgh";
static const char* input2 = "fbgdceah";

static inline void strrot(char* str)
{
    int i;
    for (i = 0; str[i]; ++i) {}
    char c = str[--i];
    for (; i > 0; --i) {
        str[i] = str[i - 1];
    }
    str[0] = c;
}

static inline void strlrot(char* str)
{
    int i;
    char c = str[0];
    for (i = 0; str[i + 1]; ++i) {
        str[i] = str[i + 1];
    }
    str[i] = c;
}

static inline void strrev(char* str, const int x, const int y)
{
    int i, j;
    char buf[16] = {0};
    for (i = y, j = 0; i >= x; --i) {
        buf[j++] = str[i];
    }
    memcpy(str + x, buf, j);
}

static void puzzle2(const char* str, char* scrumble)
{
    printf("%s\n", scrumble);
    int x, y;
    char a, b;
    if (strstr(str, "swap position ")) {
        sscanf(str, "swap position %d with position %d", &x, &y);
        printf( "swap position %d with position %d\n", x, y);
        a = scrumble[x];
        scrumble[x] = scrumble[y];
        scrumble[y] = a;
    }
    else if (strstr(str, "swap letter ")) {
        sscanf(str, "swap letter %c with letter %c", &a, &b);
        printf("swap letter %c with letter %c\n", a, b);
        for (int i = 0; scrumble[i]; ++i) {
            if (scrumble[i] == a) {
                scrumble[i] = b;
            }
            else if (scrumble[i] == b) {
                scrumble[i] = a;
            }
        }
    }
    else if (strstr(str, "rotate based on position of letter ")) {
        sscanf(str, "rotate based on position of letter %c", &a);
        printf("rotate based on position of letter %c\n", a);

        int i = 0;
        strlrot(scrumble);
        while (strchr(scrumble, a) - scrumble != i) {
            strlrot(scrumble);
            ++i;
            if (i == 4) {
                strlrot(scrumble);
            }
        }
    }
    else if (strstr(str, "rotate ")) {
        char side[16], steps[16];
        sscanf(str, "rotate %s %d %s", side, &x, steps);
        printf("rotate %s %d %s\n", side, x, steps);

        if (strstr(side, "right")) {
            for (int i = 0; i < x; ++i) {
                strlrot(scrumble);            
            }
        }
        else {
            for (int i = 0; i < x; ++i) {
                strrot(scrumble);
            }
        }
    }
    else if (strstr(str, "reverse positions ")) {
        sscanf(str, "reverse positions %d through %d", &x, &y);
        printf("reverse positions %d through %d\n", x, y);
        strrev(scrumble, x, y);
    }
    else if (strstr(str, "move position ")) {
        sscanf(str, "move position %d to position %d", &y, &x);
        printf("move position %d to position %d\n", x, y);
        a = scrumble[x];
        size_t size = strlen(scrumble);
        memmove(scrumble + x, scrumble + x + 1, size);
        memmove(scrumble + y + 1, scrumble + y, size);
        scrumble[y] = a;
    }
}

static void puzzle(const char* str, char* scrumble)
{
    int x, y;
    char a, b;
    if (strstr(str, "swap position ")) {
        sscanf(str, "swap position %d with position %d", &x, &y);
        //printf( "swap position %d with position %d\n", x, y);
        a = scrumble[x];
        scrumble[x] = scrumble[y];
        scrumble[y] = a;
    }
    else if (strstr(str, "swap letter ")) {
        sscanf(str, "swap letter %c with letter %c", &a, &b);
        //printf("swap letter %c with letter %c\n", a, b);
        for (int i = 0; scrumble[i]; ++i) {
            if (scrumble[i] == a) {
                scrumble[i] = b;
            }
            else if (scrumble[i] == b) {
                scrumble[i] = a;
            }
        }
    }
    else if (strstr(str, "rotate based on position of letter ")) {
        sscanf(str, "rotate based on position of letter %c", &a);
        //printf("rotate based on position of letter %c\n", a);
        int i, j;
        for (i = 0; scrumble[i] != a; ++i) {}
        const int rot = i + 1 + (i > 3);
        for (j = 0; j < rot; ++j) {
            strrot(scrumble);            
        }
    }
    else if (strstr(str, "rotate ")) {
        char side[16], steps[16];
        sscanf(str, "rotate %s %d %s", side, &x, steps);
        //printf("rotate %s %d %s\n", side, x, steps);

        if (strstr(side, "right")) {
            for (int i = 0; i < x; ++i) {
                strrot(scrumble);            
            }
        }
        else {
            for (int i = 0; i < x; ++i) {
                strlrot(scrumble);
            }
        }
    }
    else if (strstr(str, "reverse positions ")) {
        sscanf(str, "reverse positions %d through %d", &x, &y);
        //printf("reverse positions %d through %d\n", x, y);
        strrev(scrumble, x, y);
    }
    else if (strstr(str, "move position ")) {
        sscanf(str, "move position %d to position %d", &x, &y);
        //printf("move position %d to position %d\n", x, y);
        a = scrumble[x];
        size_t size = strlen(scrumble);
        memmove(scrumble + x, scrumble + x + 1, size);
        memmove(scrumble + y + 1, scrumble + y, size);
        scrumble[y] = a;
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

    long j = 0, linecount = 0;
    char lines[128][128];
    char str[STRLEN], scramble[STRLEN] = {0}, scramble2[STRLEN] = {0};
    strcpy(scramble, input);
    strcpy(scramble2, input2);

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            puzzle(str, scramble);
            strcpy(lines[linecount++], str);
        }
        else str[j++] = buf[i];  
    }

    if (j) {
        str[j] = 0;
        puzzle(str, scramble);
        strcpy(lines[linecount++], str);
    }

    printf("Puzzle 1: %s\n", scramble);


    for (long i = linecount - 1; i >= 0; --i) {
        puzzle2(lines[i], scramble2);
    }

    printf("Puzzle 2: %s\n", scramble2);

    free(buf);
    return EXIT_SUCCESS;
}
