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

static inline void strrev(char* str)
{
    char s[16];
    const int size = strlen(str);
    for (int i = size - 1; i >= 0; --i) {
        s[size - 1 - i] = str[i];
    }
    memcpy(str, s, size);
}

static void sort(int* array, char* str, const int size)
{
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - 1 - i; ++j) {
            if (array[j] < array[j + 1] || 
                (array[j] == array[j + 1] && str[j] > str[j + 1])) {
                int tmp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = tmp;

                char ch = str[j];
                str[j] = str[j + 1];
                str[j + 1] = ch;
            }
        }
    }
}

static void forward(char* str, const int n)
{
    const int len = strlen(abc);
    for (int i = 0; str[i]; ++i) {
        char* ch = strchr(abc, str[i]);
        if (!ch) {
            if (str[i] == '-') {
                str[i] = ' ';
            }
            continue;
        }
        
        int index = (ch - abc + n) % len;
        str[i] = abc[index];
    }
}

static int checksum(char* str)
{

    char abcsort[32];
    int repetitions[32] = {0};
    strcpy(abcsort, abc);

    char* bracket = strchr(str, '[');
    if (!bracket) {
        return 0;
    }
    
    char* close = strchr(bracket, ']');
    if (!close) {
        return 0;
    }
    
    *close = 0;

    char n[16], i = 0;
    char* num = bracket - 1;
    while (*num >= '0' && *num <= '9') {
        n[i++] = *(num--);
    }

    strrev(n);
    const int id = atoi(n);
    const size_t len = num - str;

    for (int i = 0; i < (int)len; ++i) {
        char* ch = strchr(abc, str[i]);
        if (!ch) {
            continue;
        }

        size_t index = ch - abc;
        ++repetitions[index];
    }

    sort(repetitions, abcsort, 32);

    char order[6];
    for (int i = 0; i < 5; ++i) {
        order[i] = abcsort[i];
    }
    order[5] = 0;

    int ret = !strcmp(order, bracket + 1) * id;
    if (ret) {
        *num = 0;
        forward(str, id);
        if (strstr(str, "north") && strstr(str, "object") && strstr(str, "storage")) {
            printf("Puzzle 2: %d\n", id);
        }
    }

    return ret;
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

    int count = 0;

    long j = 0, linecount = 0;
    char str[128];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            //printf("%s\n", str);
            count += checksum(str);
            ++linecount;
        }
        else str[j++] = buf[i];
    }

    str[j] = 0;
    //printf("%s\n", str);
    count += checksum(str);
    ++linecount;
   
    printf("Puzzle 1: %d\n", count);

    free(buf);
    return EXIT_SUCCESS;
}
