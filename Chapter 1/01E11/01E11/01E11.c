/* 01E11.c */
#include <stdio.h>

#define IN 1
#define OUT 0

main() {
    int c, lines, words, chars, state;

    state = OUT;
    lines = words = chars = 0;

    while ((c = getchar()) != '\n') {
        ++chars;
        if (c == '\n')
            ++lines;
        if (c == ' ' || c == '\n' || c == '\t')
            state = OUT;
        else if (state == OUT) {
            state = IN;
            ++words;
        }
    }
    ++lines;

    printf("Number of lines: %d\n", lines);
    printf("Number of words: %d\n", words);
    printf("Number of chars: %d\n", chars);

    printf("\n");
    system("pause");
    return 0;
}