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

typedef struct program_t {
    int weight;
    char name[16];
    char line[STRLEN];
} program_t;

static program_t progs[0xfff];
static int progcount = 0;
static const char* arrow = "->";
static const char* space = " ";

static void catch(char* str)
{
    program_t p;
    strcpy(p.line, str);
    
    char* s = strtok(str, " ");
    if (s) {
        strcpy(p.name, s);
        s = strtok(NULL, " ");
        sscanf(s, "(%d)", &p.weight);
        progs[progcount++] = p;
    }
    else printf("Error!\n");
}

static int rec(const int index)
{
    int count = 0;
    for (int i = 0; i < progcount; ++i) {
        if (i != index) {
            count += !!strstr(progs[i].line, progs[index].name);
        }
    }
    return count;
}

static int puzzle(void)
{
    for (int i = 0; i < progcount; ++i) {
        if (strstr(progs[i].line, arrow)) {
            if (!rec(i)) {
                return i + 1;
            }
        }
    }
    return 0;
}

static int calc(const int index)
{
    static int answ = 0;
    int w = progs[index].weight;
    
    char* s = strstr(progs[index].line, arrow);
    if (!s) {    
        return w;
    }

    int weights[16], indices[16], wcount = 0;
    for (int i = 0; i < progcount; ++i) {
        if (strstr(s, progs[i].name)) {
            weights[wcount] = calc(i);
            indices[wcount++] = i;
        }
    }

    int r = 0;
    for (int i = 0; i < wcount; ++i) {
        w += weights[i];
        if (!r && i + 1 < wcount && weights[i] != weights[i + 1] && 
            i + 2 < wcount && weights[i + 1] != weights[i + 2]) {
            r = i + 2;
        }
    }

    if (r && !answ) {
        --r, ++answ;
        int dif = weights[r - 1] - weights[r];
        printf("Puzzle 2: %d\n", progs[indices[r]].weight + dif);
    }

    return w;
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

    long j = 0;
    char str[STRLEN];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            catch(str);
        }
        else str[j++] = buf[i];

        if (j > STRLEN) {
            printf("Error: string buffer is too small!\n");
            return EXIT_FAILURE;
        }       
    }

    if (j) {
        str[j] = 0;
        catch(str);
    }

    int p = puzzle();
    if (p) {
        printf("Puzzle 1: %s\n", progs[--p].name);
        calc(p);
    }

    free(buf);
    return EXIT_SUCCESS;
}
