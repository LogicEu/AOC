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

#define STRLEN 128
#define DIST(a, b) (((a) < (b)) ? ((b) - (a)) : ((a) - (b)))

static void puzzle(const char* str, const long line, long* nums1, long* nums2)
{
    sscanf(str, "%ld %ld", nums1 + line, nums2 + line);
}

static void sort(long* nums, const long size)
{
    for (long i = 0; i < size - 1; ++i) {
        for (long j = 0; j < size - i - 1; ++j) {
            if (nums[j] > nums[j + 1]) {
                long n = nums[j];
                nums[j] = nums[j + 1];
                nums[j + 1] = n;
            }
        }
    }
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

    long j = 0, linecount = 0;
    char str[STRLEN];
    long nums1[1000], nums2[1000];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            puzzle(str, linecount++, nums1, nums2);
        }
        else str[j++] = buf[i];

        if (j > STRLEN) {
            printf("Error: string buffer is too small!\n");
            return EXIT_FAILURE;
        }       
    }

    if (j) {
        str[j] = 0;
        puzzle(str, linecount++, nums1, nums2);
    }

    sort(nums1, linecount);
    sort(nums2, linecount);

    long dist = 0, score = 0;
    for (long i = 0; i < linecount; ++i) {
        long d = DIST(nums1[i], nums2[i]), n = 0;
        dist += d;

        for (long j = 0; j < linecount; ++j) {
            n += nums1[i] == nums2[j];
        }
        
        score += nums1[i] * n;
    }

    printf("puzzle 1: %ld\n", dist);
    printf("puzzle 2; %ld\n", score);

    free(buf);
    return EXIT_SUCCESS;
}
