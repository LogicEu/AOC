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

#define BUFFSIZE 4024000

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
  
    char buff[BUFFSIZE] = {0};
    char* s = strchr(buf, '\n');
    if (s) *s = 0;
    strcpy(buff, buf);

    char str[BUFFSIZE]; 
    
    long r1, r2;

    for (long k = 0; k < 50; ++k) { 
        
        printf("%ld\n", k + 1);
        long i = 0, j = 0;
        
        while (buff[i]) {

            long mark = i;
            while (buff[i] == buff[mark]) {
                ++i;
            }

            str[j++] = '0' + i - mark;
            str[j++] = buff[mark];
        }

        str[j] = 0;
        strcpy(buff, str);
    
        if (k + 1 == 40) {
            r1 = strlen(buff);
            printf("%ld.- '%s'\n", k + 1, buff);
        }
        else if (k + 1 == 50) {
            printf("%ld.- '%s'\n", k + 1, buff);
            r2 = strlen(buff);
        }
    }

    printf("Puzzle 1: %ld\n", r1);
    printf("Puzzle 2: %ld\n", r2);

    free(buf);
    return EXIT_SUCCESS;
}
