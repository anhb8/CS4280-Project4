#include "adapter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Stack * createStack(int capacity){
	struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    	stack->capacity = capacity;
    	stack->top = -1;
    	stack->array = (struct token **)malloc(stack->capacity * sizeof(struct token *));
    	return stack;
}

int isFull(struct Stack* stack)
{
    return stack->top == stack->capacity - 1;
}

int isEmpty(struct Stack* stack)
{
    return stack->top == -1;
}

void push(struct Stack* stack, struct token * newToken) {
	if (isFull(stack)) {
		printf("Error: ");
		exit(1);
	}
	
	stack->array[++stack->top] = newToken;
}

void pop(struct Stack* stack) {
	if (isEmpty(stack)) {
		printf("Error: ");
		exit(1);
	}	
	
	 stack->array[stack->top--]=NULL;	
	

}

int find(struct Stack* stack,struct token * targetToken) {
	int i;
	for (i=stack->top;i>=0;i--){
		if(stack->array[i]->tokenID == targetToken->tokenID && 
			strcmp(stack->array[i]->tokenIns,targetToken->tokenIns)==0) {
			return stack->top-i;
		}			
	}
	return -1;
}



