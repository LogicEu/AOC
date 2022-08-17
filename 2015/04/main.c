#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

static const char* key = "ckczppom";
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

static long md5Mine(const size_t zero_count)
{
    char buf[512], found = 0;
    long i;
    for (i = 1; !found; ++i) {
        memset(buf, 0, 512);
        sprintf(buf, "%s%ld", key, i);
        uint8_t *hash = md5String(buf);
        sprint_hash(hash, buf);
        found = !memcmp(buf, check, zero_count);
        free(hash);
    }
    return --i;
}

int main(void)
{
    long n = md5Mine(5);
    printf("Puzzle 1: %ld\n", n);
    
    n = md5Mine(6);
    printf("Puzzle 2: %ld\n", n);
    
    return EXIT_SUCCESS;
}
