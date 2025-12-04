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
#define BUFSIZE 6000

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

    char str[STRLEN];
    long r, i, j, k, n, nums[BUFSIZE], idx[BUFSIZE], indices[BUFSIZE], count = 0;

    for (i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            idx[count] = count;
            nums[count++] = (long)atol(str) * 811589153L;
        }
        else str[j++] = buf[i];

        if (j > STRLEN || count > BUFSIZE) {
            printf("Error: buffer is too small!\n");
            return EXIT_FAILURE;
        }       
    }

    if (j) {
        str[j] = 0;
        idx[count] = count;
        nums[count++] = (long)atol(str) * 811589153L;
    }

    printf("Line count: %ld\n", count);
#if 1
    for (j = 0; j < count; ++j) {
        printf("%ld, ", nums[j]);
    }
    printf("\n");
#endif

    memcpy(indices, idx, count * sizeof(long));
    for (r = 0; r < 10; ++r) {
        for (k = 0; k < count; ++k) {
            for (i = 0; indices[i] != k; ++i);
            
            n = nums[i];
            j = i + n;
            if (j >= count) {
                while (j >= count) {
                    //j = j - count + 1;
                    j = (j % count) + j / count;
                }
            } else if (j < 0) {
                while (j < 0) {
                    j += count - 1;
                }
            }
            
            //printf("%ld, %ld, %ld, %ld\n", k, n, i,  j);
            memmove(indices + i, indices + i + 1, (count - i - 1) * sizeof(long));
            memmove(nums + i, nums + i + 1, (count - i - 1) * sizeof(long));
            memmove(indices + j + 1, indices + j, (count - j) * sizeof(long));
            memmove(nums + j + 1, nums + j, (count - j) * sizeof(long));
            nums[j] = n;
            indices[j] = k;
        }
#if 1
        for (j = 0; j < count; ++j) {
            printf("%ld, ", nums[j]);
        }
        printf("\n");
#endif
    }

    for (i = 0; nums[i] != 0; ++i);

    printf("%ld, %ld, %ld\n", nums[(1000 + i) % count], nums[(2000 + i) % count], nums[(3000 + i) % count]);
    n = nums[(1000 + i) % count] + nums[(2000 + i) % count] + nums[(3000 + i) % count];
    printf("Puzzle: %ld\n", n);

    free(buf);
    return EXIT_SUCCESS;
}
