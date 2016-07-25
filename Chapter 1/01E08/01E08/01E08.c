/* 01E08.c (p. 20) */
#include <stdio.h>

main() {
    int c, blanks, tabs, newlines;
    blanks = tabs = newlines = 0;

    for (c = getchar(); c != '\n'; c = getchar()) {  
        if (c == ' ')
            ++blanks;
        else if (c == '\t')
            ++tabs;
        else if (c == '\n')
            ++newlines;
    }
    if (c == '\n')
        ++newlines;
    printf("Number of blanks:   %d\n", blanks);
    printf("Number of tabs:     %d\n", tabs);
    printf("Number of newlines: %d\n", newlines);

    printf("\n");
    system("pause");
    return 0;
}