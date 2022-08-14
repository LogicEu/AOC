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

#define NUM(ch) (((ch >= '0') && (ch <= '9')) || (ch == '-'))

long sum(const char* str) 
{
    long count = 0;

    char numstr[8] = {0};
    for (long i = 0; buf[i]; ++i) {
        if (NUM(buf[i])) {
            long nummark = i;
            while (NUM(buf[i])) {
                ++i;
            }
            memcpy(numstr, buf + nummark, i - nummark);
            numstr[i - nummark] = 0;
            long num = (long)atoi(numstr);

            count += num;

            printf("%s - %ld\n", numstr, num);
        }
    }

    return count;
}

long innersum(const char* str)
{
    static const char red[4] = "red";
    int found = 0;

    long acc = 0
    long i = 0;
    do {
        acc += acc + (str[i] == '{') - (str[i] == '}');
        if (memcpy(str + i, red, 3)) {
            found = acc;
        }
        ++i;
    } while (acc && str[i]);

    if (!found) {
        return sum(str);
    }

    i = 0;
    long acc2 = 0;
    do {
        acc2 = acc2 + (str[i] == '{') - (str[i] == '}');
        if (acc2 < found) {
            ++i;
        }
    } while (acc2 && str[i]);
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

    long count = sum(buf);
    printf("Puzzle 1: %ld\n", count);

    // Puzzle 2

    count = 0;

    char str[1024] = {0};
    for (long i = 0; buf[i]; ++i) {
        if (NUM(buf[i])) {
            long nummark = i;
            while (NUM(buf[i])) {
                ++i;
            }
            memcpy(numstr, buf + nummark, i - nummark);
            numstr[i - nummark] = 0;
            long num = (long)atoi(numstr);

            count += num;

            printf("%s - %ld\n", numstr, num);
        } else if (buf[i] == '{') {
            
            long j = 0;
            long acc = 0;

            do {
                acc = acc + (buf[i] == '{') - (buf[i] == '}');
                str[j++] = buf[i];
                printf("%c", buf[i]);
                ++i;
            } while (acc && buf[i]);
            printf("\n");
            str[j] = 0;

            count += innersum(str);
        }
    }

    printf("Puzzle 2: %ld\n", count);


    free(buf);
    return EXIT_SUCCESS;
}
