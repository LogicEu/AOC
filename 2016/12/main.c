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

static int A = 0;
static int B = 0;
static int C = 0;
static int D = 0;

static int* getreg(const char* str)
{
    if (str[0] == 'a') return &A;
    if (str[0] == 'b') return &B;
    if (str[0] == 'c') return &C;
    if (str[0] == 'd') return &D;
    else return NULL;
}

static int getval(const char* str)
{
    int* reg = getreg(str);
    return reg ? *reg : atoi(str);
}

static int opcode(const char* str)
{
    int* reg;
    char reg1[8], reg2[8];
    switch (*str) {
        case 'c': {
            sscanf(str, "cpy %s %s", reg1, reg2);
            reg = getreg(reg2);
            if (reg) {
                *reg = getval(reg1);
            }
            return 0;
        }
        case 'i': {
            sscanf(str, "inc %s", reg1);
            reg = getreg(reg1);
            if (reg) {
                ++(*reg);
            }
            return 0;
        }
        case 'd': {
            sscanf(str, "dec %s", reg1);
            reg = getreg(reg1);
            if (reg) {
                --(*reg);
            }
            return 0;
        }
        case 'j': {
            sscanf(str, "jnz %s %s", reg1, reg2);
            if (getval(reg1)) {
                return getval(reg2) - 1;
            }
        }
        default: return 0;
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

    char ops[24][32];
    long j = 0, linecount = 0;
    char str[128];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j++] = 0;
            memcpy(ops[linecount++], str, j);
            j = 0;
        }
        else str[j++] = buf[i];
    }

    if (j) {
        str[j++] = 0;
        memcpy(ops[linecount++], str, j);
    }

    for (int i = 0; i < linecount; ++i) {
        i += opcode(ops[i]);
    }

    printf("Puzzle 1: %d\n", A);

    A = B = D = 0;
    C = 1;
    
     for (int i = 0; i < linecount; ++i) {
        i += opcode(ops[i]);
    }

    printf("Puzzle 2: %d\n", A);

    free(buf);
    return EXIT_SUCCESS;
}
