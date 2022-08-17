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

ivec2 ivec2_dir(const int index)
{
    ivec2 v = {0, 0};
    switch (index) {
        case 0: {
            v.y = 1;
            break;
        }
        case 1: {
            v.x = 1;
            break;
        }
        case 2: {
            v.y = -1;
            break;
        }
        case 3: {
            v.x = -1;
            break;
        }
        default: break;
    }
    return v;
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

    const int size = strlen(buf);

    ivec2 p = {0, 0}, arr[10000];
    int dir = 0, arrcount = 1, found = 0;
    arr[0] = p;

    char number[8], n = 0;
    for (int i = 0; i < size; ++i) {
        int sign = (buf[i] == 'R') - (buf[i] == 'L');
        if (!sign) {
            continue;
        }

        printf("%d, %d\n", p.x, p.y);
        ++i;
        while (buf[i] >= '0' && buf[i] <= '9') {
            number[n++] = buf[i++];
        }
    
        number[n] = 0;
        n = 0;
        
        const int num = atoi(number);
        
        dir += sign;
        if (dir > 3) dir = 0;
        if (dir < 0) dir = 3;
        ivec2 v = ivec2_dir(dir);

        for (int j = 0; j < num; ++j) {
            
            p.x += v.x;
            p.y += v.y;           
            
            if (!found) {
                for (int k = 0; k < arrcount; ++k) {
                    if (!memcmp(arr + k, &p, sizeof(ivec2))) {
                        found = k;
                        break;
                    }
                }

                if (!found) {
                    arr[arrcount++] = p;
                }
            }
        }
    }

    if (p.x < 0) p.x = -p.x;
    if (p.y < 0) p.y = -p.y;

    if (arr[found].x < 0) arr[found].x = -arr[found].x;
    if (arr[found].y < 0) arr[found].y = -arr[found].y;

    printf("Index: %d\n", found);
    printf("Puzzle 1: %d\n", p.x + p.y);
    printf("Puzzle 2; %d\n", arr[found].x + arr[found].y);

    free(buf);
    return EXIT_SUCCESS;
}
