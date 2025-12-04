#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

static char* file_read(const char* filename, size_t* len)
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
    *len = size;
    return buf;
}

#define EMPTY ((size_t)-1)

static void printfs(const size_t* fs, const size_t fsize)
{
    for (size_t i = 0; i < fsize; ++i) {
        printf("%c", fs[i] == EMPTY ? '.' : (char)(fs[i] + '0'));
    }
    printf("\n");
}

static size_t compressfs(size_t* fs, const size_t fsize)
{
    size_t i, j;
   
    //printfs(fs, fsize);
    printf("%zu\n", fsize);

    for (i = 0; fs[i] != EMPTY; ++i);
    
    for (j = fsize; fs[j] == EMPTY; --j);
    if (i == j + 1) {
        return fsize;
    }
    
    fs[i] = fs[j];
    fs[j] = EMPTY;

    return compressfs(fs, j);
}

static size_t compressfs2(size_t* fs, const size_t fsize, const size_t index)
{
    size_t i, j, n, m, k;
   
    if (index == 0 || index == EMPTY) {
        return fsize;
    }

    printfs(fs, fsize);
    //printf("%zu\n", fsize);

    for (j = index; fs[j] == EMPTY; --j);
    for (m = 0; fs[j - m] == fs[j]; ++m);
    
    i = 0, n = 0;
    while (1) {
        for (; fs[i] != EMPTY; ++i);
        for (n = 0; fs[i + n] == EMPTY; ++n);

        if (n >= m) {
            break;
        } else {
            i += n;
            if (i >= index) {
                for (; fs[j - m] == EMPTY; ++m);
                return compressfs2(fs, fsize, j - m);
            }
        }
    }
    
    for (k = 0; k < m; ++k) {
        fs[i + k] = fs[j - k];
        fs[j - k] = EMPTY;
    }

    return compressfs2(fs, fsize, j - k);
}

static size_t checksum(const size_t* fs, const size_t fsize)
{
    size_t sum = 0;
    for (size_t i = 0; i < fsize; ++i) {
        if (fs[i] != EMPTY) {
            sum += fs[i] * i;
        }
    }

    return sum;
}

int main(const int argc, const char** argv)
{
    if (argc < 2) {
        printf("Missing input puzzle.\n");
        return EXIT_FAILURE;
    }

    size_t len, off = 0, fsize = 0;
    char* buf = file_read(argv[1], &len);
    if (!buf) {
        printf("Could not open file '%s'.\n", argv[1]);
        return EXIT_FAILURE;
    }

    size_t* fs = calloc(len * 10, sizeof(size_t));
    size_t* fs2 = calloc(len * 10, sizeof(size_t));

    for (size_t i = 0; i < len; ++i) {
        const int odd = i % 2;
        const size_t n = (size_t)(char)(buf[i] - '0');

        if (odd) {
            for (size_t j = 0; j < n; ++j) {
                fs[fsize++] = EMPTY;
            }
            ++off;
        } else {
            for (size_t j = 0; j < n; ++j) {
                fs[fsize++] = i - off;
            }
        }

        if (fsize > len * 10) {
            printf("Memory buffer overflow!\n");
            return -1;
        }
    }

    fs[fsize] = EMPTY;
    memcpy(fs2, fs, (fsize + 1) * sizeof(size_t));

    compressfs(fs, fsize);
    printf("puzzle 1: %zu\n", checksum(fs, fsize));

    compressfs2(fs2, fsize, fsize - 1);
    printf("puzzle 2: %zu\n", checksum(fs2, fsize));

    free(fs);
    free(buf);
    return EXIT_SUCCESS;
}
