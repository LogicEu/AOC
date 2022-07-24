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

typedef struct road_t {
    char* city1;
    char* city2;
    long dist;
} road_t;

static road_t roads[28];
static long roadcount = 0;
static long citycount = 0;
static char cities[8][16];
static long MIN = 9999999;
static long MAX = 0;

static inline void swap(long* restrict a, long* restrict b)
{
    long temp = *a;
    *a = *b;
    *b = temp;
}

static long cityfind(const char* str)
{
    const long count = citycount;
    for (long i = 0; i < count; ++i) {
        if (!strcmp(cities[i], str)) {
            return i;
        } 
    }
    return -1;
}

static long finddist(const char* city1, const char* city2)
{
    const long count = roadcount;
    for (long i = 0; i < count; ++i) {
        if ((!strcmp(city1, roads[i].city1) && !strcmp(city2, roads[i].city2)) ||
            (!strcmp(city2, roads[i].city1) && !strcmp(city1, roads[i].city2))) {
            return roads[i].dist;
        }
    }

    printf("Find Dist Func Error!\n");
    return 0;
}

static long calcdist(const long *indexes)
{
    const long len = citycount - 1;
    long count = 0;
    for (long i = 0; i < len; ++i) {
        count += finddist(cities[indexes[i] - 1], cities[indexes[i + 1] - 1]);
    }
    return count;
}

static void printlist(const long* indexes, const long count)
{
    for (long i = 0; i < count; ++i) {
        printf("%ld ", indexes[i]);
    }
    printf("\n");
}

static void permute(long* indexes, const long count, const long k)
{
    static long level = -1;
    indexes[k] = ++level;

    if (level == count) {
        printlist(indexes, count);
        long n = calcdist(indexes);
        MIN = (n < MIN) ? n : MIN;
        MAX = (n > MAX) ? n : MAX;
    }
    else {
        for (long i = 0; i < count; ++i) {
            if (indexes[i] == 0) {
                permute(indexes, count, i);
            }
        }
    }
    --level;
    indexes[k] = 0;
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
    
    long j = 0, lines = 0;
    char str[128] = {0};
    
    long len;      
    char city1[16], city2[16];
    long c1, c2;

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            ++lines;

            sscanf(str, "%s to %s = %ld", city1, city2, &len);

            c1 = cityfind(city1);
            if (c1 == -1) {
                strcpy(cities[citycount++], city1);
                c1 = citycount - 1;
            }

            c2 = cityfind(city2);
            if (c2 == -1) {
                strcpy(cities[citycount++], city2);
                c2 = citycount - 1;
            }

            roads[roadcount].city1 = cities[c1];
            roads[roadcount].city2 = cities[c2];
            roads[roadcount].dist = len;
            ++roadcount;

            printf("'%s' [%s] -> [%s] = %ld \n", str, city1, city2, len);
        }
        else str[j++] = buf[i];
    }

    str[j] = 0;
    ++lines;
    
    sscanf(str, "%s to %s = %ld", city1, city2, &len);
    
    c1 = cityfind(city1);
    if (c1 == -1) {
        strcpy(cities[citycount++], city1);
        c1 = citycount - 1;
    }

    c2 = cityfind(city2);
    if (c2 == -1) {
        strcpy(cities[citycount++], city2);
        c2 = citycount - 1;
    }
    
    roads[roadcount].city1 = cities[c1];
    roads[roadcount].city2 = cities[c2];
    roads[roadcount].dist = len;
    ++roadcount;

    printf("'%s'\n", str);
    printf("Lines: %ld\n%ld\n", lines, citycount);

    for (long i = 0; i < citycount; ++i) {
        printf("City %ld: '%s'\n", i + 1, cities[i]);
    }

    for (long i = 0; i < roadcount; ++i) {
        printf("%s to %s = %ld\n", roads[i].city1, roads[i].city2, roads[i].dist);
    }

    long indexes[citycount];
    memset(indexes, 0, citycount * sizeof(long));
    permute(indexes, citycount, 0);
    
    printf("Puzzle 1: %ld\n", MIN);
    printf("Puzzle 2: %ld\n", MAX);

    free(buf);
    return EXIT_SUCCESS;
}
