#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

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

static inline float gettime(void)
{
    return (float)clock()/CLOCKS_PER_SEC;
}

static inline int countbits(int n)
{
    const int size = sizeof(int);
    int count = 0;
    char* bytes = (char*)&n;
    for (int i = size - 1; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {
            count += (bytes[i] >> j) & 1;
        }
    }
    return count;
}

static inline int countarray(const int* array, const int arrcount)
{
    int sum = 0;
    for (int i = 0; i < arrcount; ++i) {
        sum += array[i];
    }
    return sum;
}

int puzzle1(const int* array, const int arrcount)
{
    int sum = countarray(array, arrcount); 
    const int third = sum / 3;
    printf("Sum: %d\nThird: %d\n", sum, third);

    int icount = 0;
    int iarray[10000];
    int minbits = 9999999;

    long long minqe = 9999999999999999, qe;

    float t = gettime(); 
    float delta = t, timer = 2.0;

    for (int i = 0; i < INT_MAX; ++i) {
        
        const int onbits = countbits(i);
        if (onbits > minbits) {
            continue;
        }

        t = gettime();
        if (t - delta > timer * 4.0) {
            break;
        }
        
        sum = 0;
        qe = 0;
        
        for (int j = arrcount - 1; j >= 0 && sum <= third; --j) {
            
            char* bytes = (char*)&i;
            int bit = (bytes[j / 8] >> (j % 8)) & 1;

            const int n = array[j] * bit;
            qe = !qe * n + qe * n + !n * qe;
            sum += n;

        }

        if (sum == third && qe >= 0 && qe <= minqe) {
            minbits = onbits;
            minqe = qe;

            iarray[icount++] = i;
            printf("%lld\n", qe);
            //printf("%f, %f, %f\n", t, delta, timer);

            timer = (t - delta) > timer ? t - delta : timer;
            delta = t;
            
        }
    }
    return minqe;
}

int puzzle2(const int* array, const int arrcount)
{
    int sum = countarray(array, arrcount); 
    const int fourth = sum / 4;
    printf("Sum: %d\nFourth: %d\n", sum, fourth);

    int icount = 0;
    int iarray[10000];
    int minbits = 9999999;

    long long minqe = 9999999999999999, qe;

    float t = gettime(); 
    float delta = t, timer = 2.0;

    for (int i = 0; i < INT_MAX; ++i) {
        
        const int onbits = countbits(i);
        if (onbits > minbits) {
            continue;
        }

        t = gettime();
        if (t - delta > timer * 4.0) {
            break;
        }
        
        sum = 0;
        qe = 0;
        
        for (int j = arrcount - 1; j >= 0 && sum <= fourth; --j) {
            
            char* bytes = (char*)&i;
            int bit = (bytes[j / 8] >> (j % 8)) & 1;

            const int n = array[j] * bit;
            qe = !qe * n + qe * n + !n * qe;
            sum += n;

        }

        if (sum == fourth && qe >= 0 && qe <= minqe) {
            minbits = onbits;
            minqe = qe;

            iarray[icount++] = i;
            printf("%lld\n", qe);
            //printf("%f, %f, %f\n", t, delta, timer);

            timer = (t - delta) > timer ? t - delta : timer;
            delta = t;
            
        }
    }
    return minqe;
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
    
    int array[32];
    int arrcount = 0;

    int j = 0;
    char str[16];

    for (int i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            array[arrcount++] = atoi(str);
        }
        else str[j++] = buf[i];
    }

    str[j] = 0;
    array[arrcount++] = atoi(str);

    int p1 = puzzle1(array, arrcount);
    int p2 = puzzle2(array, arrcount);
    printf("Puzzle 1: %d\n", p1);
    printf("Puzzle 2: %d\n", p2);

    free(buf);
    return EXIT_SUCCESS;
}
