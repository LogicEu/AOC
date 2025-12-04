#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

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

#define STRLEN 0xFFF

static long isValid(const long number)
{
    char num[0xFF], n[0xFF], m[0xFF];
    sprintf(num, "%ld", number);
    long len = strlen(num);
    if (len % 2) {
        return 1;
    }

    long hlen = len / 2;
    memcpy(n, num, hlen);
    memcpy(m, num + hlen, hlen);
    long ret = !!memcmp(n, m, hlen);
    if (!ret) {
        printf("invalid: %ld\n", number);
    }
    return ret;
}

static long isValid2(const long n)
{
    long i, j, len, found;
    char num[0xFF], subnum[0xFF];
    sprintf(num, "%ld", n);

    for (i = 0; num[i]; ++i) {
        len = i + 1;
        found = 0;
        memset(subnum, 0, 0xFF);
        memcpy(subnum, num, len);

        for (j = 1; num[len * j]; ++j) {
            if (memcmp(num + len * j, subnum, len)) {
                ++found;
                break;
            }
        }

        if (!found && j > 1) {
            // printf("invalid: %ld (%ld, %ld)\n", n, j, found);
            return 1;
        }
        
    }
    
    return 0;
}

static long getRange2(char* range)
{
    long start, end, count = 0;
    sscanf(range, "%ld-%ld", &start, &end);
    for (long i = start; i <= end; ++i) {
        count += i * isValid2(i);
    }
    
    return count;
}

static long getRange(char* range)
{
    long start, end, count = 0;
    sscanf(range, "%ld-%ld", &start, &end);
    for (long i = start; i <= end; ++i) {
        count += i * !isValid(i);
    }
    
    return count;
}

static long puzzle2(char* str)
{
    static const char* sep = ",";
    char* range = strtok(str, sep);
    long count = 0;
    while (range) {
        count += getRange2(range);
        range = strtok(NULL, sep);
    }
    return count;
}

static long puzzle(char* str)
{
    static const char* sep = ",";
    char* range = strtok(str, sep);
    long count = 0;
    while (range) {
        count += getRange(range);
        range = strtok(NULL, sep);
    }
    return count;
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

    long j = 0, count = 0;
    char str[STRLEN];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            count += puzzle2(str);
        }
        else str[j++] = buf[i];

        if (j > STRLEN) {
            printf("Error: string buffer is too small!\n");
            return EXIT_FAILURE;
        }       
    }

    if (j) {
        str[j] = 0;
        count += puzzle2(str);
    }

    //printf("puzzle 1: %ld\n", count);
    printf("puzzle 2: %ld\n", count);

    free(buf);
    return EXIT_SUCCESS;
}
