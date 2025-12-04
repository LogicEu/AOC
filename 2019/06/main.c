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

#define STRLEN 16
#define STARLEN 1024

typedef struct star_t {
    char name[STRLEN];
    char parent[STRLEN];
} star_t;

static star_t star_create(char* s)
{
    char* p = strchr(s, ')');
    if (!p) {
        printf("Failed!\n");
        exit(EXIT_FAILURE);
    }
    *p++ = 0;
    star_t star;
    strcpy(star.name, p);
    strcpy(star.parent, s);
    return star;
}

static long stars_search(const star_t* stars, const char* name, const long starcount)
{
    for (long i = 0; i < starcount; ++i) {
        if (!strcmp(stars[i].name, name)) {
            return i + 1;
        }
    }
    return 0;
}

static long star_orbit(const star_t* stars, long i, const long starcount)
{
    long find, count = 1;
    do {
        find = stars_search(stars, stars[i].parent, starcount);
        count += !!find;
        i = find - 1;
    } while (find);
    return count;
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

    long i, j = 0, starcount = 0, orbits = 0;
    char str[STRLEN];
    star_t stars[STARLEN];
    for (i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            stars[starcount++] = star_create(str);
        }
        else str[j++] = buf[i];

        if (j > STRLEN || starcount > STARLEN) {
            printf("Error: string buffer is too small!\n");
            return EXIT_FAILURE;
        }       
    }

    if (j) {
        str[j] = 0;
        stars[starcount++] = star_create(str);
    }

    for (i = 0; i < starcount; ++i) {
        long n = star_orbit(stars, i, starcount);
        printf("%ld\n", n);
        orbits += n;
    }

    printf("Puzzle 1: %ld\n", orbits);

    const long you = stars_search(stars, "YOU", starcount);
    const long san = stars_search(stars, "SAN", starcount);
    if (!you || !san) {
        printf("Problem!\n");
        return EXIT_FAILURE;
    }

    long acount = 0, scount = 0, brk = 0;
    char ancesters[STARLEN][STRLEN];
    long found = you;
    do {
        --found;
        strcpy(ancesters[acount++], stars[found].parent);
        found = stars_search(stars, stars[found].parent, starcount);
    } while (found);

    found = san;
    do {
        --found;
        ++scount;
        for (i = 0; i < acount; ++i) {
            if (!strcmp(ancesters[i], stars[found].parent)) {
                brk = i + 1;
                break;
            }
        }
        found = stars_search(stars, stars[found].parent, starcount);
    } while (found && !brk);

    printf("Puzzle 2: %ld\n", brk + scount - 2);

    free(buf);
    return EXIT_SUCCESS;
}
