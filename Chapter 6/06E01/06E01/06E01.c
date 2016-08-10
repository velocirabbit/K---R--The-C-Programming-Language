/* 06E01 (p. 136) */
/*
TODO:
- Handle string constants
- Handle comments
- Handle preprocessor control lines
*/
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

Bool ok2Read(char *word);
int getword(char *word, int lim);
int binsearch(char *word, struct key *tab, int n);
int getch(void);
void ungetch(int c);

int buffp = 0;
char buff[MAXBUFF];
Bool inComment = False;
Bool inQuote = False;
Bool inPreProc = False;
Bool lineStart = True;
char comCh;  // '*' or '/'

/* count C keywords */
int main(void) {
    int n;
    char word[MAXWORD];

    while (getword(word, MAXWORD) != EOF) {
        if (ok2Read(word) && isalpha(word[0]))
            if ((n = binsearch(word, keytab, NKEYS)) >= 0)
                keytab[n].count++;
    }
    printf("\n");
    for (n = 0; n < NKEYS; n++)
        if (keytab[n].count > 0)
            printf("%s       \t%4d\n", keytab[n].word, keytab[n].count);

    printf("\n\n");
    system("pause");
    return 0;
}
/**
 * ok2Read: Returns true if it's currently okay to check the word.
 *
 * Returns: Bool indiciating if it's okay (True) or not (False)
 */
Bool ok2Read(char *word) {
    Bool result = True;
    static Bool almostComment = False;
    static Bool escaped = False;
    // Check to see if we enter a quote, comment, or preprocessor directive

    if (!escaped) {
        if (word[0] == '\\') {
            escaped = True;
            return False;
        }
        else if (!inQuote && !inComment) {
            if (!almostComment && word[0] == '/')
                almostComment = True;
            else {
                if (almostComment && (word[0] == '*' || word[0] == '/')) {
                    inComment = True;
                    comCh = word[0];
                }
                else if (word[0] == '"')
                    inQuote = True;
                else if (lineStart && word[0] == '#')
                    inPreProc = True;
                almostComment = False;
            }
        }
    }
    // If we're in any of the three, result = False
    if (inComment || inQuote || inPreProc)
        result = False;
    // Check to see if we remain in any of them
    if (inQuote) {
        if (!escaped) {
            if (word[0] == '"')
                inQuote = False;
            else if (word[0] == '\n') {
                lineStart = True;
                inPreProc = False;
                if (comCh == '/')
                    inComment = False;
                if (inQuote)
                    inQuote = False;
            }
        }
    }
    if (inComment) {

    }
    if (escaped)
        escaped = False;

    return result;
}

/* binsearch: find word in tab[0]...tab[n-1]. Tab must be sorted in increasing
order by word.
returns: index with the word */
int binsearch(char *word, struct key tab[], int n) {
    int cond;
    int low, high, mid;
    printf("test");
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
    if (!isalpha(c) && c != '_') {
        *w = '\0';
        return c;
    }
    for (; --lim > 0; w++)
        if (!isalnum(*w = getch()) && *w != '_') {
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