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

typedef struct ivec2 {
    int x;
    int y;
} ivec2;

static inline void ivec2_clamp(ivec2* v)
{
    if (v->x > 1) v->x = 1;
    if (v->y > 1) v->y = 1;
    if (v->x < -1) v->x = -1;
    if (v->y < -1) v->y = -1;
}

static int ivec2_table(const ivec2 v)
{
    static const int table[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    return table[(v.y + 1) * 3 + (v.x + 1)];
}

static int button(const char* str)
{
    static ivec2 mem = {0, 0};
    ivec2 p = mem;
    
    for (int i = 0; str[i]; ++i) {
        switch (str[i]) {
            case 'U': {
                --p.y;
                break;
            }
            case 'D': {
                ++p.y;
                break;
            }
            case 'R': {
                ++p.x;
                break;
            }
            case 'L': {
                --p.x;
                break;
            }
            default: break;
        }
        ivec2_clamp(&p);
    }
    mem = p;
    return ivec2_table(p);
}

static char button2(const char* str)
{    
    static char table[5][5] = {
        {0,    0,  '1',  0,   0 },
        {0,   '2', '3', '4',  0 },
        {'5', '6', '7', '8', '9'},
        {0,   'A', 'B', 'C',  0 },
        {0,    0,  'D',  0,   0 }
    };
    
    static ivec2 p = {0, 2};

    for (int i = 0; str[i]; ++i) {
        switch (str[i]) {
            case 'U': {
                if (p.y > 0 && table[p.y - 1][p.x]) {
                    --p.y;
                }
                break;
            }
            case 'D': {
                if (p.y < 4 && table[p.y + 1][p.x]) {
                    ++p.y;
                }
                break;
            }
            case 'R': {
                if (p.x < 4 && table[p.y][p.x + 1]) {
                    ++p.x;
                }
                break;
            }
            case 'L': {
                if (p.x > 0 && table[p.y][p.x - 1]) {
                    --p.x;
                }
                break;
            }
            default: {
                printf(".\n");
                break;
            }
        }
    }

    return table[p.y][p.x];
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
    int buttons[5];
    char buttons2[5];

    long j = 0, linecount = 0;
    char str[1024];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            buttons[linecount] = button(str);
            buttons2[linecount++] = button2(str);
        }
        else str[j++] = buf[i];
    }

    str[j] = 0;
    buttons[linecount] = button(str);
    buttons2[linecount++] = button2(str);

    printf("Puzzle 1: ");
    for (int i = 0; i < linecount; ++i) {
        printf("%d", buttons[i]);
    }
    printf("\n");

    printf("Puzzle 2: ");
    for (int i = 0; i < linecount; ++i) {
        printf("%c", buttons2[i]);
    }
    printf("\n");

    free(buf);
    return EXIT_SUCCESS;
}
