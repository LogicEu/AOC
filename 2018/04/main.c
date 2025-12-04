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

#define STRLEN 128
#define LEN 32
#define swap(a, b, type) do { type tmp = a; a = b; b = tmp; } while (0)

typedef struct info_t {
    int month; // 60 * 24 * 30
    int day; // 60 * 24
    int hour; // 60
    int minute;
    char op[32];
} info_t;

static info_t info_parse(const char* str)
{
    int year;
    char s1[LEN], s2[LEN];
    info_t info;
    sscanf(
        str, "[%d-%d-%d %d:%d] %s %s", 
        &year, &info.month, &info.day, &info.hour, &info.minute, s1, s2
    );

    if (!strcmp(s1, "Guard")) {
        memcpy(info.op, s2 + 1, LEN);
    } else memcpy(info.op, s1, LEN);

    return info;
}

static inline long info_sum(const info_t* info)
{
    return  info->minute + 
            60 * (info->hour + 
            24 * (info->day + 
            30 * (info->month)));
}

static int partition(info_t* arr, const int low, const int high)
{
    long pivot = info_sum(arr + high);
    int i = (low - 1);
    for (int j = low; j < high; ++j) {
        if (info_sum(arr + j) < pivot) {
            ++i;
            swap(arr[i], arr[j], info_t);
        }
    }
    swap(arr[i + 1], arr[high], info_t);
    return i + 1;
}

static void quickSort(info_t* arr, const int low, const int high)
{
    if (low < high) {
        int p = partition(arr, low, high);
        quickSort(arr, low, p - 1);
        quickSort(arr, p + 1, high);
    }
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

    long i, j = 0, infocount = 0;
    char str[STRLEN];
    info_t infos[1088];

    for (i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            infos[infocount++] = info_parse(str);
        }
        else str[j++] = buf[i];
    }

    if (j) {
        str[j] = 0;
        infos[infocount++] = info_parse(str);
    }

    quickSort(infos, 0, infocount - 1);
    long guards[0xfff]; 
    int day

    for (i = 0; i < infocount; ++i) {
        if (!strcmp(infos[i].op, "falls")) {

        }
        else if (!strcmp(infos[i].op, "wakes")) {
            
        }
        else {
            
        }
    }


    free(buf);
    return EXIT_SUCCESS;
}
