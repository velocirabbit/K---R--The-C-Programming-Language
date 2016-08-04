/* 05E19 (p. 126) */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100        // max char length of token
#define BUFFSIZE 100        // max storage size of char input buffer

enum { NAME, PARENS, BRACKETS };
typedef enum { False = 0, True } bool;

void preclean(void);
void processdcl(void);
void processundcl(void);
void dcl(void);
void dirdcl(void);
void ungetch(int c);
int getch(bool ignWhtSp);
int gettoken(void);
int getprecedence(char c);

int tokentype;              // type of last token
int buffp = 0;              // next free position in buff
char buff[BUFFSIZE];        // buffer for ungetch
char token[MAXTOKEN];       // last token string
char name[MAXTOKEN];        // identifier name
char datatype[MAXTOKEN];    // data type = char, int, etc.
char out[1000];             // output string

int main(void) {  // convert declaration to words
    printf("What would you like to do? Type 'dcl' to perform dcl parsing, and\n");
    printf("'undcl' to perform undcl parsing.\n\n");

    gettoken();

    if (strcmp(token, "dcl") == 0) {
        printf("\nOkay! Type in the declaration strings you want to parse.\n\n");
        preclean();
        processdcl();
    }
    else if (strcmp(token, "undcl") == 0) {
        printf("\nOkay! Describe what the declaration should do.\n\n");
        preclean();
        processundcl();
    }
    else
        printf("ERROR: Not an option. Sorry!\n");

    printf("\n\n");
    system("pause");
    return 0;
}

/**
* preclean: Minor cleanup
*/
void preclean(void) {
    strcpy(token, "");
    buffp = 0;
}

/**
* processdcl: Runs the dcl parsing function on the input string
*/
void processdcl(void) {
    while (gettoken() != EOF) {
        strcpy(datatype, token);    // 1st token in line is the data type
        out[0] = '\0';
        dcl();                      // parse rest of line
        if (tokentype != '\n')
            printf("syntax error\n");
        printf("\t%s: %s %s\n", name, out, datatype);
    }
}

/**
* processundcl: Runs the undcl parsing function on the input string
*/
void processundcl(void) {
    int type;
    int lastprec = -1;
    char temp[MAXTOKEN];

    while (gettoken() != EOF) {
        strcpy(out, token);
        while ((type = gettoken()) != '\n') {
            if (type == PARENS || type == BRACKETS) {
                if (getprecedence(type) > lastprec && lastprec != -1) {
                    sprintf(temp, "(%s)", out);
                    strcpy(out, temp);
                }
                strcat(out, token);
            }
            else if (type == '*') {
                sprintf(temp, "*%s", out);
                strcpy(out, temp);
            }
            else if (type == NAME) {
                sprintf(temp, "%s %s", token, out);
                strcpy(out, temp);
            }
            else
                printf("ERROR: Invalid input at %s\n", token);
            lastprec = (type != NAME) ? getprecedence(type) : -1;
        }
        printf("\t%s\n", out);
    }
}

/**
* dcl: Parse a declarator
*/
void dcl(void) {
    int ns;

    for (ns = 0; gettoken() == '*'; )
        ns++;  // count the *'s
    dirdcl();
    while (ns-- > 0)
        strcat(out, " pointer to");
}

/**
* dirdcl: Parse a direct declarator
*/
void dirdcl(void) {
    int type;
    if (tokentype == '(') {         // ( dcl )
        dcl();
        if (tokentype != ')')
            printf("ERROR: Missing ')'\n");
    }
    else if (tokentype == NAME)
        strcpy(name, token);        // variable name
    else
        printf("ERROR: Expected name or (dcl)\n");
    while ((type = gettoken()) == PARENS || type == BRACKETS)
        if (type == PARENS)
            strcat(out, " function returning");
        else {
            strcat(out, " array");
            strcat(out, token);
            strcat(out, " of");
        }
}

/**
* ungetch: Pushes a character back onto the input buffer.
*/
void ungetch(int c) {
    if (buffp >= BUFFSIZE) {
        printf("ERROR: Can't push char '%c' back onto input buffer.\n", c);
        printf("       Too many characters.\n");
    }
    else
        buff[buffp++] = c;
}

/**
* getch: Get a (possibly pushed back) character from the input.
*/
int getch(bool ignWhtSp) {
    char c = (buffp > 0) ? buff[--buffp] : getchar();
    if (ignWhtSp && (c == ' ' || c == '\t'))
        while ((c = (buffp > 0) ? buff[--buffp] : getchar()) == ' ' || c == '\t');

    return c;
}

/**
* gettoken: Skips blanks and tabs, then finds the next token in the input; a
*   "token" is a name, a pair of parenthesis, a pair of brackets perhaps
*   including a number, or any other single character.
*
* Returns: Either an enumerated int signifying the token string type, or a
*   char of the token itself.
*/
int gettoken(void) {
    int c;
    char *p = token;

    while ((c = getch(False)) == ' ' || c == '\t');  // skip spaces and tabs
    if (c == '(') {
        if ((c = getch(True)) == ')') {
            strcpy(token, "()");
            return tokentype = PARENS;
        }
        else {
            ungetch(c);
            return tokentype = '(';
        }
    }
    else if (c == '[') {
        for (*p++ = c; (*p++ = getch(True)) != ']'; );  // get everything between
        *p = '\0';
        return tokentype = BRACKETS;
    }
    else if (isalpha(c)) {
        for (*p++ = c; isalnum(c = getch(False)); )
            *p++ = c;
        *p = '\0';
        ungetch(c);
        return tokentype = NAME;
    }
    else
        return tokentype = c;
}

/**
 * getprecedence: Returns an operation precedence value for the operation defined
 *   by string c. Note that this should probably only be used for comparisons.
 * 
 * Returns: int symbolizing precedence order (larger value is higher precedence)
 */
int getprecedence(char c) {
    char lvl14[] = { PARENS, BRACKETS, '.' };
    char lvl13[] = { '|', '-', '+', '*', '&' };
    for (int i = 0; i < 3; i++)
        if (c == lvl14[i])
            return 14;
    for (int i = 0; i < 5; i++)
        if (c == lvl13[i])
            return 13;
    return 0;
}