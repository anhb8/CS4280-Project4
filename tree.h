#ifndef TREE_H
#define TREE_H
#include "node.h"
#include "adapter.h"

struct node *createNode();
void preorder(struct node* ptr,struct Stack* stack);

#endif
