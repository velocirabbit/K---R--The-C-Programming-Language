/* 05E10.c (p. 118) */
#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { False = 0, True } bool;

#define MAXLINELEN 1000   // max operator and operand lengths
#define STACKSIZE 1000    // max valueStack size
#define STROPCOUNT 14     // number of string operands
#define ANS "ans"         // ANS variable name
#define ZERO "0.0"        // initial ANS variable value

// Operand codes
#define STRERR 0          // indicates an error while reading a string operand
#define NUMBER '0'        // indicates a number was found in getop()
#define DUPE 'd'          // indicates a dupe() operand. Next two ops are p and c
#define SWAP 's'          // indicates a swap() operand. Next two ops are p1 and p2
#define DEL  'x'          // indicates a del() operand. Next op is p
#define PRINT 'p'         // indicates a print() operand. Next op is p
#define CLEAR 'c'         // indicates a clear() operand
#define SINE 'n'          // sin() operand
#define COS 'o'           // cos() operand
#define TAN 't'           // tan() operand
#define EXP 'e'           // exp() operand
#define POW 'w'           // pow() operand
#define LN 'l'            // ln() operand
#define LOG 'g'           // log() operand
#define MEMORIZE 'm'      // memorize() operand
#define RECALL 'r'        // recall() operand

bool isNumber(char c[]);
bool operate(char ***op);
char getOp(char c[]);
double modulo(double n, double d);
double pop(void);
void push(double n);
void dupe(int p, char c);
void swap(int p1, int p2);
void del(int p);
void print(int p);

int sPos = 0;
double stack[STACKSIZE];

// Operands
char strOps[][STROPCOUNT] = { "dupe", "swap", "del", "print",
                              "clr", "sin", "cos", "tan",
                              "exp", "pow", "ln", "log",
                              "mem", "re" };

int main(int argc, char *argv[]) {
    bool result = True;
    if (argc == 1) {  // math equation is missing
        printf("ERROR: Please enter a math equation in reverse Polish notation\n");
        result = False;
    }
    
    // Go through each operator and number in the equation
    while (*++argv != NULL && result) {
        if (isNumber(*argv))
            push(atof(*argv));
        else
            result = operate(&argv);
    }
    
    // Print resulting answer
    if (result)
        printf("%g\n", pop());
    return 1 - result;
}

/**
 * isNumber: Evaluates string c to see if it's a floating point number. c may start with
 *   a +/- sign, and may have a decimal point.
 *
 * Returns: True if c is a number, False if otherwise
 */
bool isNumber(char c[]) {
    if (*c == '-' || *c == '+') {
        if (strlen(c) == 1)
            return False;
        else
            c++;
    }
    for (c; isdigit(*c); c++);
    if (*c == '.')
        c++;
    for (c; isdigit(*c); c++);
    return (*c == '\0') ? True : False;
}


/**
 * operate: Evaluates the next operation (won't be a number, but subsequent operators
 *   may), pushing and popping numbers from the stack as needed.
 *
 * Returns: True if everything evaluated okay, False if anything wrong happened
 */
bool operate(char ***op) {
    bool result = True;
    char c = getOp(**op);
    switch (c) {
        case '+':
            push(pop() + pop());
            break;
        case '*':
            push(pop() * pop());
            break;
        case '-': case '/': case '%': case POW:
        {
            // These all need a temp variable
            double temp = pop();
            switch (c) {
                case '-':
                    push(pop() - temp);
                    break;
                case '/':
                    push(pop() / temp);
                    break;
                case '%':
                    push(modulo(pop(), temp));
                    break;
                case POW:
                    push(pow(pop(), temp));
                    break;
            }
            break;
        }
        case DUPE:
        {
            if (getOp(*++(*op)) == NUMBER) {
                int p = atoi(**op);
                char c = getOp(*++(*op));
                dupe(p, c);
            }
            else {
                printf("ERROR: Could not duplicate value\n");
                result = False;
            }
            break;
        }
        case SWAP:
        {
            int p1, p2;
            if (getOp(*++(*op)) == NUMBER)
                p1 = atoi(**op);
            else {
                printf("ERROR: Could not swap values\n");
                result = False;
            }
            if (getOp(*++(*op)) == NUMBER)
                p2 = atoi(**op);
            else {
                printf("ERROR: Could not swap values\n");
                result = False;
            }
            swap(p1, p2);
            break;
        }
        case DEL:
        {
            if (getOp(*++(*op)) == NUMBER)
                del(atoi(**op));
            else {
                printf("ERROR: Could not delete value\n");
                result = False;
            }
            break;
        }
        case PRINT:
        {
            if (getOp(*++(*op)) == NUMBER) {
                print(atoi(**op));
            }
            else {
                printf("ERROR: Cannot print value\n");
                result = False;
            }
            break;
        }
        case CLEAR:
            sPos = 0;
            break;
        case SINE:
            push(sin(pop()));
            break;
        case COS:
            push(cos(pop()));
            break;
        case TAN:
            push(tan(pop()));
            break;
        case EXP:
            push(exp(pop()));
            break;
        case LN:
            push(log(pop()));
            break;
        case LOG:
            push(log10(pop()));
            break;
        case MEMORIZE:
            break;
        case RECALL:
            break;
        case STRERR:
            result = False;
            break;
        default:
            break;
    }
    return result;  //TODO: FIX
}

/**
 * getOp: Checks to see if string c matches any of the op codes or even a number.
 *
 * Returns: The matching op code if c matches one, or code STRERR if not.
 */
char getOp(char c[]) {
    if (isNumber(c))
        return NUMBER;
    else if (strlen(c) == 1) {  // single length operators can just be returned as is
        return c[0];
    }
    else {
        int i;
        for (i = 0; i < STROPCOUNT && strcmp(c, strOps[i]) != 0; i++);
        switch (i) {
            case 0:
                return DUPE;
                break;
            case 1:
                return SWAP;
                break;
            case 2:
                return DEL;
                break;
            case 3:
                return PRINT;
                break;
            case 4:
                return CLEAR;
                break;
            case 5:
                return SINE;
                break;
            case 6:
                return COS;
                break;
            case 7:
                return TAN;
                break;
            case 8:
                return EXP;
                break;
            case 9:
                return POW;
                break;
            case 10:
                return LN;
                break;
            case 11:
                return LOG;
                break;
            case 12:
                return MEMORIZE;
                break;
            case 13:
                return RECALL;
                break;
            default:
                return STRERR;
                break;
        }

    }
}

/**
 * modulo: Literally just the modulo operator (%), but allows for floating point
 *   operands.
 *
 * Returns a value as expected, and can be used with negative numbers.
 */
double modulo(double n, double d) {
    int nSign = (n < 0) ? -1 : 1;
    int dSign = (d < 0) ? -1 : 1;
    int m;
    for (m = 1; dSign * d * m <= nSign * n; m++);
    return n - (nSign * dSign * d * --m);
}

/**
 * pop: Gets the topmost value in the stack if available.
 *
 * Returns: The number if available, 0.0 if not
 */
double pop(void) {
    if (sPos > 0)
        return stack[--sPos];
    else
        printf("ERROR: Stack is empty; nothing to pop off\n");
    return (double)False;
}

/**
 * push: Puts double n into the next free position in the value stack (unless the stack
 *   is full).
 */
void push(double n) {
    if (sPos < STACKSIZE - 1)
        stack[sPos++] = n;
    else
        printf("ERROR: Stack full; can't put another number in\n");
}

/*
 * dupe: Duplicates the number at position p in valueStack. c is a code that determines
 *   where the duplicated value will go:
 *
 *      'a': adjacent to the original. Every value after p is shifted up by 1, and the
 *             dupe takes position p + 1
 *      'z': end of the stack. The dupe will take position vsPos (default)
 *
 *      anything else: Uses the default ('z'), but prints an error message indicating so
 *
 *   If p >= vsPos, nothing happens.
 */
void dupe(int p, char c) {
    if (p < sPos) {
        switch (c) {
            case 'a':
                for (int i = sPos; i > p; i--)
                    stack[i] = stack[i - 1];
                break;
            case 'z':
                push(stack[p]);
                break;
            default:
                printf("ERROR: Unrecognized dupe() option '%c' - placing at end instead\n", c);
                push(stack[p]);
                break;
        }
        sPos++;
    }
    else {
        printf("ERROR: Can't dupe value at position '%d'\n", p);
        printf("       Outside of stack's range (sPos = %d)\n", sPos);
    }
}

/**
 * swap: Swap the values at positions p1 and p2 in the stack. If either >= sPos, nothing
 *   happens.
 */
void swap(int p1, int p2) {
    if (p1 < sPos && p2 < sPos) {
        double temp = stack[p1];
        stack[p1] = stack[p2];
        stack[p2] = temp;
    }
    else {
        printf("ERROR: Cannot swap values at positions %d and %d\n", p1, p2);
        printf("       One or both are outside of stack's range (sPos = %d)\n", sPos);
    }
}

/**
 * del: Delete the value at position p in the stack. Everything after is shifted down by
 *   1 position. If p >= sPos, nothing happens.
 */
void del(int p) {
    if (p > sPos) {
        for (int i = p; p < sPos; p++)
            stack[i] = stack[i + 1];
        sPos--;
    }
    else {
        printf("ERROR: Cannot delete value at position %d\n", p);
        printf("       Outside of stack's range (sPos = %d)\n", sPos);
    }
}

/**
 * print: Prints the number at position p in the stack.
 */
void print(int p) {
    if (p < sPos)
        printf("stack[%d] = %.8g\n", p, stack[p]);
    else {
        printf("ERROR: Cannot print value at position %d\n", p);
        printf("       Outside of stack's range (sPos = %d)\n", sPos);
    }
}