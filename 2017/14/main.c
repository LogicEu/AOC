#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define LEN 128
#define STRLEN 256
#define swap(a, b, type) do { type tmp = a; a = b; b = tmp; } while (0)

typedef struct hash_t {
    uint8_t buf[16];
} hash_t;

static const char hextable[] = "0123456789abcdef";

static void hextobin(const char* hex, char* bin)
{
    int i, j, n;
    for (i = 0; hex[i]; ++i) {
        char* s = strchr(hextable, hex[i]);
        if (s) {
            n = s - hextable;
            for (j = 3; j >= 0; --j) {
                bin[i * 4 + 3 - j] = ((n >> (j)) & 1) + '0';
            } 
        }
    }
    bin[i * 4] = 0;
}

static void bin(const uint8_t* src, char* str, const int len)
{
    int n = 0;
    for (int i = len - 1; i >= 0; --i) {
        for (int  j = 7; j >= 0; --j) {
            str[n++] = ((src[i] >> j) & 1) + '0';
        }
    }
    str[n] = 0;
}

static void hex(const uint8_t* src, char* str, const int len)
{
    for (int i = 0; i < len; ++i) {
        str[i * 2 + 0] = hextable[src[i] >> 4];
        str[i * 2 + 1] = hextable[src[i] & 0x0f];
    }
    str[len * 2] = 0;
}

static void reverse(uint8_t* array, const int from, const int len)
{
    for (int i = 0; i < len / 2; ++i) {
        int n = (from + i) % STRLEN;
        int m = (from + len - i - 1) % STRLEN;
        swap(array[n], array[m], int);
    }
}

static hash_t hash(const char* str)
{
    int i, j, s = 0, p = 0;
    char cpy[STRLEN], extra[] = {17, 31, 73, 47, 23, 0};
    strcpy(cpy, str);
    strcat(cpy, extra);

    uint8_t buf[STRLEN];
    for (int i = 0; i < STRLEN; ++i) {
        buf[i] = (uint8_t)i;
    }

    for (j = 0; j < 64; ++j) {
        for (i = 0; cpy[i]; ++i) {
            reverse(buf, p, cpy[i]);
            p = (p + s + cpy[i]) % STRLEN;
            ++s;
        }
    }
    
    hash_t h;
    for (i = 0; i < 16; ++i) {
        h.buf[i] = buf[i * 16];
        for (j = 1; j < 16; ++j) {
            h.buf[i] ^= buf[i * 16 + j];
        }
    }

    return h;
}

static void rec(char** grid, const int x, const int y, const int val)
{
    grid[y][x] = val;
    if (y + 1 < LEN && grid[y + 1][x] == '1') {
        rec(grid, x, y + 1, val);
    }
    if (y > 0 && grid[y - 1][x] == '1') {
        rec(grid, x, y - 1, val);
    }
    if (x + 1 < LEN && grid[y][x + 1] == '1') {
        rec(grid, x + 1, y, val);
    }
    if (x > 0 && grid[y][x - 1] == '1') {
        rec(grid, x - 1, y, val);
    }
}

static void puzzle(const char* str)
{
    int count = 0;
    char** buf = malloc(sizeof(char*) * LEN), b[STRLEN];

    for (int i = 0; i < LEN; ++i) {
        sprintf(b, "%s-%d", str, i);
        hash_t h = hash(b);
        hex(h.buf, b, sizeof(hash_t));
        
        buf[i] = malloc(LEN + 1);
        hextobin(b, buf[i]);
        
        for (int j = 0; j < LEN; ++j) {
            count += buf[i][j] == '1';
        }
    }

    printf("Puzzle 1: %d\n", count);

    count = 0;
    for (int i = 0; i < LEN; ++i) {
        for (int j = 0; j < LEN; ++j) {
            if (buf[i][j] == '1') {
                rec(buf, j, i, 'x');
                ++count;
            }
        }
    }
    
    printf("Puzzle 2: %d\n", count);
    for (int i = 0; i < LEN; ++i) {
        free(buf[i]);
    }
    free(buf);
}

int main(const int argc, const char** argv)
{
    if (argc < 2) {
        printf("Missing input puzzle.\n");
        return EXIT_FAILURE;
    }
    
    puzzle(argv[1]);

    return EXIT_SUCCESS;
}
