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

#define INGREDIENT_COUNT 4

typedef struct ingredient_t {
    char name[16];
    int capacity;
    int durability;
    int flavour;
    int texture;
    int calories;
} ingredient_t;

static ingredient_t ingredient_create(const char* str)
{
    ingredient_t ingredient;
    
    sscanf(
        str,
        "%s capacity %d, durability %d, flavor %d, texture %d, calories %d",
        ingredient.name,
        &ingredient.capacity,
        &ingredient.durability,
        &ingredient.flavour,
        &ingredient.texture,
        &ingredient.calories
    );

    /*printf("%s %d %d %d %d %d\n", 
            ingredient.name, 
            ingredient.capacity,
            ingredient.durability, 
            ingredient.flavour, 
            ingredient.texture,
            ingredient.calories
    );*/

    return ingredient;
}

static ingredient_t ingredient_add(const ingredient_t* ingredients, const int* percentages)
{
    ingredient_t sum = {"Sum", 0, 0, 0, 0, 0};

    for (int i = 0; i < INGREDIENT_COUNT; ++i) {
        sum.capacity += ingredients[i].capacity * percentages[i];
        sum.durability += ingredients[i].durability * percentages[i];
        sum.flavour += ingredients[i].flavour * percentages[i];
        sum.texture += ingredients[i].texture * percentages[i];
        sum.calories += ingredients[i].calories * percentages[i];
    }

    sum.capacity = sum.capacity >= 0 ? sum.capacity : 0;
    sum.durability = sum.durability >= 0 ? sum.durability : 0;
    sum.flavour = sum.flavour >= 0 ? sum.flavour : 0;
    sum.texture = sum.texture >= 0 ? sum.texture : 0;
    sum.calories = sum.calories >= 0 ? sum.calories : 0;
   
    return sum;
}

static long ingredient_sum(const ingredient_t* ingredients, const int* percentages)
{
    ingredient_t sum = ingredient_add(ingredients, percentages);
    return (long)sum.capacity * (long)sum.durability * 
        (long)sum.flavour * (long)sum.texture;
}
static long ingredient_sum2(const ingredient_t* ingredients, const int* percentages)
{
    ingredient_t sum = ingredient_add(ingredients, percentages);
    long r = (long)sum.capacity * (long)sum.durability * 
        (long)sum.flavour * (long)sum.texture;
    if (sum.calories == 500) {
        return r;
    }
    return 0;
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


    ingredient_t ingredients[INGREDIENT_COUNT];
    int ingredient_count = 0;

    char str[128];
    long j = 0, line_count = 0;
    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;

            //printf("%ld.- '%s'\n", line_count, str);
            ingredients[ingredient_count++] = ingredient_create(str);

            ++line_count;
            memset(str, 0, 128);
        }
        else str[j++] = buf[i];
    }

    str[j] = 0;
    j = 0;

    //printf("%ld.- '%s'\n", line_count, str);
    ingredients[ingredient_count++] = ingredient_create(str);

    ++line_count;

    long maxscore = 0, maxcalorie = 0;

    for (int i = 0; i <= 100; ++i) {
        for (int j = 0; j <= 100 - i; ++j) {
            for (int k = 0; k <= 100 - i - j; ++k) {
                
                int percentages[4] = {i, j, k, 100 - i -j - k};
                
                long sum = ingredient_sum(ingredients, percentages);
                long calorie = ingredient_sum2(ingredients, percentages);
                
                if (sum > maxscore) {
                    maxscore = sum;
                }

                if (calorie > maxcalorie) {
                    maxcalorie = calorie;
                }
            }
        }
    }

    printf("Puzzle 1: %ld\n", maxscore);
    printf("Puzzle 2: %ld\n", maxcalorie);

    free(buf);
    return EXIT_SUCCESS;
}
