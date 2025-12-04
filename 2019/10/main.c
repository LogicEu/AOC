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

#define fabs(f) (((f) >= 0.0) : (f) : -(f))

typedef struct vec2 {
    float x, y;
} vec2;

static inline float slope(const vec2 a, const vec2 b)
{
    const float x = b.x - a.x;
    const float y = b.y - a.y;
    return y / x;
}

static inline int signf(const void* ptr)
{
    const char* data = ptr;
    return (data[3] >> 7) & 1;
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

#define LEN 30

    vec2 arr[LEN * LEN];
    long arrcount = 0, x = 0, y = 0, i, j, k;
    for (i = 0; buf[i]; ++i) {
        if (buf[i] == '#') {
            printf("In: %ld, %ld\n", x, y);
            arr[arrcount++] = (vec2){(float)x++, (float)y};
        }
        else if (buf[i] == '\n') {
            ++y;
            x = buf[i + 1] ? 0 : x;
        }
        else ++x;
    }

#if 1 
    const long w = x, h = y + (buf[i - 1] != '\n');
    for (y = 0; y < h; ++y) {
        for (x = 0; x < w; ++x) {
            int found = 0;
            for (k = 0; k < arrcount; ++k) {
                if (arr[k].x == (float)x && arr[k].y == (float)y) {
                    ++found;
                    break;
                }
            }
            printf("%c", found ? '#' : '.');
        }
        printf("\n");
    }
#endif

    long slopecount, max = -10000000;
    float slopes[LEN * LEN];
    for (i = 0; i < arrcount; ++i) {
        slopecount = 0;
        printf("%f, %f\n", arr[i].x, arr[i].y);
        for (j = 0; j < arrcount; ++j) {
            if (i != j) {
                const float s = slope(arr[i], arr[j]);
                int found = 0;
                for (k = 0; k < slopecount; ++k) {
                    if (s == slopes[k]) {
                        if (s == 0.0 && signf(&s) != signf(slopes + k)) {
                            continue;
                        }
                        ++found;
                        break;
                    }
                }
                if (!found) {
                    slopes[slopecount++] = s;
                    printf("%ld -> %f\n", slopecount, s);
                }
            }
        }
        if (slopecount > max) {
            max = slopecount;
        }
        //printf("%ld: %d, %d\n", slopecount, (int)arr[i].x, (int)arr[i].y);
    }
    
    printf("Puzzle 1: %ld\n", max);

    free(buf);
    return EXIT_SUCCESS;
}
