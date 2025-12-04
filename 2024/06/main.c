#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

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

#define STRLEN 130
#define VECCOUNT 16900

struct ivec2 {
    long x, y;
};

int main(const int argc, const char** argv)
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

    long j = 0, linecount = 0;
    char grid[STRLEN][STRLEN], grid2[STRLEN][STRLEN], str[STRLEN];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            strcpy(grid[linecount++], str);
        }
        else str[j++] = buf[i];

        if (j > STRLEN) {
            printf("Error: string buffer is too small!\n");
            return EXIT_FAILURE;
        }       
    }

    if (j) {
        str[j] = 0;
        strcpy(grid[linecount++], str);
    }

    int dirindex = -1;
    const struct ivec2 dirs[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    struct ivec2 pos;
    
    for (long y = 0; y < linecount && dirindex == -1; ++y) {
        for (long x = 0; x < linecount; ++x) {
            if (grid[y][x] == '<' || grid[y][x] == '^' || 
                grid[y][x] == 'v' || grid[y][x] == '>') {
                pos.x = x;
                pos.y = y;
                switch (grid[y][x]) {
                    case '>':
                        dirindex = 0;
                        break;
                    case 'v':
                        dirindex = 1;
                        break;
                    case '<':
                        dirindex = 2;
                        break;
                    case '^':
                        dirindex = 3;
                        break;
                }

                break;
            }        
        }
    }

    long veccount, poscount = 0, count = 0;
    const int di = dirindex;
    const struct ivec2 op = pos;
    struct ivec2 positions[VECCOUNT], vecs[VECCOUNT][2];
    
    memcpy(grid2, grid, STRLEN * STRLEN);

    while (pos.x >= 0 && pos.y >= 0 && pos.x < linecount && pos.y < linecount) {
        const struct ivec2 p = {pos.x + dirs[dirindex].x, pos.y + dirs[dirindex].y};
        if (grid[p.y][p.x] == '#') {
            dirindex = (dirindex + 1) % 4;
        } else {
            int found = 0;
            for (long i = 0; i < poscount; ++i) {
                if (pos.x == positions[i].x && pos.y == positions[i].y) {
                    ++found;
                    break;
                }
            }

            if (!found) {
                positions[poscount++] = pos;
                if (poscount > VECCOUNT) {
                    printf("Position count is too high!\n");
                    return 1;
                }
            }
            
            pos = p;
        }
    }

    printf("puzzle 1: %ld\n", poscount);

    for (long i = 1; i < poscount; ++i) {
        
        printf("%ld\n", i);
        
        memcpy(grid, grid2, STRLEN * STRLEN);
        grid[positions[i].y][positions[i].x] = '#';
        pos = op;
        dirindex = di;
        veccount = 0;

        while (pos.x >= 0 && pos.y >= 0 
            && pos.x < linecount && pos.y < linecount) {
            const struct ivec2 d = dirs[dirindex];
            const struct ivec2 p = {pos.x + d.x, pos.y + d.y};

            if (grid[p.y][p.x] == '#') {
                dirindex = (dirindex + 1) % 4;
            } else {
                
                int found = 0;
                for (long j = 0; j < veccount; ++j) {
                    if (vecs[j][0].x == pos.x && vecs[j][0].y == pos.y &&
                        vecs[j][1].x == d.x && vecs[j][1].y == d.y) {
                        ++found;
                        break;
                    }
                }

                if (found) {
                    ++count;
                    break;
                }

                vecs[veccount][0] = pos;
                vecs[veccount++][1] = d;
                pos = p;

                if (veccount > VECCOUNT) {
                    printf("Vector count is to high!\n");
                    return 1;
                }
            }
        } 
    }

    printf("puzzle 2: %ld\n", count);

    free(buf);
    return EXIT_SUCCESS;
}

