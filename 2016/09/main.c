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

static long decompresslen(const char* buf)
{ 
    char str[0xffff * 0x000f];
    long j = 0;

    int a = 0, b = 0, open = 0;
    char num[8] = {0}, n = 0;
    
    for (long i = 0; buf[i]; ++i) {    
        if (open) {
            if (buf[i] == ')') {
                b = atoi(num);
                memset(num, 0, 8);
                n = 0;
                open = 0;
            }
            
            else if (buf[i] == 'x') {
                a = atoi(num);
                memset(num, 0, 8);
                n = 0;
            }
            
            else {
                num[n++] = buf[i];
            }
            
            continue;
        } 
        
        else if (a && b) {
            
            for (int k = 0; k < b; ++k) {
                memcpy(str + j, buf + i, a);
                j += a;
            }
            
            i += a - 1;
            b = 0;
            a = 0;
        }
        
        else if (buf[i] == '(') {
            ++open;
        }
        
        else {
            str[j++] = buf[i];
        }
    }

    str[j] = 0;
    return (long)strlen(str);
}

static long decomp(const char* buf)
{
    static long level = 0;;
    
    char str[0xffff];

    long j = 0, count = 0, lvl = level++;
    int a = 0, b = 0, open = 0;
    char num[8] = {0}, n = 0;
    
    const long len = strlen(buf);
    for (long i = 0; buf[i]; ++i) {
        
        /*if (!lvl) {
            printf("%% %.02lf ...\n", ((double)i / (double)len) * 100.0);
        }*/

        if (open) {
            
            if (buf[i] == ')') {
                b = atoi(num);
                memset(num, 0, 8);
                n = 0;
                open = 0;
            }
            
            else if (buf[i] == 'x') {
                a = atoi(num);
                memset(num, 0, 8);
                n = 0;
            }
            
            else {
                num[n++] = buf[i];
            }
            
            continue;
        } 
        
        else if (a && b) {
            
            for (int k = 0; k < b; ++k) {
                memcpy(str + j, buf + i, a);
                j += a;
            }
            
            str[j] = 0;
            j = 0;
            count += decomp(str); 

            i += a - 1;
            b = 0;
            a = 0;
        }
        
        else if (buf[i] == '(') {
            ++open;
        }
        
        else {
            ++count;
        }
    }

    return count;
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

    // Puzzle
    printf("Puzzle 1: %zu\n", decompresslen(buf));
    printf("Puzzle 2: %ld\n", decomp(buf));

    free(buf);
    return EXIT_SUCCESS;
}
