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

#define COUNT 8
#define LEN 16

static int len = 0;
static int max = 0;
static char chars[COUNT][LEN] = {0};
static char conect[COUNT][COUNT - 1][LEN] = {0};
static int table[COUNT][COUNT - 1] = {0};

static inline void swap(char* a, char* b)
{
    char tmp = *a;
    *a = *b;
    *b = tmp;
}

static inline int find(const int index, const int index2)
{
    if (index != COUNT && index2 != COUNT) {
        for (int i = 0; i < COUNT - 1; ++i) {
            if (strstr(conect[index][i], chars[index2])) {
                return table[index][i];
            }
        }
    }
    return 0;
}

static int calc(const char* array)
{
    int sum = 0;
    for (int i = 0; i < len; ++i) {
        const int index = (int)(array[i] - '0');
        const int prev = (int)(array[(i - 1) * (i > 0) + (len - 1) * !i] - '0');
        const int next = (int)(array[(i + 1) % len] - '0');
        sum += find(index, prev);
        sum += find(index, next);
    }
    return sum;
}

static void permuteimpl(char* array, const int l, const int r)
{
    if (r == l) {
        int n = calc(array);
        if (n > max) {
            max = n;
            //printf("%s -> %d\n", array, n);
        }
    } else {
        for (int i = l; i <= r; i++) {
            swap(array + l, array + i);
            permuteimpl(array, l + 1, r);
            swap(array + l, array + i);
        }
    }
}

static inline void permute(char* array)
{
    len = strlen(array);
    return permuteimpl(array, 0, len - 1);
}

static void catch(const char* str, const int linecount)
{
    const int div = linecount / (COUNT - 1);
    const int mod = linecount % (COUNT - 1);

    int n;
    char name[LEN], name2[LEN], op[LEN];
    
    sscanf(
        str,
        "%s would %s %d happiness units by sitting next to %s",
        name, 
        op, 
        &n, 
        name2
    );

    if (!mod) {
        strcpy(chars[div], name);
    }

    strcpy(conect[div][mod], name2); 
    table[div][mod] = n * (-(op[0] == 'l') + (op[0] != 'l'));
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

    // Puzzle 1
    int j = 0, linecount = 0;
    char str[128];
    for (int i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            catch(str, linecount++);
        }
        else str[j++] = buf[i];
    }
    
    str[j] = 0;
    catch(str, linecount++);

    char perm[COUNT + 2] = "01234567";
    permute(perm);
    printf("Puzzle 1: %d\n", max);

    max = -10000;
    strcpy(perm, "012345678");
    permute(perm);
    printf("Puzzle 2: %d\n", max);

    free(buf);
    return EXIT_SUCCESS;
}
