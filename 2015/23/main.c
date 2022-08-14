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

#define strcheck(opcode, str) (strstr(opcode, str) == opcode) 

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

    char opcodes[48][32];

    long j = 0, linecount = 0;
    char str[32];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            printf("%ld.- '%s'\n", linecount, str);
            strcpy(opcodes[linecount], str);
            ++linecount;
        }
        else str[j++] = buf[i];
    }

    str[j] = 0;
    printf("%ld.- '%s'\n", linecount, str);
    strcpy(opcodes[linecount], str);
    ++linecount;

    printf("Lines: %ld\n", linecount);
    
    char reg[16] = {0};
    int a = 1, b = 0, n;
    for (int i = 0; i < linecount; ++i) {
        printf("%d.- %d %d\n", i, a, b);
        if (strcheck(opcodes[i], "hlf")) {
            sscanf(opcodes[i], "hlf %s", reg);
            if (reg[0] == 'a') a /= 2;
            else if (reg[0] == 'b') b /= 2;
        }
        else if (strcheck(opcodes[i], "tpl")) {
            sscanf(opcodes[i], "tpl %s", reg);
            if (reg[0] == 'a') a *= 3;
            else if (reg[0] == 'b') b *= 3;
        }
        else if (strcheck(opcodes[i], "inc")) {
            sscanf(opcodes[i], "inc %s", reg);
            if (reg[0] == 'a') ++a;
            else if (reg[0] == 'b') ++b;
        }
        else if (strcheck(opcodes[i], "jmp")) {
            sscanf(opcodes[i], "jmp %d", &n);
            i += n - 1;
        }
        else if (strcheck(opcodes[i], "jie")) {
            sscanf(opcodes[i], "jie %s %d", reg, &n);
            if (reg[0] == 'a' && !(a % 2)) i += n - 1;
            else if (reg[1] == 'b' && !(b % 2)) i += n - 1;
        }
        else if (strcheck(opcodes[i], "jio")) {
            sscanf(opcodes[i], "jio %s %d", reg, &n);
            printf("jio %s %d\n", reg, n);
            if (reg[0] == 'a' && a == 1) i += n - 1;
            else if (reg[1] == 'b' && b == 1) i += n - 1;
        }
    }

    printf("a: %d\nb: %d\nPuzzle 1: %d\n", a, b, b);

    free(buf);
    return EXIT_SUCCESS;
}
