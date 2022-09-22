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

#define STRLEN 128
#define WIDTH 37
#define HEIGHT 25

typedef struct dir_t {
    int size;
    int used;
    int avail;
    int use;
} dir_t;

static dir_t grid[WIDTH * HEIGHT];
static int posx = 0;
static int posy = 0;

static void catch(const char* str)
{
    dir_t d;
    int x, y;
    sscanf(str, "/dev/grid/node-x%d-y%d %dT %dT %dT %d%%", 
            &x, &y, &d.size, &d.used, &d.avail, &d.use);
    //printf("/dev/grid/node-x%d-y%d\t%dT\t%dT\t%dT\t%d%%\n",
    //        x, y, d.size, d.used, d.avail, d.use);
    grid[x * HEIGHT + y] = d;
}

static int puzzle1(void)
{
    int paircount = 0;
    const int size = WIDTH * HEIGHT;
    for (int i = 0; i < size; ++i) {
        if (grid[i].used) {
            for (int j = 0; j < size; ++j) {
                paircount += (i != j && grid[i].used <= grid[j].avail);
            }
        }
    }
    return paircount;
}

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
    char str[STRLEN];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            if (linecount > 1) {
                catch(str);
            }
            ++linecount;
        }
        else str[j++] = buf[i];
    }

    if (j) {
        str[j] = 0;
        catch(str);
    }

    printf("Puzzle 1: %d\n", puzzle1());

    free(buf);
    return EXIT_SUCCESS;
}
