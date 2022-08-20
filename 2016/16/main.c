#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define byteat(i) ((i) / 8)
#define bitat(i) ((i) % 8)
#define bitset(buf, i, n) buf[byteat(i)] |= (n) << bitat(i)
#define bitget(buf, i) ((buf[byteat(i)] >> bitat(i)) & 1)
#define bitclear(buf, i) buf[byteat(i)] &= ~(1UL << bitat(i))

static const char* input = "01000100010010111";

static inline void printbin(uint8_t* buf, const size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        printf("%c", bitget(buf, i) + '0');
    }
    printf("\n");
}

static inline int strtoint(const char* str)
{
    int n = 0;
    for (int i = 0; str[i]; ++i) {
        n |= (str[i] - '0') << i;
    }
    return n;
}

static inline size_t proc(uint8_t* buf, const size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        const size_t frontindex = len + 1 + i;
        const size_t backindex = len - 1 - i;
        bitset(buf, frontindex, !bitget(buf, backindex));
    }
    return len * 2 + 1;
} 

static inline void checksum(uint8_t* buf, size_t len)
{
    while (!(len % 2)) {
        for (size_t i = 0; i < len; i += 2) {
            const size_t hindex = i / 2;
            const int n = (bitget(buf, i) == bitget(buf, i + 1));
            bitclear(buf, hindex);
            bitset(buf, hindex, n);
        }
        len /= 2;
    }
    printf("Puzzle 1: ");
    printbin(buf, len);
}   

static inline void calc(uint8_t* buf, const size_t fill, size_t len)
{
    while (len < fill) {
        len = proc(buf, len);
    }
    checksum(buf, fill);
}

static size_t strsize;

static inline char fnum(const size_t index, size_t len)
{
    if (index < strsize) {
        return input[index];
    }
    
    if (index == len) {
        return '2';
    }

    while (len > index) {
        len = (len - 1) / 2;
    }
    
    size_t dif = len - (index - len);
    return '0' + (fnum(dif, len) == '0');
}

static char num(const size_t i, const size_t lvl)
{
    if (lvl) {
        return '0' + (num(i * 2, lvl - 1) == num(i * 2 + 1, lvl - 1));
    }

    size_t len = strsize;
    while (len < i) {
        len += len + 1;
    }

    return fnum(i, len);
}

static inline void nth(const size_t fill)
{
    size_t len = fill, n = 0;
    while (!(len % 2)) {
        len /= 2;
        ++n;
    }
        
    char output[32] = {0};
    for (size_t i = 0; i < len; ++i) {
        output[i] = num(i, n);
    }

    printf("Puzzle 2: %s\n", output);
}

int main(void)
{
    uint8_t buf[0xfffff];
    const size_t len = strlen(input);
    const int n = strtoint(input);

    memset(buf, 0, 0xfffff);
    memcpy(buf, &n, sizeof(int));
    calc(buf, 272, len);

    strsize = len;
    nth(35651584);
    return EXIT_SUCCESS;
}
