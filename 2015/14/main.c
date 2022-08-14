#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

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

typedef struct raindeer_t {
    char name[16];
    int speed;
    int runtime;
    int resttime;
} raindeer_t;

static raindeer_t raindeer_create(const char* str)
{
    raindeer_t raindeer; 

    sscanf(
        str, 
        "%s can fly %d km/s for %d seconds, but then must rest for %d seconds.",
        raindeer.name,
        &raindeer.speed,
        &raindeer.runtime,
        &raindeer.resttime
    );

    return raindeer;
}

static int raindeer_race(const raindeer_t* raindeer, const int seconds)
{
    bool run_or_rest = true;
    int timer = raindeer->runtime;
    int km = 0;
    
    for (int i = 0; i < seconds; ++i) {
        
        if (run_or_rest) {
            km += raindeer->speed;
        }

        --timer;
        if (timer == 0) {
            run_or_rest = !run_or_rest;
            timer = run_or_rest ? raindeer->runtime : raindeer->resttime;
        }
    }
    return km;
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
    // Count lines -> Raindeers

    const int race_seconds = 2503;
    int max = 0;

    raindeer_t raindeers[9];
    int raindeer_count = 0;;
    int scores[9] = {0};

    char line[128] = {0};
    long j = 0, linecount = 1;
    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n' || !buf[i + 1]) {
            
            line[j] = 0;
            j = 0;

            //printf("%ld.- '%s'\n", linecount, line);

            raindeers[raindeer_count] = raindeer_create(line);
            int kms = raindeer_race(raindeers + raindeer_count, race_seconds);
            
            printf(
                "After %d seconds, %s raced %d kms.\n", 
                race_seconds, raindeers[raindeer_count].name, kms
            );

            if (kms > max) {
                max = kms;
            }

            ++raindeer_count;
            ++linecount;
            memset(line, 0, 128);
            
        }
        else line[j++] = buf[i];
    }

    printf("Puzzle 1: %d\n", max);

    // Puzzle 2

#define RAINDEER_COUNT 9

    for (int i = 1; i <= race_seconds; ++i) {

        int max = 0, maxindex;

        for (int j = 0; j < RAINDEER_COUNT; ++j) {
            int kms = raindeer_race(raindeers + j, i);
            if (kms > max) {
                max = kms;
                maxindex = j;
            }
        }

        ++scores[maxindex];
        
        printf(
            "%s in winning at %d kms in second %d.\n",
            raindeers[maxindex].name, max, i
        );

    }

    max = 0;
    for (int i = 0; i < RAINDEER_COUNT; ++i) {
        if (scores[i] > max) {
            max = scores[i];
        }
        printf("%s: %d\n", raindeers[i].name, scores[i]);
    }

    printf("Puzzle 2: %d\n", max);

    free(buf);
    return EXIT_SUCCESS;
}
