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

#define NUM(ch) (((ch >= '0') && (ch <= '9')) || (ch == '-'))

static long sum(const char* buf) 
{
    long count = 0;
    char numstr[8] = {0};
    for (long i = 0; buf[i]; ++i) {
        if (NUM(buf[i])) {
            
            long nummark = i;
            while (NUM(buf[i])) {
                ++i;
            }
            
            memcpy(numstr, buf + nummark, i - nummark);
            numstr[i - nummark] = 0;
            count += (long)atoi(numstr);
        
        }
    }
    return count;
}

static long innersum(const char* buf, const char c, long* i)
{
    int red = 0;
    long count = 0;
    char numstr[8];
    
    for (; buf[*i]; ++(*i)) {
    
        const long n = *i; 
        //printf("%c", buf[n]);

        if (NUM(buf[n])) {
            while(NUM(buf[*i])) {
                ++(*i);
                if (NUM(buf[*i])) {
                    //printf("%c", buf[*i]);
                }
            }
            
            memcpy(numstr, buf + n, *i - n);
            numstr[*i - n] = 0;
            count += (long)atoi(numstr);

            --(*i);
        }
        
        else if (buf[n] == '{' || buf[n] == '[') {
            ++(*i);
            count += innersum(buf, buf[n], i);
        }
        
        else if (buf[n] == '}' || buf[n] == ']') {
            //++(*i);
            break;
        }
        
        else if (!memcmp(buf + n, "\"red\"", 5) && c == '{') {
            //printf("red\"");
            ++red;
            *i += 4;
        }

    }
    
    return count * !red;
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

    // Puzzle 1

    long count = sum(buf);
    printf("Puzzle 1: %ld\n", count);

    // Puzzle 2
    long i = 0;
    count = innersum(buf, 0, &i);
    //printf("\n");
    printf("Puzzle 2: %ld\n", count);

    free(buf);
    return EXIT_SUCCESS;
}
