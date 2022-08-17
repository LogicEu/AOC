#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

static const char* key = "abbhdwsy";
static const char* check = "000000";

static inline void sprint_hash(const uint8_t *p, char* buf)
{
    static char buffer[4];
    sprintf(buffer, "%02x", p[0]);
    strcpy(buf, buffer);
    for (unsigned int i = 1; i < 16; ++i) {
        sprintf(buffer, "%02x", p[i]);
        strcat(buf, buffer);
    }
}

static void md5Mine(void)
{
    char buf[512], found = 0;
    char str[16] = {0};
    for (long i = 0; found < 8; ++i) {
        memset(buf, 0, 512);
        sprintf(buf, "%s%ld", key, i);
        uint8_t *hash = md5String(buf);
        sprint_hash(hash, buf);
        if (!memcmp(buf, check, 5)) {
            //printf("%s\n", buf);
            str[found++] = buf[5];
        }
        free(hash);
    }
    printf("Puzzle 1: %s\n", str);
}

static void md5Decrypt(void)
{
    char buf[512], found = 0;
    char str[16] = {0};
    for (long i = 0; found < 8; ++i) {
        memset(buf, 0, 512);
        sprintf(buf, "%s%ld", key, i);
        uint8_t *hash = md5String(buf);
        sprint_hash(hash, buf);
        if (!memcmp(buf, check, 5)) {
            //printf("%s\n", buf);
            int index = buf[5] - '0';
            if (index >= 0 && index <= 7 && !str[index]) {
                str[index] = buf[6];
                ++found;
            }
        }
        free(hash);
    }
    printf("Puzzle 2: %s\n", str);
}

int main(void)
{
    md5Mine();
    md5Decrypt();
    return EXIT_SUCCESS;
}
