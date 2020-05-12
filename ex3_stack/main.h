
#ifndef UNTITLED1_CLACULATOR_H
#define UNTITLED1_CLACULATOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "stack.h"

#define LINE_LEN 100
#define WIDE_LINE_LEN 300
#define INFIX 'i'
#define POSTFIX 'p'
#define INFIX_MSG "Infix:%s\n"
#define POST_FIX_MSG "Postfix: %s\n"
#define RESULT_MSG "The value is: %d\n"
#define CONVERSION_ERR_MSG "error: conversion of num to int failed"
/**
 * define enums
 */
enum Type
{
    LeftParenthesis,
    RightParenthesis,
    Plus,
    Minus,
    Mult,
    Div,
    Pow,
    Num
};

/**
 * define struct Element
 */
typedef struct Element
{
    void * _data;
    enum Type type;
} Element;

/**
 * read from the stdin the expression and then make on him the actions(convert and calculate)
 */
void getExpression();

/**
 * convert the expression in stdin into Infix
 * @param line
 * @param arr
 * @param len
 */
void toInfix(char *line, Element **arr, int *len);

/**
 * get char and return the object type
 * @param c
 * @return
 */

int getCharTypeVal(char c);

/**
 * convert the infix into postfix
 * @param infix
 * @param infixLen
 * @param postfix
 * @param postfixLen
 */

void infixToPostfix(Element *infix[], const int *infixLen, Element *postfix[], int *postfixLen);

/**
 * calculate the exp in postfix and return the result
 * @param p
 * @param pLen
 * @return
 */

int calculatePostfixExp(Element *p[], int pLen);
/**
 * check if c is /n(isNewLine)
 * @param c
 * @return
 */

int isNewLine(char c);

/**
 * check if c  is operator
 * @param c
 * @return
 */

int isOperator(char c);
/**
 * check if c  is Left Parenthesis
 * @param c
 * @return
 */
int isLeftParenthesis(char c);

/**
 * check if c  is Right Parenthesis
 * @param c
 * @return
 */

int isRightParenthesis(char c);
/**
 * check if type is operator
 * @param type
 * @return
 */

int isOperatorElement(int type);
/**
 * check which operator is higher
 * @param a
 * @param b
 * @return
 */

int isAHigherOrderThanB(enum Type a, enum Type b);
/**
 * return the value of the operator
 * @param operator
 * @return
 */

int value(enum Type operator);
/**
 * return the res of action that the operator do on 2 numbers.
 * @param a
 * @param b
 * @param t
 * @return
 */

int evaluate(int a, int b, enum Type t);
/**
 * convert the arr of elements into string
 * @param arr
 * @param len
 * @param type
 */

void print(Element **arr, int len, char type);
/**
 * eturn the char that presents t
 * @param t
 * @return
 */
char getTypeValChar(enum Type t);
/**
 * if the type is number its convert into string and add it to the message
 * @param exp
 * @param index
 * @param num
 */
void getNumChars(char exp[], int *index, const int *num);
/**
 * free memory
 * @param e
 */
void freeElement(Element *e);
/**
 * free memory
 * @param arr
 * @param len
 */
void freeExp(Element *arr[], int len);
/**
 * check memory allocation
 * @param e
 */

void checkElemAlloc(Element *e);
/**
 * check memory allocation
 * @param e
 */
void checkIntAlloc(int *e);
/**
 * check memory allocation
 * @param num
 */
void checkConversion(int num);


#endif //UNTITLED1_CLACULATOR_H
