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

static long recursive(const char* str, const char reps[44][2][16], 
        const long rcount, const long count)
{
    char buf[516];

    static long bestmark = 999999;

    if (count >= bestmark) {
        return 0;
    }

    if (!strcmp(str, "e")) {
        bestmark = count;
        return count;
    }

    for (long i = 0; i < rcount; ++i) { 
        
        if (!strcmp(str, reps[i][0])) {  
            return 0;
        }

        const long len = strlen(reps[i][1]);
        const long len2 = strlen(reps[i][0]);
        
        char* substr = (char*)(size_t)str;
        while ((substr = strstr(substr, reps[i][1]))) {
            
            size_t j = substr - str;

            strcpy(buf, str);
            memcpy(buf + j, reps[i][0], len2);
            memcpy(buf + j + len2, buf + j + len, strlen(buf + j + len) + 1);

            printf("%ld.- '%s'\n", count, buf);

            recursive(buf, reps, rcount, count + 1);
            
            ++substr;
        }

    }

    return count * !strcmp(buf, "e");
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
    
    char str[516];
    long j = 0, linecount = 0;

    long rcount = 0;
    char replacements[44][2][16];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            str[j] = 0;
            j = 0;
            ++linecount;
    
            if (str[0]) {
                sscanf(str, "%s => %s", replacements[rcount][0], replacements[rcount][1]);
                ++rcount;
            }
        }
        else str[j++] = buf[i];
    }

    str[j] = 0;

    for (long i = 0; i < rcount; ++i) {
        printf("%s => %s\n", replacements[i][0], replacements[i][1]);
    }
    printf("%s\n", str);


    long arrcount = 0;
    char strrep[516];
    char strarr[1000][516];

    for (long i = 0; i < rcount; ++i) {
        
        const size_t len = strlen(replacements[i][0]);
        const size_t len2 = strlen(replacements[i][1]);
        
        for (long j = 0; str[j]; ++j) {
            if (!memcmp(str + j, replacements[i][0], len)) {
                
                memset(strrep, 0, 516);
                memcpy(strrep, str, j);
                memcpy(strrep + j, replacements[i][1], len2);
                memcpy(strrep + j + len2, str + j + len, strlen(str + j + len));
                
                int found = 0;
                for (long k = 0; k < arrcount; ++k) {
                    if (!strcmp(strarr[k], strrep)) {
                        ++found;
                        break;
                    }
                }

                if (!found) {
                    strcpy(strarr[arrcount++], strrep);
                }
            }
        }
    }
    
    printf("Puzzle 1: %ld\n", arrcount);

    //Puzzle 2

    for (int i = 0; i < rcount - 1; ++i) {
        for (int j = 0; j < rcount - i - 1; ++j) {
            if (strlen(replacements[j][1]) < strlen(replacements[j + 1][1])) {
                char rep[2][16];
                memcpy(rep, replacements[j], 2 * 16);
                memcpy(replacements[j], replacements[j + 1], 2 * 16);
                memcpy(replacements[j + 1], rep, 2 * 16);
            }
        }
    }

    for (int i = 0; i < rcount; ++i) {
        printf("%s => %s\n", replacements[i][0], replacements[i][1]);
    }

    long puzzle2 = recursive(str, replacements, rcount, 0);
    printf("Puzzle 2: %ld\n", puzzle2);

    free(buf);
    return EXIT_SUCCESS;
}
