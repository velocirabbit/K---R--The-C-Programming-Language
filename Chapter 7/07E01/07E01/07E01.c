/* 07E01.c (p. 153) */
#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define LOWERLONG   "-lower"
#define LOWERSHORT  "-l"
#define UPPERLONG   "-upper"
#define UPPERSHORT  "-u"

int main(int argc, char *argv[]) {
    int d, c;
    if (argc == 1 || strcmp(argv[1], LOWERLONG) == 0 ||
        strcmp(argv[1], LOWERSHORT) == 0) {
        printf("Printing to lowercase:\n\n");
        d = 'l';
    }
    else if (strcmp(argv[1], UPPERLONG) == 0 || strcmp(argv[1], UPPERSHORT) == 0) {
        printf("Printing to uppercase:\n\n");
        d = 'u';
    }
    else {
        printf("\nERROR: Incorrect option usage. Default to '-l'\n");
        d = 'l';
    }

    while ((c = getchar()) != EOF) {
        if (c == '\n')
            printf("\n");
        putchar((d == 'l') ? tolower(c) : toupper(c));
    }
    return 0;
}