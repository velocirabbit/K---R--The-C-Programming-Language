/* 07E02.c (p. 155) */
#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXWIDTH 80
#define OCTLEN   6
#define MAXWORDLEN 80

char *nextword(void);

int main(void) {
    int i = 0;
    char c;

    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            printf("%c\n", c);
            i = 0;
        }
        else {
            int nextlen;
            if (iscntrl(c))
                nextlen = OCTLEN;
            else {
                int wasblank;
                while ((wasblank = isblank(c))) {
                    printf("%c", c);
                    c = getchar();
                }
                nextlen = 1;  // since c is the start of the next word
                char *w = nextword();
                nextlen += strlen(w);
                if (i + nextlen >= MAXWIDTH) {
                    printf("\n");
                    i = 0;
                }
                if (wasblank)
                    printf("\\%03o", c);
                else
                    printf("%c%s", c, w);
                i += nextlen;
            }
        }
    }

    return 0;
}

char *nextword(void) {
    char *w = malloc(sizeof(char[MAXWORDLEN]));
    char *e = w;
    while (!isblank(*e++ = getchar()));
    *e = '\0';
    return w;
}