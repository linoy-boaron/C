#include "stack.h"

/**
 * allocate memory to the stack
 * @param elementSize
 * @return
 */
Stack *stackAlloc(size_t elementSize)
{
    Stack *stack = (Stack *) malloc(sizeof(Stack));
    stack->_top = NULL;
    stack->_elementSize = elementSize;
    return stack;
}

/**
 * push element to the stack
 * @param stack
 * @param data
 */
void push(Stack *stack, void *data)
{
    Node *node = (Node *) malloc(sizeof(Node));
    if (node == NULL)
    {
        fprintf(stderr, MEMORY_ERR_MSG);
        exit(EXIT_FAILURE);
    }

    node->_data = malloc(stack->_elementSize);
    memcpy(node->_data, data, stack->_elementSize);
    node->_next = stack->_top;
    stack->_top = node;
}

/**
 * do peek
 * @param stack
 * @return
 */
void *peek(Stack *stack)
{
    if (stack->_top != NULL)
    {
        return stack->_top->_data;
    }
    return NULL;
}
/**
 * pop element from the stack
 * @param stack
 * @param headData
 */
void pop(Stack *stack, void *headData)
{
    if (stack == NULL)
    {
        fprintf(stderr, NO_STACK);
        exit(EXIT_FAILURE);
    }
    if (stack->_top == NULL)
    {
        return;
    }
    Node *node = stack->_top;
    memcpy(headData, node->_data, stack->_elementSize);
    stack->_top = node->_next;
    free(node->_data);
    free(node);
}

/**
 * check if the Stack is empty
 * @param stack
 * @return
 */
int isEmpty(Stack *stack)
{
    return stack->_top == NULL;
}
/**
 * free the memory that the stake take
 * @param stack
 */
void freeStack(Stack **stack)
{
    Node *p1;
    Node *p2;
    if (*stack != NULL)
    {
        p1 = (*stack)->_top;
        while (p1)
        {
            p2 = p1;
            p1 = p1->_next;
            free(p2->_data);
            free(p2);
        }
        free(*stack);
        *stack = NULL;
    }
}