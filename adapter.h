#ifndef ADAPTER_H
#define ADAPTER_H
#include "token.h"

#define MAXVARS 100

struct Stack * createStack();
void push(struct Stack* stack, struct token * newToken);
void pop(struct Stack* stack);
struct Stack{
	struct token ** array;
	int capacity;
	int top; //index
};
int isFull(struct Stack* stack);
int isEmpty(struct Stack* stack);
int find(struct Stack* stack,struct token * newToken);

#endif
