#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STRLEN 128

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

static int puzzle(const char* str)
{
    char buf[STRLEN];
    strcpy(buf, str);

    char* s = strtok(buf, " ");
    char** words = malloc(STRLEN * sizeof(char*));
    int k = 0, ret = 1;

    words[k] = calloc(STRLEN, 1);
    strcpy(words[k++], s);
    while ((s = strtok(NULL, " "))) {
        words[k] = calloc(STRLEN, 1);
        strcpy(words[k++], s);
    }

    for (int i = 0; i < k && ret; ++i) {
        for (int j = i + 1; j < k; ++j) {
            if (!strcmp(words[i], words[j])) {
                ret = 0;
                break;
            }
        }
    }

    for (size_t i = 0; i < k; ++i) {
        free(words[i]);
    }
    free(words);

    return ret;
}

static inline void arrcnt(int* arr, const char* str)
{
    for (int i = 0; str[i]; ++i) {
        ++arr[(int)(str[i] - 'a')];
    }   
}

static int anagram(const char* str, const char* str2)
{
    int abc[32] = {0}, abc2[32] = {0};
    arrcnt(abc, str);
    arrcnt(abc2, str2);
    return !memcmp(abc, abc2, 32 * sizeof(int));
}

static int puzzle2(const char* str)
{
    char buf[STRLEN];
    strcpy(buf, str);

    char* s = strtok(buf, " ");
    char** words = malloc(STRLEN * sizeof(char*));
    int k = 0, ret = 1;

    words[k] = calloc(STRLEN, 1);
    strcpy(words[k++], s);
    while ((s = strtok(NULL, " "))) {
        words[k] = calloc(STRLEN, 1);
        strcpy(words[k++], s);
    }

    for (int i = 0; i < k && ret; ++i) {
        for (int j = i + 1; j < k; ++j) {
            if (anagram(words[i], words[j])) {
                printf("'%s' - '%s'\n", words[i], words[j]);
                ret = 0;
                break;
            }
        }
    }

    for (size_t i = 0; i < k; ++i) {
        free(words[i]);
    }
    free(words);

    return ret;
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

    long j = 0, count = 0, count2 = 0;
    char str[STRLEN];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            count += puzzle(str);
            count2 += puzzle2(str);
        }
        else str[j++] = buf[i];

        if (j > STRLEN) {
            printf("Error: string buffer is too small!\n");
            return EXIT_FAILURE;
        }       
    }

    if (j) {
        str[j] = 0;
        count += puzzle(str);
        count2 += puzzle2(str);
    }

    printf("Puzzle 1: %ld\n", count);
    printf("Puzzle 2: %ld\n", count2);

    free(buf);
    return EXIT_SUCCESS;
}
