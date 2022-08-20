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

typedef struct bot_t {
    int low;
    int high;
    int slots[2];
    int used;
} bot_t;

static bot_t bots[256] = {0};
static int output[256] = {0};

static void catch(const char* str)
{
    char str[2][8];
    int bot, val, n[2];
    if (strstr(str, "value ")) {
        sscanf(str, "value %d goes to bot %d", &val, &bot);
        bots[bot].slots[bots[bot].used++] = val;
    }
    else if (strstr(str, " gives low to ")) {
        sscanf(str, "bot %d gives low to %s %d and high to %s %d", 
            &bot, str[0], n, str[1], n + 1
        );

        for (int i = 0; i < 2; ++i) {
            if (strstr(str[i], "output")) {
                output[n[i]] = bots[bot].slots[i];
                bots[bot].slots[i] = 0;
                --bots[bot].used;
            }
            else {
                bots[n[i]] = bots[bot].slots[i];
                bots[bot].slots[i] = 0;
                --bots[bot].used;
                ++bots[n[i]].used;
            }
        }
    }
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

    memset(bots, 0, sizeof(bot_t) * 256);

    long j = 0, linecount = 0;
    char str[128];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            //printf("%s\n", str);
            catch(str);
            ++linecount;
        }
        else str[j++] = buf[i];
    }

    if (j) {
        str[j] = 0;
        //printf("%s\n", str);
        catch(str);
        ++linecount;
    }

    free(buf);
    return EXIT_SUCCESS;
}
