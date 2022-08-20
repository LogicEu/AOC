#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

static const char* key = "jlmsuwbz";

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

static uint8_t* md52016(char* str)
{
    static char buf[512];
    uint8_t* hash = md5String(str);
    sprint_hash(hash, buf);
    for (int i = 0; i < 2016; ++i) {
        free(hash);
        hash = md5String(buf);
        sprint_hash(hash, buf);
    }
    return hash;
}

static void md5Mine(void)
{
    char buf[512], ch;
    long tripleindex = 0, timer = 0, found = 0;

    for (long i = 0; found < 64; ++i) {
        
        memset(buf, 0, 512);
        sprintf(buf, "%s%ld", key, i);
        
        uint8_t *hash = md5String(buf);
        sprint_hash(hash, buf);
        
        if (timer) {
            for (int j = 0; buf[j + 4]; ++j) {
                if (ch == buf[j] && ch == buf[j + 1] && ch == buf[j + 2] && 
                    ch == buf[j + 3] && ch == buf[j + 4]) {
                    ++found;
                    timer = 1;
                    break;
                }
            }
            --timer;
            if (!timer) {
                i = tripleindex;
            }
        } else {
            for (int j = 0; buf[j + 2]; ++j) {
                if (buf[j] == buf[j + 1] && buf[j] == buf[j + 2]) {
                    tripleindex = i;
                    timer = 1000;
                    ch = buf[j];
                    break;
                }
            }
        }

        free(hash);
    }

    printf("Puzzle 1: %ld\n", tripleindex);
}

static void md5Decrypt(void)
{
    static char hashes[0xffff][64] = {0};
    static long hashcount = 0;

    char buf[512], ch;
    long tripleindex = 0, timer = 0, found = 0;

    for (long i = 0; found < 64; ++i) {
       
        memset(buf, 0, 512);
        
        if (i >= hashcount) {        
            sprintf(buf, "%s%ld", key, i);
            uint8_t *hash = md52016(buf);
            sprint_hash(hash, buf);
            strcpy(hashes[hashcount++], buf);
            free(hash);
        }
        else strcpy(buf, hashes[i]);

        if (timer) {
            
            for (int j = 0; buf[j + 4]; ++j) {
                if (ch == buf[j] && ch == buf[j + 1] && ch == buf[j + 2] && 
                    ch == buf[j + 3] && ch == buf[j + 4]) {
                    ++found;
                    //printf("%ld.- '%s'\n", found, hashes[tripleindex]);
                    timer = 1;
                    break;
                }
            }
            
            --timer;
            
            if (!timer) {
                i = tripleindex;
            }

        } else {
            for (int j = 0; buf[j + 2]; ++j) {
                if (buf[j] == buf[j + 1] && buf[j] == buf[j + 2]) {
                    tripleindex = i;
                    timer = 1000;
                    ch = buf[j];
                    break;
                }
            }
        }
    }

    printf("Puzzle 2: %ld\n", tripleindex);
}

int main(void)
{
    md5Mine();
    md5Decrypt();
    return EXIT_SUCCESS;
}
