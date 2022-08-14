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

    int charcount = 0;
    char characters[56][16];
    memset(characters, 0, 56 * 16);

    int j = 0, line_count = 1;
    char line[64] = {0};
    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n' || !buf[i + 1]) {
            line[j] = 0;
            j = 0;

            printf("%d.- '%s'\n", line_count++, line);

            int n;
            char ch1[16], ch2[16], op[16];

            sscanf(
                line, 
                "%s would %s %d happiness units by sitting next to %s.",
                ch1,
                op,
                &n,
                ch2
            );
            
            memset(line, 0, 64);
        }
        else line[j++] = buf[i];
    }

    free(buf);
    return EXIT_SUCCESS;
}
