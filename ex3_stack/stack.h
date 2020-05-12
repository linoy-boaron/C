
#ifndef UNTITLED1_STACK_H
#define UNTITLED1_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_ERR_MSG "error: memory allocation failed"
#define NO_STACK "error: stack operation error due to NULL stack"

/**
 * define struct NODE
 */
typedef struct Node
{
    void *_data;
    struct Node *_next;
} Node;
/**
 * define struct Stack
 */
typedef struct Stack
{
    Node *_top;
    size_t _elementSize;
} Stack;

/**
 * allocate memory to the stack
 * @param elementSize
 * @return
 */
Stack *stackAlloc(size_t elementSize);
/**
 * push element to the stack
 * @param stack
 * @param data
 */
void push(Stack *stack, void *data);
/**
 * do peek
 * @param stack
 * @return
 */
void *peek(Stack *stack);
/**
 * pop element from the stack
 * @param stack
 * @param headData
 */
void pop(Stack *stack, void *headData);
/**
 *  check if the Stack is empty
 * @param stack
 * @return
 */

int isEmpty(Stack *stack);
/**
 * free the memory that the stake take
 * @param stack
 */

void freeStack(Stack **stack);


#endif //UNTITLED1_STACK_H
