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

#define ITEM_COUNT 10

static char items[ITEM_COUNT][16] = {
    "children",
    "cats",
    "samoyeds",
    "pomeranians",
    "akitas",
    "vizslas",
    "goldfish",
    "trees",
    "cars",
    "perfumes"
};

static int results[ITEM_COUNT] = {3, 7, 2, 3, 0, 0, 5, 3, 2, 1};

static int item_index(const char* item)
{
    for (int i = 0; i < ITEM_COUNT; ++i) {
        if (strstr(item, items[i])) {
            return i;
        }
    }
    printf("Error! '%s'\n", item);
    return -1;
}

static int check_item(const int index, const int val)
{
    if (index == 1 || index == 7) {
        return val > results[index];
    }
    else if (index == 3 || index == 6) {
        return val < results[index];
    }
    else return val == results[index];
}

static void check_aunt(const char* str)
{
    char A[16], B[16], C[16];
    int n, a, b, c;
    sscanf(
        str,
        "Sue %d: %s %d, %s %d, %s %d",
        &n, A, &a, B, &b, C, &c
    );

    /*printf("Sue %d: %s %d, %s %d, %s %d\n",
        n, A, a, B, b, C, c
    );*/

    const int index_a = item_index(A);
    const int index_b = item_index(B);
    const int index_c = item_index(C);

    if (a == results[index_a] && b == results[index_b] && c == results[index_c]) {
        printf("Puzzle 1: %d\n", n);
    }

    if (check_item(index_a, a) && check_item(index_b, b) && check_item(index_c, c)) {
        printf("Puzzle 2: %d\n", n);
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

    char str[128];
    long j = 0, linecount = 0;
    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;

            //printf("%ld.- '%s'\n", linecount, str);
            check_aunt(str);

            ++linecount;
            memset(str, 0, 128);
        }
        else str[j++] = buf[i];
    }

    //str[j] = 0;
    //j = 0;
    //printf("%ld.- '%s'\n", linecount, str);
    //++linecount;

    free(buf);
    return EXIT_SUCCESS;
}
