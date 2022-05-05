#ifndef TREE_H
#define TREE_H
#include "node.h"
#include "adapter.h"

struct node *createNode();
void preorder(struct node* ptr,struct Stack* stack);
void recGen(struct node* ptr, struct Stack* stack);
void newName(int type, char* name);
enum {VAR = 101, LABEL};

#endif
