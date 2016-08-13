/* 06_getwordcount (p. 140) */
#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXWORD 100
#define MAXBUFF 1000

struct tnode {          // the tree node
    char *word;           // points to the text
    int count;            // number of occurrences
    struct tnode *left;   // left child
    struct tnode *right;  // right child
};

char buff[MAXBUFF];
int buffp = 0;

char *strdup2(char *s);
struct tnode *talloc(void);
struct tnode *addtree(struct tnode *p, char *w);
void treeprint(struct tnode *p);
int getword(char *word, int lim);
int getch(void);
void ungetch(int c);

int main(void) {
    struct tnode *root;
    char word[MAXWORD];

    root = NULL;
    while (getword(word, MAXWORD) != EOF)
        if (isalpha(word[0]))
            root = addtree(root, word);
    treeprint(root);

    printf("\n\n");
    system("pause");
    return 0;
}

/* talloc */
struct tnode *talloc(void) {
    return (struct tnode *) malloc(sizeof(struct tnode));
}

/* strdup */
char *strdup2(char *s) {
    char *p;

    p = (char *)malloc(strlen(s) + 1);  // +2 for '\0'
    if (p != NULL)
        strcpy(p, s);
    return p;
}

/* addtree */
struct tnode *addtree(struct tnode *p, char *w) {
    int cond;

    if (p == NULL) {    // a new word has arrived
        p = talloc();   // make a new node
        p->word = strdup2(w);
        p->count = 1;
        p->left = p->right = NULL;
    }
    else if ((cond = strcmp(w, p->word)) == 0)
        p->count++;     // repeated word
    else if (cond < 0)  // less than into left subtree
        p->left = addtree(p->left, w);
    else                // greater than into right subtree
        p->right = addtree(p->right, w);
    return p;
}

/* treeprint */
void treeprint(struct tnode *p) {
    if (p != NULL) {
        treeprint(p->left);
        printf("%s       \t%4d\n", p->word, p->count);
        treeprint(p->right);
    }
}

/* getword */
int getword(char *word, int lim) {
    int c;
    char *w = word;

    while (isspace(c = getch()));
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

/* getch */
int getch(void) {
    return (buffp > 0) ? buff[--buffp] : getchar();
}

/* ungetch */
void ungetch(int c) {
    if (buffp < MAXBUFF)
        buff[buffp++] = c;
}