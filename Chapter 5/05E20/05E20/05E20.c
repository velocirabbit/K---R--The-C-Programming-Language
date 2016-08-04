/* 05E20 (p. 126) */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 1000        // max char length of token
#define BUFFSIZE 1000        // max storage size of char input buffer
#define NUMTYPES 9
#define NUMQUALS 2

enum { NAME, PARENS, BRACKETS, TYPE, QUAL, ARGS };
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

bool inArgs = False;
int tokentype;              // type of last token
int lasttype;               // type of token before last
int buffp = 0;              // next free position in buff
char buff[BUFFSIZE];        // buffer for ungetch
char token[MAXTOKEN];       // last token string
char name[MAXTOKEN];        // identifier name
char datatype[MAXTOKEN];    // data type = char, int, etc.
char out[1000];             // output string

const char types[][MAXTOKEN] = { "void", "char", "short", "int", "long",
                                "float", "double", "signed", "unsigned" };
const char quals[][MAXTOKEN] = { "const", "volatile" };

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
        if (tokentype == TYPE) {
            strcpy(datatype, token);    // 1st token in line is the data type
            while (gettoken() == TYPE) {
                strcat(datatype, " ");
                strcat(datatype, token);
            }
            ungetch(tokentype);
        }
        else
            printf("ERROR: Declaration should start with a data type\n");
        out[0] = '\0';
        dcl();                      // parse rest of line
        if (tokentype != '\n')
            printf("syntax error\n");
        printf("\t%s: %s %s\n\n", name, out, datatype);
    }
}

/**
* processundcl: Runs the undcl parsing function on the input string
*/
void processundcl(void) {
    //TODO: dcl works, debug undcl
    int type;
    int lastprec = -1;
    char temp[MAXTOKEN];

    while (gettoken() != EOF) {
        strcpy(out, token);
        while ((type = gettoken()) != '\n') {
            switch (type) {
                case PARENS: case BRACKETS:
                {
                    if (getprecedence(type) > lastprec && lastprec != -1) {
                        sprintf("temp, (%s)", out);
                        strcpy(out, temp);
                    }
                    strcat(out, token);
                    break;
                }
                case '*':
                {
                    sprintf(temp, "*%s", out);
                    strcpy(out, temp);
                    break;
                }
                case NAME:
                {
                    if (!inArgs)
                        sprintf(temp, "%s %s", token, out);
                    else
                        sprintf(temp, "%s %s,", out, token);
                    strcpy(out, temp);
                    break;
                }
                case TYPE: case QUAL:
                {
                    if (!inArgs)
                        strcat(out, " ");
                    strcat(out, datatype);
                    break;
                }
                case ARGS:
                {
                    if (inArgs) {
                        out[strlen(out) - 2] = ')';
                        inArgs = False;
                    }
                    else {
                        strcat(out, "(");
                        inArgs = True;
                    }
                    break;
                }
                default:
                {
                    printf("ERROR: Invalid input at %s\n", token);
                    break;
                }
            }
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
    int type = -1;
    switch (tokentype) {
        case '(': case ARGS:            // ( dcl ), args
        {
            if (tokentype == ARGS && !inArgs) {
                strcat(out, " function (with args of");
                inArgs = True;
                type = ARGS;
            }
            dcl();
            if (tokentype != ')')
                printf("ERROR: Missing ')'\n");
            break;
        }
        case NAME: case TYPE: case QUAL:
        {
            strcpy(name, token);        // variable name
            break;
        }
        default:
        {
            printf("ERROR: Expected name or (dcl)\n");
            break;
        }
    }
    while ((type = gettoken()) == PARENS || type == BRACKETS || type == ARGS)
        if (type == PARENS || type == ARGS) {
            if (type == ARGS)
                inArgs = True;
            char args[MAXTOKEN] = "", dt[MAXTOKEN], n[MAXTOKEN];
            int nm = 0;
            bool newdt = True, newn = True;
            while (inArgs) {
                int t;
                if ((t = gettoken()) == TYPE || t == QUAL) {
                    if (newdt)
                        strcpy(dt, token);
                    else {
                        strcat(dt, " ");
                        strcat(dt, token);
                    }
                    nm++;
                    newdt = False;
                }
                else if (t == NAME) {
                    if (newn)
                        strcpy(n, token);
                    else
                        strcat(n, token);
                    newn = False;
                }
                else if (t == ',' || t == ')') {
                    if (nm > 1)
                        strcat(args, ", ");
                    else
                        strcpy(args, " (with args of ");
                    strcat(args, n);
                    strcat(args, ": ");
                    strcat(args, dt);
                    newdt = True;
                    newn = True;
                    if (t == ')') {
                        strcat(args, ")");
                        inArgs = False;
                    }
                }
            }
            strcat(args, " ");
            strcat(out, " function");
            strcat(out, args);
            strcat(out, "returning");
        }
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
    lasttype = tokentype;

    c = getch(True);  // get next nonspace character
    if (c == '(') {
        if ((c = getch(True)) == ')') {
            strcpy(token, "()");
            return tokentype = PARENS;
        }
        else {
            ungetch(c);
            if (lasttype == NAME || lasttype == ')')
                return tokentype = ARGS;
            else
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
        for (int i = 0; i < NUMTYPES; i++)
            if (strcmp(token, types[i]) == 0)
                return tokentype = TYPE;
        for (int i = 0; i < NUMQUALS; i++)
            if (strcmp(token, quals[i]) == 0)
                return tokentype = QUAL;
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