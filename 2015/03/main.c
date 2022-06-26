#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* file_read(const char* filename)
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

typedef struct vec2 {
    long x;
    long y;
} vec2;

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("Missing input puzzle.\n");
        return EXIT_FAILURE;
    }

    char* buf = file_read(argv[1]);
    if (!buf) {
        printf("Could not open file '%s'.\n", argv[1]);
    }

    // Puzzle
    
    const long size = strlen(buf);
    vec2 path[size];
    long nodes = 0;
    
    vec2 p = {0, 0};
 
    for (long i = 0; i < size; ++i) {
        
        int found = 0;
        for (long j = 0; j < nodes; ++j) {
            if (!memcmp(path + j, &p, sizeof(vec2))) {
                ++found;
                break;
            }
        }

        if (!found) {
            path[nodes++] = p;
        }

        if (buf[i] == '^') ++p.y;
        else if (buf[i] == 'v') --p.y;
        else if (buf[i] == '>') ++p.x;
        else if (buf[i] == '<') --p.x;
    }

    printf("PUZZLE 1: %ld\n", nodes);

    // Puzzle 2
    
    nodes = 0;
    memset(path, 0, size * sizeof(vec2));

    vec2 current[2] = {
        {0, 0},
        {0, 0}
    };

    for (long i = 0; i < size; ++i) {
        
        const int on = i % 2;
        
        int found = 0;
        for (long j = 0; j < nodes; ++j) {
            if (!memcmp(path + j, current + on, sizeof(vec2))) {
                ++found;
                break;
            }
        }

        if (!found) {
            path[nodes++] = current[on];
        }

        if (buf[i] == '^') ++current[on].y;
        else if (buf[i] == 'v') --current[on].y;
        else if (buf[i] == '>') ++current[on].x;
        else if (buf[i] == '<') --current[on].x;
    }

    printf("PUZZLE 2: %ld\n", nodes);

    free(buf);
    return EXIT_SUCCESS;
}
