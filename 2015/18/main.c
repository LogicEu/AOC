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

#define LEN 100
#define GRID_AT(buf,  x, y) (buf[(y) * (LEN + 1) + (x)])

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
    char* buf2 = strdup(buf), *swap;
    const size_t steps = 100;

    for (size_t i = 0; i < steps; ++i) {
 
        //Only Puzzle 2
        /*GRID_AT(buf, 0, 0) = '#';
        GRID_AT(buf, 0, LEN - 1) = '#';
        GRID_AT(buf, LEN - 1, 0) = '#';
        GRID_AT(buf, LEN - 1, LEN - 1) = '#';*/
        //Only Puzzle 2

        for (size_t y = 0; y < LEN; ++y) {
            for (size_t x = 0; x < LEN; ++x) {
                int count = 0;

                count += (x + 1 < LEN) && (y + 1 < LEN) && (GRID_AT(buf, x + 1, y + 1) == '#');
                count += (x + 1 < LEN) && (GRID_AT(buf, x + 1, y) == '#');
                count += (x + 1 < LEN) && (y > 0) && (GRID_AT(buf, x + 1, y - 1) == '#');
                count += (y > 0) && (GRID_AT(buf, x, y - 1) == '#');
                count += (x > 0) && (y > 0) && (GRID_AT(buf, x - 1, y - 1) == '#');
                count += (x > 0) && (GRID_AT(buf, x - 1, y) == '#');
                count += (x > 0) && (y + 1 < LEN) && (GRID_AT(buf, x - 1, y + 1) == '#');
                count += (y + 1 < LEN) && (GRID_AT(buf, x, y + 1) == '#');

                if (GRID_AT(buf, x, y) == '#') {
                    if (count >= 2 && count <= 3) {
                        GRID_AT(buf2, x, y) = '#';
                    } else {
                        GRID_AT(buf2, x, y) = '.';
                    }
                } else {
                    if (count == 3) {
                        GRID_AT(buf2, x, y) = '#';
                    } else {
                        GRID_AT(buf2, x, y) = '.';
                    }
                }
            }
        }
        swap = buf;
        buf = buf2;
        buf2 = swap;
    }

    //Only Puzzle 2
    /*GRID_AT(buf, 0, 0) = '#';
    GRID_AT(buf, 0, LEN - 1) = '#';
    GRID_AT(buf, LEN - 1, 0) = '#';
    GRID_AT(buf, LEN - 1, LEN - 1) = '#';*/
    //Only Puzzle 2

    size_t count = 0;
    for (size_t i = 0; buf[i]; ++i) {
        count += buf[i] == '#';
    }

    printf("Puzzle: %zu\n", count);

    free(buf2);
    free(buf);
    return EXIT_SUCCESS;
}
