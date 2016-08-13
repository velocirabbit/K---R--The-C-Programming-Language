/* 06_getwordptr (p. 137)
 * Implementation of 06_getword (counts instances of C keywords in a file)
 * using pointers to traverse structures */
#include <ctype.h>
#include <stdio.h>
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

char buff[MAXBUFF];
int buffp = 0;

Bool ok2read(char *word);
void ungetch(int c);
int getch(void);
int getword(char *word, int lim);
struct key *binsearch(char *word, struct key *tab, int n);

int main(void) {
    char word[MAXWORD];
    struct key *p;

    while (getword(word, MAXWORD) != EOF)
        if (ok2read(word) && (isalpha(word[0]) || word[0] == '_'))
            if ((p = binsearch(word, keytab, NKEYS)) != NULL)
                p->count++;
    printf("\n");
    for (p = keytab; p < keytab + NKEYS; p++)
        if (p->count > 0)
            printf("%s       \t%4d\n", p->word, p->count);
    printf("\n\n");
    system("pause");
    return 0;
}

/* ok2read */
Bool ok2read(char *word) {
    static char comCh;  // '*' or '/'
    static Bool inComment = False;
    static Bool inQuote = False;
    static Bool inPreProc = False;
    static Bool lineStart = True;
    static Bool almostComment = False;
    static Bool almostNoComment = False;
    static Bool escaped = False;
    static Bool result = True;  // start assuming result is True

    // Check to see if next char will be escaped
    if (inQuote && !escaped && word[0] == '\\') {
        escaped = True;
        return False;
    }

    // Check to see if we enter a quote, comment, or preprocessor directive
    if (!inQuote && !inComment) {  // but only if not in quote or comment
        if (lineStart && word[0] == '#')
            inPreProc = True;
        else if (!almostComment && word[0] == '/')  // maybe enter comment
            almostComment = True;
        else {
            if (almostComment && (word[0] == '*' || word[0] == '/')) {
                inComment = True;
                comCh = word[0];
            }
            else if (word[0] == '"')
                inQuote = True;
            almostComment = False;
        }
    }

    lineStart = False;
    // Check to see if we remain in any of them
    if (!result) {  // but only if we haven't just entered them
        if (inQuote && !escaped && word[0] == '"')
            inQuote = False;
        else if (inComment && comCh == '*') {
            if (word[0] == '*')
                almostNoComment = True;
            else if (almostNoComment) {
                if (word[0] == '/')
                    inComment = False;
                almostNoComment = False;
            }
        }
    }
    if (word[0] == '\n' && !escaped) {
        if (inComment && comCh == '/')
            inComment = False;
        if (!inQuote && !inComment) {
            lineStart = True;
            inPreProc = False;
        }
    }

    // Done looking at word, so no longer need escape
    if (escaped)
        escaped = False;

    // If we're in any of the three, result = False
    result = (inComment || inQuote || inPreProc) ? False : True;
    return result;
}

/* ungetch */
void ungetch(int c) {
    if (buffp < MAXBUFF)
        buff[buffp++] = c;
}

/* getch */
int getch(void) {
    return (buffp > 0) ? buff[--buffp] : getchar();
}

/* getword */
int getword(char *word, int lim) {
    int c;
    char *w = word;

    while (isspace(c = getch()) && c != '\n');  // skip white spaces except '\n'
    if (c != EOF)
        *w++ = c;
    if (!isalpha(c) && c != '_') {
        w = '\0';
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

/* binsearch */
struct key *binsearch(char *word, struct key *tab, int n) {
    int cond;
    struct key *low = &tab[0];
    struct key *high = &tab[n];
    struct key *mid;

    while (low < high) {
        // Sets mid to address halfway between low and high
        mid = low + (high - low) / 2;
        if ((cond = strcmp(word, mid->word)) < 0)
            high = mid;
        else if (cond > 0)
            low = mid + 1;
        else
            return mid;
    }
    return NULL;
}