/* 06E01 (p. 136) */
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD 100
#define MAXBUFF 1000
#define NKEYS (sizeof(keytab) / sizeof(struct key))

typedef enum { False = 0, True } Bool;

struct key {
    char *word;
    int count;
} keytab[] = {
    { "auto",        0 },
    { "break",       0 },
    { "case",        0 },
    { "char",        0 },
    { "const",       0 },
    { "continue",    0 },
    { "default",     0 },
    { "do",          0 },
    { "double",      0 },
    { "else",        0 },
    { "enum",        0 },
    { "extern",      0 },
    { "float",       0 },
    { "for",         0 },
    { "goto",        0 },
    { "if",          0 },
    { "int",         0 },
    { "long",        0 },
    { "register",    0 },
    { "return",      0 },
    { "short",       0 },
    { "signed",      0 },
    { "sizeof",      0 },
    { "static",      0 },
    { "struct",      0 },
    { "switch",      0 },
    { "typedef",     0 },
    { "union",       0 },
    { "unsigned",    0 },
    { "void",        0 },
    { "volatile",    0 },
    { "while",       0 }
};

int getword(char *word, int lim);
int binsearch(char *word, struct key *tab, int n);
int getch(void);
void ungetch(int c);

int buffp = 0;
char buff[MAXBUFF];
Bool inComment = False;
char comCh;  // '*' or '/'

/* count C keywords */
int main(void) {
    int n;
    char word[MAXWORD];

    while (getword(word, MAXWORD) != EOF)
        if (isalpha(word[0]))
            if ((n = binsearch(word, keytab, NKEYS)) >= 0)
                keytab[n].count++;
    printf("\n");
    for (n = 0; n < NKEYS; n++)
        if (keytab[n].count > 0)
            printf("%s       \t%4d\n", keytab[n].word, keytab[n].count);

    printf("\n\n");
    system("pause");
    return 0;
}

/* binsearch: find word in tab[0]...tab[n-1]. Tab must be sorted in increasing
order by word.
returns: index with the word */
int binsearch(char *word, struct key tab[], int n) {
    int cond;
    int low, high, mid;

    low = 0;
    high = n - 1;
    while (low <= high) {
        mid = (low + high) / 2;
        if ((cond = strcmp(word, tab[mid].word)) < 0)
            high = mid - 1;
        else if (cond > 0)
            low = mid + 1;
        else
            return mid;
    }
    return -1;
}

/* getword: get next word or character from input
returns: the first character of the word, or EOF for end of file, or the
character itself if it is not alphabetic */
int getword(char *word, int lim) {
    int c;
    char *w = word;

    while (isspace(c = getch()));
    if (c != EOF)
        *w++ = c;
    if (!isalpha(c)) {
        *w = '\0';
        return c;
    }
    for (; --lim > 0; w++)
        if (!isalnum(*w = getch())) {
            ungetch(*w);
            break;
        }
    *w = '\0';
    return word[0];
}

/**
* getch: Gets the next character from stdin unless there's something in the buffer.
*
* Returns: next character
*/
int getch(void) {
    return (buffp > 0) ? buff[--buffp] : getchar();
}

/**
* ungetch: Puts char c into the input buffer.
*/
void ungetch(int c) {
    if (buffp < MAXBUFF)
        buff[buffp++] = c;
    else
        printf("ERROR: Buffer full. Can't place char '%c'\n", c);
}