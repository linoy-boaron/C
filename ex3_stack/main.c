/**
 * @file main.c
 * @author Linoy Boaron <linoy.boaron@mail.huji.ac.il>
 * @version 1.0
 * @date 05/12/18
 *
 * @brief get expression and print the infix convertion and the Postfix convertion of the exp and print the res of
 * the mathematical exp .
 *
 * @section DESCRIPTION
 * Input  : some math expression
 * Process:onvert it into infix and then to Postfix and return the res of the exp
 * Output :print the infix convertion and the Postfix convertion of the exp and then print the mathematical exp to .
 */


#include "main.h"


/**
 *
 * the main function . receive  the expression and convert it into infix and then to Postfix and return the res.
 */
int main()
{
    getExpression();
    return EXIT_SUCCESS;
}
/**
 * read from the stdin the expression and then make on him the actions(convert and calculate)
 */
void getExpression()
{
    char line[LINE_LEN] = {};
    while (fgets(line, LINE_LEN, stdin) != NULL)
    {
        int infixLen = LINE_LEN, postfixLen = LINE_LEN;
        Element *infix[LINE_LEN] = {}, *postfix[LINE_LEN] = {}; // array of pointers to elements
        toInfix(line, infix, &infixLen);
        print(infix, infixLen, INFIX);
        infixToPostfix(infix, &infixLen, postfix, &postfixLen);
        print(postfix, postfixLen, POSTFIX);
        int result = calculatePostfixExp(postfix, postfixLen);
        printf(RESULT_MSG, result);
        freeExp(infix, infixLen);
        freeExp(postfix, postfixLen);
    }
}
/**
 * convert the expression in stdin into Infix
 * @param line
 * @param arr
 * @param len
 */
void toInfix(char *line, Element **arr, int *len)
{
    int j = 0;
    for (int i = 0; i < LINE_LEN; i++)
    {
        char c = line[i];
        if (isNewLine(c))
        {
            break;
        }
        if (c == ' ')
        {
            continue;
        }
        arr[j] = malloc(sizeof(Element));
        checkElemAlloc(arr[j]);
        if (isOperator(c) || isLeftParenthesis(c) || isRightParenthesis(c))
        {
            arr[j]->_data = NULL;
            arr[j]->type = getCharTypeVal(c);
        }

        else
        {
            char operand[LINE_LEN] = {};
            int curChar = i;
            while (!(isOperator(c) || isLeftParenthesis(c) || isRightParenthesis(c) || isNewLine(c)))//process num
            {
                operand[i - curChar] = line[i];
                i++;
                c = line[i];
            }
            i--;
            errno = 0;
            int *num = malloc(sizeof(int));
            checkIntAlloc(num);
            *num = strtol(operand, NULL, 10);
            checkConversion(*num);
            arr[j]->_data = (void *) num;
            arr[j]->type = Num;
        }
        j++;
    }
    *len = j;
}
/**
 * check if c is /n(isNewLine)
 * @param c
 * @return
 */
int isNewLine(char c)
{
    return c == '\n';
}

/**
 * check if c  is operator
 * @param c
 * @return
 */
int isOperator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}
/**
 * check if c  is Left Parenthesis
 * @param c
 * @return
 */


int isLeftParenthesis(char c)
{
    return c == '(';
}
/**
 * check if c  is Right Parenthesis
 * @param c
 * @return
 */
int isRightParenthesis(char c)
{
    return c == ')';
}
/**
 * get char and return the object type
 * @param c
 * @return
 */
int getCharTypeVal(char c)
{
    if (c == '(')
    {
        return LeftParenthesis;
    }
    else if (c == ')')
    {
        return RightParenthesis;
    }
    else if (c == '+')
    {
        return Plus;
    }
    else if (c == '-')
    {
        return Minus;
    }
    else if (c == '*')
    {
        return Mult;
    }
    else if (c == '/')
    {
        return Div;
    }

    return Pow;
}
/**
 * convert the infix into postfix
 * @param infix
 * @param infixLen
 * @param postfix
 * @param postfixLen
 */
void infixToPostfix(Element *infix[], const int *infixLen, Element *postfix[], int *postfixLen)
{
    Stack *stack = stackAlloc(sizeof(Element));
    int i = 0, j = 0;
    while (i < *infixLen)
    {
        Element *cur = infix[i];
        if (cur->type == Num) //If an operand is found Add it to P
        {
            postfix[j] = cur;
            j++;
            infix[i] = NULL;

        }
        if (cur->type == LeftParenthesis)//If a left parenthesis is found Push it to the stack
        {
            push(stack, infix[i]);
            freeElement(infix[i]);
            infix[i] = NULL;
        }
        if (cur->type == RightParenthesis)
        {
            Element *stackTopVal = (Element *) peek(stack);
            while (!isEmpty(stack))
            {
                if (stackTopVal->type == LeftParenthesis)
                {
                    break;
                }
                postfix[j] = (Element *) malloc(sizeof(Element));
                checkElemAlloc(postfix[j]);
                pop(stack, postfix[j]);
                j++;
                stackTopVal = (Element *) peek(stack);
            }
            Element *useless = malloc(sizeof(Element));
            pop(stack, useless);
            freeElement(useless);
        }
        if (isOperatorElement(cur->type))//If an operator is found
        {
            Element *stackTopVal = (Element *) peek(stack);
            if (isEmpty(stack) || stackTopVal->type == LeftParenthesis)
            {
                push(stack, cur);
                freeElement(infix[i]);
                infix[i] = NULL;
            }
            else
            {
                while (!isEmpty(stack) && stackTopVal->type != LeftParenthesis &&
                       !isAHigherOrderThanB(cur->type, stackTopVal->type))
                {
                    postfix[j] = (Element *) malloc(sizeof(Element));
                    checkElemAlloc(postfix[j]);
                    pop(stack, postfix[j]);
                    j++;
                    stackTopVal = (Element *) peek(stack);
                }
                push(stack, cur);
            }
        }
        i++;
    }
    while (!isEmpty(stack))
    {
        postfix[j] = (Element *) malloc(sizeof(Element));
        checkElemAlloc(postfix[j]);
        pop(stack, postfix[j]);
        j++;
    }
    *postfixLen = j;
    freeStack(&stack);
}

/**
 * check if type is operator
 * @param type
 * @return
 */
int isOperatorElement(int type)
{
    return type == Plus || type == Minus || type == Mult || type == Div || type == Pow;
}
/**
 * check which operator is higher
 * @param a
 * @param b
 * @return
 */
int isAHigherOrderThanB(enum Type a, enum Type b)
{
    return value(a) > value(b);
}

/**
 * return the value of the operator
 * @param operator
 * @return
 */
int value(enum Type operator)
{
    if (operator == Plus || operator == Minus)
    {
        return 1;
    }
    else if (operator == Mult || operator == Div)
    {
        return 2;
    }
    else
    {
        return 3;
    }

}
/**
 * calculate the exp in postfix and return the result
 * @param p
 * @param pLen
 * @return
 */
int calculatePostfixExp(Element *p[], int pLen)
{
    Stack *stack = stackAlloc(sizeof(int));
    int i = 0;
    while (i < pLen)
    {
        if (p[i]->type == Num)
        {
            push(stack, p[i]->_data);
        }
        if (isOperatorElement(p[i]->type))
        {
            int a, b;
            pop(stack, &a);
            pop(stack, &b);
            int res = evaluate(a, b, p[i]->type);
            push(stack, &res);
        }
        i++;
    }
    int result;
    pop(stack, &result);
    return result;
}
/**
 * return the res of action that the operator do on 2 numbers.
 * @param a
 * @param b
 * @param t
 * @return
 */
int evaluate(int a, int b, enum Type t)
{
    switch (t)
    {
        case Plus:
            return a + b;
        case Minus:
            return b-a;
        case Mult:
            return a * b;
        case Div:
            return b / a;
        default:
            return (int) pow(a, b);
    }
}
/**
 * convert the arr of elements into string
 * @param arr
 * @param len
 * @param type
 */
void print(Element **arr, int len, char type)
{
    char exp[WIDE_LINE_LEN] = {};
    int j = 0;
    for (int i = 0; i < len; i++)
    {
        if (arr[i]->type == Num)
        {
            getNumChars(exp, &j, (int *) arr[i]->_data);
        }
        else
        {
            exp[j] = ' ';
            exp[j + 1] = getTypeValChar(arr[i]->type);
            j += 2;
        }
    }
    if (type == INFIX)
    {
        printf(INFIX_MSG, exp);
    }
    else
    {
        printf(POST_FIX_MSG, exp);
    }
}
/**
 * if the type is number its convert into string and add it to the message
 * @param exp
 * @param index
 * @param num
 */
void getNumChars(char exp[], int *index, const int *num)
{
    int i = *index;
    exp[i] = ' ';
    i++;
    char numChars[LINE_LEN] = {};
    sprintf(numChars, "%d", *num);
    int j = 0;
    while (numChars[j] != '\0')
    {
        exp[i] = numChars[j];
        j++;
        i++;
    }
    *index = i;

}
/**
 * return the char that presents t ;
 * @param t
 * @return
 */
char getTypeValChar(enum Type t)
{
    if (t == LeftParenthesis)
    {
        return '(';
    }
    else if (t == RightParenthesis)
    {
        return ')';
    }
    else if (t == Plus)
    {
        return '+';
    }
    else if (t == Minus)
    {
        return '-';
    }
    else if (t == Mult)
    {
        return '*';
    }
    else if (t == Div)
    {
        return '/';
    }

    return '^';
}
/**
 * free memory
 * @param e
 */
void freeElement(Element *e)
{
    if (e != NULL)
    {
        free(e->_data);
        free(e);
    }
}
/**
 * free memory
 * @param arr
 * @param len
 */
void freeExp(Element *arr[], int len)
{
    for (int i = 0; i < len; i++)
    {
        freeElement(arr[i]);
    }

}
/**
 * check memory allocation
 * @param e elment
 */
void checkElemAlloc(Element *e)
{
    if (e == NULL)
    {
        fprintf(stderr, MEMORY_ERR_MSG);
        exit(EXIT_FAILURE);
    }
}

/**
 * check memory allocation
 * @param e
 */
void checkIntAlloc(int *e)
{
    if (e == NULL)
    {
        fprintf(stderr, MEMORY_ERR_MSG);
        exit(EXIT_FAILURE);
    }
}
/**
 * check memory allocation
 * @param num
 */
void checkConversion(int num)
{
    if (num == 0 && errno != 0)
    {
        fprintf(stderr, CONVERSION_ERR_MSG);
        exit(EXIT_FAILURE);
    }
}