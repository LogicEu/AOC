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

static int puzzle(const char* str)
{
    char buf[STRLEN];
    strcpy(buf, str);

    char* s = strtok(buf, " \t");
    int max = 0, min = 0xffff, n = atoi(s);

    max = n > max ? n : max;
    min = n < min ? n : min;

    while ((s = strtok(NULL, " \t"))) {
        n = atoi(s);
        max = n > max ? n : max;
        min = n < min ? n : min;
    }
    
    return max - min;
}

static int puzzle2(const char* str)
{
    char buf[STRLEN];
    strcpy(buf, str);

    char* s = strtok(buf, " \t");
    int nums[16], k = 0;
    nums[k++] = atoi(s);

    while ((s = strtok(NULL, " \t"))) {
        nums[k++] = atoi(s);
    }
    
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            if (i != j && !(nums[i] % nums[j])) {
               return nums[i] / nums[j];
            } 
        }
    }

    printf("Assumption error! '%s'\n", str);
    return 0;
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

    char str[0xfff];
    long j = 0, count = 0, count2 = 0;

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            count += puzzle(str);
            count2 += puzzle2(str);
        }
        else str[j++] = buf[i];
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
