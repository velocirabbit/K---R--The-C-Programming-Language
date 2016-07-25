/* 05E13.c (p. 118) */
#include <stdio.h>
#include <stdlib.h>

#define MAXSTRLEN 1000
#define DEFAULTN 10

int getline(char *c, int max);
void tail(char *c[], int n, int lines);

int main(int argc, char *argv[]) {
    int n;
    if (argc == 2) {
        if (argv[1][0] == '-')
            n = atoi((char*)++(argv[1]));
        else
            printf("ERROR: Please preface argument n with a '-'\n");
    }
    else if (argc > 2) {
        printf("ERROR: Too many arguments!\n");
        printf("Usage: 05E13 [-n]\n");
        printf("Optional arguments:\n");
        printf("    -n: Number of lines to print (counts from the bottom).\n");
        printf("        If n is greater than the number of lines put in, print all\n");
        printf("        of them.\n");
    }  // Usage message here
    else
        n = DEFAULTN;

    int c, lines = 0;
    char **in = malloc(sizeof(char*));
    in[lines] = malloc(sizeof(char[MAXSTRLEN]));
    while ((c = getline(in[lines++], MAXSTRLEN)) != 0) {
        in = realloc(in, sizeof(char*) * (lines + 1));
        in[lines] = malloc(sizeof(char[MAXSTRLEN]));
    }
    lines--;
    if (lines > 0)
        tail(in, n, lines);
    else
        printf("Oh! Okay then!\n");

    for (int i = 0; i <= lines; i++)
        free(in[i]);
    free(in);
    return 0;
}

/**
 * getline: Gets a line of input. Doesn't add a newline character att the end of the
 *   string.
 *
 * Returns: the number of characters in the line obtained (not counting the terminating
 *   null string character).
 */
int getline(char *s, int max) {
    int i, c;

    for (i = 0; i < max && (c = getchar()) != EOF && c != '\n'; i++)
        s[i] = c;
    s[i] = 0;
    return i;
}

/**
 * tail: Prints the last n lines of string array c. 
 */
void tail(char *c[], int n, int lines) {
    int on = (n > lines) ? 0 : lines - n;
    for (; n > 0 && on < lines; n--, on++)
        printf("%s\n", c[on]);
}