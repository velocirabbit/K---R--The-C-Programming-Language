/* 04E03.c (p. 79) */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXOP 1000     // maximum character size of operand or operator
#define MAXVAL 1000    // max size of the value stack
#define BUFFSIZE 1000  // character input buffer size
#define NUMBER '0'     // represents a number was found

void push(double f);
double pop(void);
double modulo(double n, double d);
int getop(char s[]);
int getch(void);
void ungetch(int bufp);

int sp = 0;           // next free index in the value stack
int buffp = 0;
double val[MAXVAL];   // value stack
char buff[BUFFSIZE];  // character input buffer

main() {
    int type;
    double op2;
    char s[MAXOP];
    
    while ((type = getop(s)) != EOF) {
        switch (type) {
            case NUMBER:  // number
                push(atof(s));
                break;
            case '+':     // addition
                push(pop() + pop());
                break;
            case '*':     // multiplication
                push(pop() * pop());
                break;
            case '-':     // subtraction
                op2 = pop();
                push(pop() - op2);
                break;
            case '/':     // division
                op2 = pop();
                if (op2 != 0.0)
                    push(pop() / op2);
                else
                    printf("ERROR: Division by 0\n");
                break;
            case '%':     // modulo
                op2 = pop();
                if (op2 != 0.0)
                    push(modulo(pop(), op2));
                else
                    printf("ERROR: Modulo by 0\n");
                break;
            case '\n':    // solve
                printf("\t%.8g\n", pop());
                break;
            default:
                printf("ERROR: Unknown command '%s'\n", s);
                break;
        }
    }
    return 0;
}

/* 
 * getop: gets the next operator or numeric operand. If there are any characters in the
 *   character input buffer, it'll pull from there, first.
 */
int getop(char s[]) {
    int i = 0;
    while ((s[i] = getch()) == ' ' || s[i] == '\t');  // skip white space characters

    if (!isdigit(s[i]) && s[i] != '.') {
        if (s[i] == '-' || s[i] == '+') {
            s[++i] = getch();
            if (s[i] == ' ' || s[i] == '\t') {
                ungetch(s[i]);
                return s[--i];  // the '-' or '+' was just the operator after all
            }
        }
        else
            return s[i];        // definitely not a number
    }
    if (isdigit(s[i]))  // collect integer part
        while (isdigit(s[++i] = getch()));
    if (s[i] == '.')    // collect fraction part
        while (isdigit(s[++i] = getch()));

    if (s[i] != EOF)
        ungetch(s[i]);
    s[i] = '\0';
    return NUMBER;

}

/*
 * getch: Gets a character from the input. May be pushed back
 */
int getch(void) {
    return (buffp > 0) ? buff[--buffp] : getchar();
}

/*
 * ungetch: Puts character c back into the input buffer at the last open position
 */
void ungetch(int c) {
    if (buffp > BUFFSIZE)
        printf("ERROR: Character input buffer full! Too many characters!\n");
    else
        buff[buffp++] = c;
}

/* 
 * modulo: Allows for modulo (%) operator to be used on doubles and floats.
 *   Returns the remainder (as a double) when n is divided by d.
 */
double modulo(double n, double d) {
    int i;
    int nSign = n < 0 ? -1 : 1;
    int dSign = d < 0 ? -1 : 1;
    for (i = 1; dSign * d * i <= nSign * n; i++);  // increments i until d * i > n
    return nSign * (nSign * n - (dSign * d * --i));
}

/*
 * push: Puts f onto the value stack
 */
void push(double f) {
    if (sp < MAXVAL)
        val[sp++] = f;
    else
        printf("ERROR: Stack full - can't push %g\n", f);
}

/*
 * pop: Pops and returns the last value in the value stack
 */
double pop(void) {
    if (sp > 0)
        return val[--sp];
    else {
        printf("ERROR: Stack empty\n");
        return 0.0;
    }
}