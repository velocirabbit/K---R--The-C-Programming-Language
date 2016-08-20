/* 06E03.c (p. 143) */
#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINECOUNT 1000
#define MAXSTRLEN    1000
#define TFLIM        0.015

typedef enum { False = 0, True } bool;

typedef struct s_word {
    char *wordstr;
    int *foundOn;
    int count;
    struct s_word *left;
    struct s_word *right;
} word;

bool parseArgs(int argc, char *argv[]);
bool getWord(char **line, char *w);
int readWords(char **lines, word **w, int n);
int getfile(char ***lines, char fn[], int n);
int getWordCount(word *w);
word *add2tree(word *w, char *s, int ln);
char *safecopy(char *s);
char *strlower(char *s);
void append(int **arr, int i, int len);
void printTree(word *w);
void printUsageStatement(void);
float gettf(word *w);

int totalwords = 0;

int main(int argc, char *argv[]) {
    printf("\n");
    if (!parseArgs(argc, argv))  // will print error statements in here
        return 1;

    char **lines = malloc(sizeof(char**));
    if (lines == NULL) {
        printf("ERROR: Couldn't create string array for source file.\n");
        return 1;
    }
    int n;
    if ((n = getfile(&lines, argv[1], MAXSTRLEN)) <= -1) {
        printf("ERROR: Source file couldn't be read\n");
        return 1;
    }
    else if (n == 0) {
        printf("ERROR: Source file empty\n");
        return 1;
    }

    word *root = NULL;  // Must set to NULL to be root
    readWords(lines, &root, n);
    totalwords = getWordCount(root);
    // Print it all out
    printTree(root);

    // Clean up pointers
    //for (--n; n >= 0; n--)
        //cfree(lines[n]);
    free(lines);

    printf("\n");
    return 0;
}

/**
* parseArgs: Parses the command line arguments to pull out relevant info.
*   Prints all necessary error statements relating to the argument inputs.
*
* Returns: True if program can continue running, False otherwise.
*/
bool parseArgs(int argc, char *argv[]) {
    switch (argc) {
        case 2:
            // argv[1] would be the file to read
            return True;
        default:
            printUsageStatement();
            return False;
    }
}

/**
* getWord: Gets the next word in string pointed to by line, then stores that
*   word into string w.
*
* Returns: True if a word was found, False if not.
*/
bool getWord(char **lines, char *w) {
    while (!isalnum(**lines) && **lines != '-') {
        if (**lines == '\0')
            return False;
        (*lines)++;
    }
    while (isalnum(**lines) || **lines == '-')
        *w++ = *(*lines)++;
    *w = '\0';
    return True;
}

/**
* readWords: Goes through string array lines and adds each word to the binary
*   tree starting at node w.
*
* Returns: The number of words read.
*/
int readWords(char **lines, word **w, int n) {
    char *nextw = malloc(sizeof(char[MAXSTRLEN]));
    int count = 0;
    for (int i = 0; i < n; i++) {
        while (getWord(&(lines[i]), nextw)) {
            *w = add2tree(*w, strlower(nextw), i + 1);
            count++;
        }
    }
    return count;
}

/**
* getfile: Reads file fn and stores each line of text into string array lines.
*   Uses fgets(), so the newline character at the end of each line is included.
*   Assumes lines is empty and needs to be resized as new lines are added.
*
*  Returns: The number of lines read, or -1 if an error occured.
*/
int getfile(char ***lines, char fn[], int len) {
    FILE *fp = fopen(fn, "r");
    if (fp == NULL)
        return -1;

    int i = 0;
    do {
        // NOTE: Did the *lines realloc wrong in previous exercises by using
        //       sizeof(char**), although it sort of works itself out.
        i++;
        if ((*lines = realloc(*lines, sizeof(char*) * i)) == NULL) {
            printf("ERROR: Couldn't resize string array to fit %d lines", i);
            return -1;
        }
        if (((*lines)[i - 1] = malloc(sizeof(char) * len)) == NULL) {
            printf("ERROR: Couldn't create string pointer to line %d\n", i);
            return -1;
        }
    } while (fgets((*lines)[i - 1], len, fp) != NULL);

    return i - 1;  // line i will be null
}

/**
* getWordCount: Gets the total word count of all words in the document by
*   going through the binary tree starting at node w and adding the number
*   of lines each word is found to a running total.
*
* Returns: The total number of words in the document.
*/
int getWordCount(word *w) {
    int l = 0, r = 0;
    if (w->left != NULL)
        l += getWordCount(w->left);
    if (w->right != NULL)
        r += getWordCount(w->right);
    return l + r + w->count;
}

/**
* add2tree: Looks for a node for word s in the binary tree starting from node
*   w. If found, adds int ln to the list of ints that word is found. If not,
*   a new node is created, and the int is added to that node's list of ints.
*
* Returns: The node for word s (whether existing already, or newly created).
*/
word *add2tree(word *w, char *s, int ln) {
    int cond;
    if (w == NULL) {
        if ((w = (word*)malloc(sizeof(word))) == NULL) {
            printf("ERROR: Couldn't create tree node for word '%s'\n",
                w->wordstr);
            return NULL;
        }
        w->wordstr = safecopy(s);
        w->foundOn = NULL;
        w->count = 1;
        append(&(w->foundOn), ln, w->count);
        w->left = w->right = NULL;
    }
    else if ((cond = strcmp(s, w->wordstr)) == 0) {
        (w->count)++;
        append(&(w->foundOn), ln, w->count);
    }
    else if (cond < 0)
        w->left = add2tree(w->left, s, ln);
    else
        w->right = add2tree(w->right, s, ln);
    return w;
}

/**
* safecopy: Deep copies word s into a new string. Does error checking.
*
* Returns: A pointer to the new, copied string
*/
char *safecopy(char *s) {
    char *p;
    if ((p = malloc(strlen(s) + 1)) == NULL) {
        printf("ERROR: Couldn't copy word '%s'\n", s);
        return NULL;
    }
    strcpy(p, s);
    return p;
}

/**
 * strlower: Convers string s to all lower case
 *
 * Returns: Pointer to new string in all lower case
 */
char *strlower(char *s) {
    int l = strlen(s) + 1;
    char *p = malloc(sizeof(char) * l);
    while (*p++ = tolower(*s++));
    return p - l;
}

/**
* append: Appends int i to the end of int array arr
*/
void append(int **arr, int i, int len) {;
    if (*arr == NULL) {
        if ((*arr = (int*)malloc(sizeof(int))) == NULL) {
            printf("ERROR: Couldn't create int array\n");
            return;
        }
    }
    else
        *arr = realloc(*arr, sizeof(int) * len);
    *(*arr + len - 1) = i;
}

/**
* printTree: Prints each node in the binary tree starting at node w. Each node
*   is on its own line.
*/
void printTree(word *w) {
    if (w->left != NULL)
        printTree(w->left);
    if (gettf(w) < TFLIM) {
        printf("%s       \t\tLines", w->wordstr);
        for (int i = 0; i < w->count; i++) {
            printf(" %d", *(w->foundOn + i));
            if (i < w->count - 1)
                printf(", ");
        }
        printf("\n");
    }
    if (w->right != NULL)
        printTree(w->right);
}

/**
* printUsageStatement: Prints the usage statement.
*/
void printUsageStatement(void) {
    printf("\n  USAGE:\n");
    printf("\t06E03 filename\n\n");
    printf("  where\n");
    printf("    filename\t\tName of the file to use.\n");
}

/**
 * gettf: Finds the term frequency
 *
 * Returns: TF as float
 */
float gettf(word *w) {
    return (float)w->count / (float)totalwords;
}