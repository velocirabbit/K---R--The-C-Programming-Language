/* 06E02.c (p. 143) */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRLEN 1000

/*
 * typedefs, enums, and structs
 */
typedef enum e_bool { False = 0, True } bool;  // bool feels better lowercase
typedef enum e_vartype {
    VOID, CHAR, SHORT, INT, LONG, FLOAT, DOUBLE,
    SIGNED, UNSIGNED, STRUCT, UNION, ENUM, TYPEDEF
} vartype;

typedef struct s_varname {
    char *name;                 // name of var. Check initial chars to group
    vartype t;                 // variable type
    struct s_varname *left;     // left child
    struct s_varname *right;    // right child
} varname;

/*
 * Function prototypes
 */
int getfile(char ***lines, char fn[], int len);
int findVarNames(varname **vn, char ***lines, int n);
varname *add2tree(varname *vn, char *w, vartype t);
varname *talloc(void);
char *safecopy(char *s);
void treeprint(varname *vn);
void checkArgs(int argc, char *argv[]);
void printUsageStatement(void);

/*
 * Global variables and constants
 */
int varChars = 6;
const char *TYPES[] = {
    "void", "char", "short", "int", "long",
    "float", "double", "signed", "unsigned"
};

/**
 * main()
 */
int main(int argc, char *argv[]) {
    checkArgs(argc, argv);

    int n;
    char **lines = malloc(sizeof(char**));
    if (lines == NULL) {
        printf("\nERROR: Couldn't create a pointer to a string array to store\n");
        printf("       file contents in.\n");
        return 1;
    }
    if ((n = getfile(&lines, argv[1], MAXSTRLEN)) <= -1)
        return 1;  // Will handle printing errors in getfile()
    else if (n == 0) {
        printf("\nERROR: File is empty. Nothing to read.\n");
        return 0;
    }

    // Find each unique variable name and save it to the tree
    varname *root;
    findVarNames(&root, &lines, n);
    // Print it all out
    treeprint(root);

    // Clean up pointers
    for (--n; n >= 0; n--)
        free(lines[n]);
    free(lines);

    printf("\n\n");
    return 0;
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
    if (fp == NULL) {
        printf("\nERROR: Couldn't open and read file'%s'\n", fn);
        return -1;
    }

    int i = 0;
    do {
        i++;
        // These memory spaces will be freed in main()
        if ((*lines = realloc(*lines, sizeof(char**) * i)) == NULL) {  // ptr to str
            printf("\nERROR: Couldn't reallocate memory space for string array\n");
            return -1;
        }
        if (((*lines)[i - 1] = malloc(sizeof(char) * len)) == NULL) {  // str in arr
            printf("\nERROR: Couldn't reallocate memory space for text line %d\n",
                len);
            return -1;
        }
    } while (fgets((*lines)[i - 1], len, fp) != NULL);

    return i - 1;
}

/**
 * findVarNames: Goes through the n lines of text/code in string array lines and
 *   looks for all of the unique variable names. Creates a varname struct for
 *   each and adds it to the binary tree that has initial node pointed to by vn.
 *
 * Returns: The number of nodes added.
 */
int findVarNames(varname **vn, char ***lines, int n) {

    return 0;
}

/**
 * add2tree: Looks in the existing binary tree (starting from the node pointed
 *   to by vn) for a node with name w. If it doesn't exist yet, a new node is
 *   created and added to the tree. Variable names should only be used once
 *   regardless of type, so don't need to check vartype if a duplicate name is
 *   found (which shouldn't happen, anyways).
 *
 * Returns: The pointer to the calling node (vn).
 */
varname *add2tree(varname *vn, char *w, vartype t) {
    int cond;

    if (vn == NULL) {  // new varname to add to the tree
        if ((vn = talloc()) == NULL)
            printf("\nERROR: failed ptr node '%s'\n", w);
        if ((vn->name = safecopy(w)) == NULL)
            printf("\nERROR; failed ptr node name '%s'\n", w);
        vn->t = t;
        vn->left = vn->right = NULL;
    }
    else if ((cond = strcmp(w, vn->name)) < 0)
        vn->left = add2tree(vn->left, w, t);
    else if (cond > 0)
        vn->right = add2tree(vn->right, w, t);
    // if (cond == 0), do nothing since the varname has been encountered before
    return vn;
}

/**
 * talloc: Safetly created a pointer of type varname (does error checking to
 *   and prints a message if an error happens).
 *
 * Returns: Created pointer of type varname
 */
varname *talloc(void) {
    varname *t = (varname*)malloc(sizeof(varname));
    if (t == NULL)
        printf("\nERROR: Couldn't allocate memory for new node in tree\n");
    return t;
}

/**
 * safecopy: Creates a new char pointer, then does an error check to make sure
 *   nothing went wrong (prints a message if anything did). If nothing went
 *   wrong, copies string s into the pointer.
 *
 * Returns: Created char pointer.
 */
char *safecopy(char *s) {
    char *t = (char*)malloc(sizeof(strlen(s) + 1));  // +1 for '\0'
    if (t == NULL)
        printf("\nERROR: Couldn't allocate memory to copy string into\n");
    else
        strcpy(t, s);
    return t;
}

/**
 * treeprint: Prints the binary tree vn in order
 */
void treeprint(varname *vn) {
    if (vn != NULL) {
        treeprint(vn->left);
        printf("%s\n", vn->name);
        treeprint(vn->right);
    }
}

/**
* checkArgs: Wrapper function that's called by main() to parse the args
*/
void checkArgs(int argc, char *argv[]) {
    switch (argc) {
        case 2: break;  // Do nothing and keep default 6 chars if not specified
        case 3:
            if ((varChars = atoi(argv[2])) <= 0) {
                printf("\nERROR: Must use a valid, positive integer!\n");
                printf("       Defaulting to 6 characters....\n");
                varChars = 6;
            }
            break;
        default:
            printUsageStatement();
            break;
    }
}

/**
 * printUsageStatement: Prints the usage statement lolz
 */
void printUsageStatement(void) {
    printf("\n\n  USAGE:\n");
    printf("\t06E02 filename [num]\n\n");
    printf("  where\n");
    printf("    filename\t\tName of the file to use.\n");
    printf("\n    Options:\n");
    printf("\tnum       \tNumber of initial characters in a variable name\n");
    printf("\t          \tto compare. Defaults to 6 characters.\n\n");
}