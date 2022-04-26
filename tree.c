#include "tree.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

struct node * createNode(char * nodeN) {
	struct node * temp = (struct node *)malloc(sizeof(struct node));
	temp->firstN = temp->secondN = temp->thirdN = temp->fourthN = NULL;
	temp->fName = (char *)malloc(sizeof(nodeN) + 1);
	strcpy(temp->fName,nodeN);
	
	temp->arrToken = (struct token **) malloc(5*sizeof(struct token*));
	temp->arrSize = 0;
	return temp;
}
int varCount[100] = {0};
int indexVar = 0;

void preorder(struct node* ptr,struct Stack* stack){
	if(ptr!=NULL){
		if(strcmp(ptr->fName,"block")==0) {
			indexVar++;
		}

		else if (strcmp(ptr->fName,"vars")==0 && ptr->arrSize>0) {
			if (find(stack,ptr->arrToken[1])!=-1) {
				printf("Error: Token %s already exists (Line %d)\n",ptr->arrToken[1]->tokenIns,ptr->arrToken[1]->line);
				exit(1);
			}
			push(stack,ptr->arrToken[1]);
			varCount[indexVar]++;
		}

		else {
			int i;
			for (i=0;i<ptr->arrSize;i++) {
				if (ptr->arrToken[i]->tokenID==IDENT) {
					if(find(stack,ptr->arrToken[i])==-1) {
						printf("Error: Undefined variables %s (Line %d)\n",ptr->arrToken[i]->tokenIns,ptr->arrToken[i]->line);
						exit(1);
					}
				}
			}
		}

		preorder(ptr->firstN, stack);
        	preorder(ptr->secondN, stack);
        	preorder(ptr->thirdN, stack);
        	preorder(ptr->fourthN, stack);
	
		if(strcmp(ptr->fName,"block")==0) {
			while(varCount[indexVar]>0){
				pop(stack);
				varCount[indexVar]--;
			}
			indexVar--;	
		}	
	}

}

