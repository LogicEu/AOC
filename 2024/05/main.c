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

static int read_update(char* str, int* update)
{
    int count = 0;
    char* tok = strtok(str, ",");
    while (tok) {
        update[count++] = atoi(tok);
        tok = strtok(NULL, ",");
    }

    return count;
}

static long isvalid(const int* update, const int updatecount,
     const int rules[STRLEN][2], const long rulecount)
{
    for (long i = 0; i < updatecount - 1; ++i) {
        for (long j = i + 1; j < updatecount; ++j) {
            int n = update[i], m = update[j];
            for (long k = 0; k < rulecount; ++k) {
                if (rules[k][1] == n && rules[k][0] == m) {
                    return 0;
                }
            }
        }
    }

    return update[updatecount / 2];
}

static long isvalid2(int* update, const int updatecount,
     const int rules[STRLEN][2], const long rulecount, const int validity)
{
    for (long i = 0; i < updatecount - 1; ++i) {
        for (long j = i + 1; j < updatecount; ++j) {
            int n = update[i], m = update[j];
            for (long k = 0; k < rulecount; ++k) {
                if (rules[k][1] == n && rules[k][0] == m) {
                    update[i] = m;
                    update[j] = n;
                    return isvalid2(update, updatecount, rules, rulecount, 1);
                }
            }
        }
    }

    return update[updatecount / 2] * validity;
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

    long j = 0, updatebool = 0, updatecount = 0, rulecount = 0;
    char str[STRLEN];
    int rules[STRLEN][2], updates[STRLEN][32], updatecounts[STRLEN] = {0};

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;

            if (!updatebool) {
                if (str[0] == '\0') {
                    ++updatebool;
                } else {
                    sscanf(str, "%d|%d", rules[rulecount], rules[rulecount] + 1);
                    ++rulecount;
                }
            } else {
                updatecounts[updatecount] = read_update(str, updates[updatecount]);
                updatecount++;
            }
        }
        else str[j++] = buf[i];

        if (j > STRLEN || rulecount > STRLEN || updatecount > STRLEN) {
            printf("Error: string buffer is too small!\n");
            return EXIT_FAILURE;
        }       
    }

    if (j) {
        str[j] = 0;
        updatecounts[updatecount] = read_update(str, updates[updatecount]);
        updatecount++;
    }

    long count = 0, count2 = 0;
    for (long i = 0; i < updatecount; ++i) {
        count += isvalid(updates[i], updatecounts[i], rules, rulecount);
        count2 += isvalid2(updates[i], updatecounts[i], rules, rulecount, 0);
    }

    printf("puzzle 1: %ld\n", count);
    printf("puzzle 2: %ld\n", count2);

    free(buf);
    return EXIT_SUCCESS;
}

