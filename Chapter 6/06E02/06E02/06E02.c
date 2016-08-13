/* 06E02.c (p. 143) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRLEN 1000

/*
 * typedefs, enums, and structs
 */
typedef enum e_bool { False = 0, True } bool;  // bool feels better lowercase
typedef enum e_vartypes {
    VOID, CHAR, SHORT, INT, LONG, FLOAT, DOUBLE,
    SIGNED, UNSIGNED, STRUCT, UNION, ENUM, TYPEDEF
} vartypes;

typedef struct s_varname {
    char *name;                 // name of var. Check initial chars to group
    vartypes t;                 // variable type
    struct s_varname *left;     // left child
    struct s_varname *right;    // right child
} varname;

/*
 * Function prototypes
 */
int getfile(char ***lines, char fn[], int lnlen);
int findVarNames(varname *vn, char ***lines);
varname *add2tree(varname *vn, char *w);
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

    int numlines;
    char **lines = malloc(sizeof(char**));
    if (lines == NULL) {
        printf("ERROR: Couldn't create a pointer to a string array to store\n");
        printf("       file contents in.\n");
        return 1;
    }
    if ((numlines = getfile(&lines, argv[1], MAXSTRLEN)) <= -1)
        return 1;  // Will handle printing errors in getfile()
    else if (numlines == 0) {
        printf("ERROR: File is empty. Nothing to read.\n");
        return 0;
    }

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
int getfile(char ***lines, char fn[], int lnlen) {

}

/**
 * findVarNames: Goes through the lines of text/code in string array lines and
 *   looks for all of the unique variable names. Creates a varname struct for
 *   each and adds it to the binary tree that has initial node pointed to by vn.
 *
 * Returns: The number of nodes added.
 */
int findVarName(varname *vn, char ***lines) {

}

/**
 * add2tree: Looks in the existing binary tree (starting from the node pointed
 *   to by vn) for a node with name w. If it doesn't exist yet, a new node is
 *   created and added to the tree.
 *
 * Returns: The pointer to the calling node (vn).
 */
varname *add2tree(varname *vn, char *w) {

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