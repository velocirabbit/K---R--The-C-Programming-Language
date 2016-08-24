/* 06E04.c (p. 143) */
#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRLEN 1000

typedef struct s_word {
    char *w;
    int count;
    struct s_word *left;
    struct s_word *right;
} word;

typedef enum { False = 0, True } bool;

bool getword(char *w, char **line);
bool parseargs(int argc, char *argv[], char *fn);
int readfile(char *fn, char ***lines, int strlen);
word *add2tree(word *w, char *s);
char *lowerstr(char *s);
void buildtree(word **w, char **lines, int nlines);
void printtree(word *w);

int main(int argc, char *argv[]) {
    char filename[MAXSTRLEN];
    if (!parseargs(argc, argv, filename))
        return 1;

    char **lines;
    int n;
    if ((n = readfile(filename, &lines, MAXSTRLEN)) <= 0)
        return 1;

    word *root = NULL;
    buildtree(&root, lines, n);
    printtree(root);

    free(lines);
    printf("\n");
    return 0;
}

int readfile(char *fn, char ***lines, int strlen) {
    FILE *fp = fopen(fn, "r");
    int i = 1;
    *lines = malloc(sizeof(char**));
    do {
        if ((*lines = realloc(*lines, sizeof(char*) * i)) == NULL)
            return -1;
        if (((*lines)[i - 1] = malloc(sizeof(char) * strlen)) == NULL)
            return -1;
    } while (fgets((*lines)[i++ - 1], strlen, fp) != NULL);
    return --i - 1;  // last line will be NULL. Also need to decrement by 1
}

bool getword(char *w, char **line) {
    while (!isalnum(**line) && **line != '-') {
        if (**line == '\0' || **line == '\n' || **line == EOF)
            return False;
        (*line)++;
    }
    while (isalnum(**line) || **line == '-')
        *w++ = *(*line)++;
    *w = '\0';
    return True;
}

bool parseargs(int argc, char *argv[], char *fn) {
    switch (argc) {
        case 2:
            strcpy(fn, argv[1]);
            return True;
        default:
            printf("Usage:\n");
            printf("\t06E04 [filename]\n");
            return False;
    }
}

word *add2tree(word *w, char *s) {
    int cmp;
    if (w == NULL) {
        if ((w = malloc(sizeof(word))) == NULL)
            return w;
        if ((w->w = malloc(sizeof(char) * (strlen(s) + 1))) == NULL)
            return w;
        strcpy(w->w, s);
        w->count = 1;
        w->left = w->right = NULL;
    }
    else if ((cmp = strcmp(w->w, s)) == 0)
        (w->count)++;
    else if (cmp < 0)
        w->left = add2tree(w->left, s);
    else if (cmp > 0)
        w->right = add2tree(w->right, s);
    return w;
}

char *lowerstr(char *s) {
    int len = strlen(s) + 1;
    char *t = (char*)malloc(sizeof(char) * len);
    while (*t++ = tolower(*s++));
    return t - len;
}

void buildtree(word **w, char **lines, int nlines) {
    char *wstr = (char*)malloc(sizeof(char[MAXSTRLEN]));
    for (int i = 0; i < nlines; i++) {
        while (getword(wstr, &(lines[i])))
            *w = add2tree(*w, lowerstr(wstr));
    }
}

void printtree(word *w) {
    if (w->left != NULL)
        printtree(w->left);
    printf("%d\t%s\n", w->count, w->w);
    if (w->right != NULL)
        printtree(w->right);
}