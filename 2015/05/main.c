#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* file_read(const char* filename)
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

int requirements(const char* str) 
{
    int count = 0, twice = 0;
    for (int i = 0; str[i]; ++i) {
        count += !!strchr("aeiou", str[i]);
        twice += str[i] == str[i + 1];
    }

    if (count < 3           || 
        !twice              ||
        strstr(str, "ab")  ||
        strstr(str, "cd")  ||
        strstr(str, "pq")  ||
        strstr(str, "xy")) {
        return 0;
    }
    
    return 1;
}

int requirements2(const char* str)
{
    int between = 0;
    for (int i = 0; str[i + 2]; ++i) {
        between += str[i] == str[i + 2];
    }
    
    if (!between) {
        return 0;
    }

    int pairs = 0;
    for (int i = 0; str[i + 1]; ++i) {
        for (int j = i + 2; str[j] && str[j + 1]; ++j) {
            pairs += !memcmp(str + i, str + j, 2);
        }
    }

    if (!pairs) {
        return 0;
    }

    return 1;
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
    
    long count = 0, count2 = 0, n = 0;
    char str[32];

    for (long i = 0; buf[i]; ++i) { 
        if (buf[i] != '\n') {
            str[n++] = buf[i];
        }
        else {
            str[n++] = 0;

            //printf("%s\n", str);
            count += requirements(str);
            count2 += requirements2(str);

            memset(str, 0, n);
            n = 0;
        }
    }
    
    str[n++] = 0;
    //printf("%s\n", str);
    count += requirements(str);
    count2 += requirements2(str);
    printf("Nice Strings: %ld\nNice Strings 2: %ld\n", count, count2);

    free(buf);
    return EXIT_SUCCESS;
}
