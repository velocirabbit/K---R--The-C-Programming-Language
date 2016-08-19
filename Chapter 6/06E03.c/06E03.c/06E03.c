/* 06E03.c (p. 143) */
#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINECOUNT 1000
#define MAXSTRLEN    1000
#define TFLIM        0.65
#define WORDCOUNT (sizeof(w->foundOn) / sizeof(int))

typedef enum { False = 0, True } bool;

typedef struct s_word {
    char *wordstr;
    int foundOn[MAXLINECOUNT];
    struct s_word *left;
    struct s_word *right;
} word;

bool parseArgs(int argc, char *argv[]);
bool getWord(char **line, char *w);
int readWords(char **lines, word *w, int n);
int getfile(char ***lines, char fn[], int n);
int getWordCount(word *w);
word *add2tree(word *w, char *s, int ln);
void printTree(word *w);
void printUsageStatement(void);

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
    readWords(lines, root, n);
    // Print it all out
    printTree(root);

    // Clean up pointers
    for (--n; n >= 0; n--)
        free(lines[n]);
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
    //TODO: getWord
}

/**
 * readWords: Goes through string array lines and adds each word to the binary
 *   tree starting at node w.
 *
 * Returns: The number of words read.
 */
int readWords(char **lines, word *w, int n) {
    char *nextw = malloc(sizeof(char) * n);
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (getWord(&(lines[i]), nextw)) {
            add2tree(w, nextw, i);
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
    return l + r + WORDCOUNT;
}

/**
 * add2tree: Looks for a node for word s in the binary tree starting from node
 *   w. If found, adds int ln to the list of ints that word is found. If not,
 *   a new node is created, and the int is added to that node's list of ints.
 *
 * Returns: The node for word s (whether existing already, or newly created).
 */
word *add2tree(word *w, char *s, int ln) {
    //TODO: add2tree
}

/**
 * printTree: Prints each node in the binary tree starting at node w. Each node
 *   is on its own line.
 */
void printTree(word *w) {
    if (w->left != NULL)
        printTree(w->left);
    printf("%s       \tLines", w->wordstr);
    for (int i = 0; i < WORDCOUNT; i++) {
        printf(" %d", (w->foundOn)[i]);
        if (i < WORDCOUNT - 1)
            printf(", ");
    }
    printf("\n");
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