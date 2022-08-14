#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    const int posy = 2978, posx = 3083;
    int x = 0, y = 0;
    long n = 20151125;
    for (long i = 0; !((x + 1 == posx) && (y + 1 == posy)); ++i) {
        n = (n * 252533) % 33554393;
        ++x, --y;
        if (y < 0) {
            y = x;
            x = 0;
        }
        //printf("%ld - %d, %d\n", n, x + 1, y + 1); 
    }
    printf("Puzzle 1: %ld\n", n);
    return EXIT_SUCCESS;
}
