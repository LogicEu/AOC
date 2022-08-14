#include <stdio.h>

static long puzzle1(const long input)
{
    const long cnst = 60;
    long i, house = 0, max = 0;
    for (i = 600000; house < input; i += cnst) {
        house = 0;
        for (long j = 1; j < i; ++j) {
            house += j * !(i % j);
        }
        house = (house + i) * 10;

        /*if (house > max) {
            printf("House %ld got %ld presents.\n", i, house);
            max = house;
        }*/
    }
    return i - cnst;
}

static long puzzle2(const long input)
{
    const long cnst = 20;
    long i, house = 0, max = 0;
    for (i = 800000; house < input; i += cnst) {
        house = 0;
        for (long j = 1; j < i; ++j) {
            house += j * (!(i % j) && (i <= j * 50));
        }
        house = (house + i) * 11;

        /*if (house > max) {
            printf("House %ld got %ld presents.\n", i, house);
            max = house;
        }*/
    }
    return i - cnst;
}

int main(void)
{
    const long input = 34000000;
    printf("Puzzle 1: %ld\n", puzzle1(input));
    printf("Puzzle 2: %ld\n", puzzle2(input));
    return 0;
}
