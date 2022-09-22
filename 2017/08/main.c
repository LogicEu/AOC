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

typedef struct var_t {
    char name[8];
    int val;
} var_t;

static const char* space = " ";
static int supermax = -0xffff;

static var_t catch(char* str)
{
    var_t var;
    var.val = 0;
    var.name[0] = 0;
    
    char* s = strtok(str, space);
    if (s) {
        strcpy(var.name, s);
    } 
    
    return var;
}

static long find(var_t* vars, const char* name, const long size)
{
    for (long i = 0; i < size; ++i) {
        if (!strcmp(vars[i].name, name)) {
            return i + 1;
        }
    }
    return 0;
}

static int check(const int n, const char* op, const int m)
{
    if (!strcmp(op, ">")) {
        return (n > m);
    }
    if (!strcmp(op, "<")) {
        return (n < m);
    }
    if (!strcmp(op, ">=")) {
        return (n >= m);
    }
    if (!strcmp(op, "<=")) {
        return (n <= m);
    }
    if (!strcmp(op, "==")) {
        return (n == m);
    }
    if (!strcmp(op, "!=")) {
        return (n != m);
    }

    printf("Error 1!\n");
    return 0;
}

static void parse(const char* str, var_t* vars, const long varcount)
{
    int n, m;
    char op[4], symbol[4], rname[4], lname[4];
    sscanf(str, "%s %s %d if %s %s %d", rname, op, &n, lname, symbol, &m);

    long l = find(vars, lname, varcount);
    long r = find(vars, rname, varcount);

    if (!l || !r) {
        printf("Error 2! %ld %ld\n", r, l);
        return;
    }
    
    --l, --r;
    if (check(vars[l].val, symbol, m)) {
        vars[r].val += !strcmp(op, "dec") ? -n : n;
        if (vars[r].val > supermax) {
            supermax = vars[r].val;
        }
    }
}

static int puzzle(char* buf, var_t* vars, const long varcount)
{
    static const char* nxtline = "\n";
    
    for (long i = 0; i < varcount; ++i) {
        if (vars[i].val > supermax) {
            supermax = vars[i].val;
        }
    }

    char* s = strtok(buf, nxtline);
    parse(s, vars, varcount);
    while ((s = strtok(NULL, nxtline))) {
        parse(s, vars, varcount);
    }

    int max = -0xffff;
    for (long i = 0; i < varcount; ++i) {
        if (vars[i].val > max) {
            max = vars[i].val;
        }
    }
    return max;
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

    long j = 0, varcount = 0;
    char str[STRLEN];
    var_t vars[0xfff];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            vars[varcount++] = catch(str);
        }
        else str[j++] = buf[i];

        if (j > STRLEN) {
            printf("Error: string buffer is too small!\n");
            return EXIT_FAILURE;
        }       
    }

    if (j) {
        str[j] = 0;
        vars[varcount++] = catch(str);
    }
    
    for (long i = 0; i < varcount; ++i) {
        for (long j = i + 1; j < varcount; ++j) {
            if (!strcmp(vars[i].name, vars[j].name)) {
                memmove(vars + j, vars + j + 1, (--varcount - j) * sizeof(var_t));
                --j;
            }
        }
    }

    printf("Puzzle 1: %d\n", puzzle(buf, vars, varcount));
    printf("Puzzle 2: %d\n", supermax);

    free(buf);
    return EXIT_SUCCESS;
}
