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

#define BUFSIZE 32
#define EXPCOUNT 512

typedef struct str_t {
    char buf[BUFSIZE];
} str_t;

typedef struct var_t {
    char name[4];
    uint16_t val;
} var_t;

static var_t vars[512];
static long varcount = 0;
static const char* assign = " -> ";

static var_t* varfind(const char* name)
{
    const long len = varcount;
    for (long i = 0; i < len; ++i) {
        if (!strcmp(vars[i].name, name)) {
            return vars + i;
        }
    }
    return NULL;
}

static char* expfind(str_t* expressions, const long words, const char* key)
{
    char str[BUFSIZE];

    const long klen = strlen(key);
    const long alen = strlen(assign);

    for (long i = 0; i < words; ++i) {
        
        strcpy(str, assign);
        strcat(str, key);
        
        char* s = strstr(expressions[i].buf, str);
        if (s && strlen(s) - alen == klen) {
            //printf("Node: '%s'\n", expressions[i].buf);
            return expressions[i].buf;
        }
    }

    return NULL;
}

static uint16_t expnode(str_t* expressions, const long words, const char* str)
{
    char buf[BUFSIZE] = {0};
    char* s = strstr(str, assign);
    if (!s) {
        printf("Could not found ' -> ' in string '%s'.\n", str);
        return 0;
    }

    memcpy(buf, str, s - str);

    long wordcount = 0, mark = 0;
    char word[3][8];
    memset(word, 0, 3 * 8);

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == ' ') {
            memcpy(word[wordcount++], buf + mark, i - mark);
            mark = i + 1;
        }
    }
    
    memcpy(word[wordcount++], buf + mark, strlen(buf + mark));

    char op[8] = {0}, v1[8] = {0}, v2[8] = {0};

    for (long i = 0; i < wordcount; ++i) {
        if (*word[i] >= 'A' && *word[i] <= 'Z') {
            strcpy(op, word[i]);
        }
        else if (!*v1) {
            strcpy(v1, word[i]);
        } else {
            strcpy(v2, word[i]);
        }
    }

    uint16_t val1, val2;

    if (*v1 >= 'a' && *v1 <= 'z') {
        var_t* var = varfind(v1);
        if (var) {
            val1 = var->val;
        } else {
            s = expfind(expressions, words, v1);
            val1 = expnode(expressions, words, s);           
            strcpy(vars[varcount].name, v1);
            vars[varcount].val = val1;
            ++varcount;
        }
    } else {
        val1 = (uint16_t)atoi(v1);
    }

    if (!*v2) {
        return !*op ? val1 : ~val1;
    }
    else if (*v2 >= 'a' && *v2 <= 'z') {
        s = expfind(expressions, words, v2);
        val2 = expnode(expressions, words, s);
        strcpy(vars[varcount].name, v2);
        vars[varcount].val = val2;
        ++varcount; 
    } else {
        val2 = (uint16_t)atoi(v2);
    }

    if (!strcmp(op, "AND")) {
        return val1 & val2;
    }
    else if (!strcmp(op, "OR")) {
        return val1 | val2;
    }
    else if (!strcmp(op, "RSHIFT")) {
        return val1 >> val2;
    }
    else if (!strcmp(op, "LSHIFT")) {
        return val1 << val2;
    }

    printf("MAJOR ERROR!\n");
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

    // Puzzle 1

    char str[BUFSIZE];
    long j = 0, word = 0;

    str_t expressions[EXPCOUNT];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            strcpy(expressions[word++].buf, str);
        }
        else str[j++] = buf[i];
    }

    char* s = expfind(expressions, word, "a");
    uint16_t result = expnode(expressions, word, s);
    printf("Puzzle 1: %u\n", result);

    // Puzzle 2
    
    strcpy(vars[0].name, "b");
    vars[0].val = result;
    varcount = 1;

    s = expfind(expressions, word, "a");
    result = expnode(expressions, word, s);
    printf("Puzzle 2: %u\n", result);
    
    free(buf);
    return EXIT_SUCCESS;
}
