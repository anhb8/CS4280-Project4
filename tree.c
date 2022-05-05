#include "tree.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

extern FILE * ptrFile;


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
int labelC=0;
int varC=0;
char Name[20];
void newName(int type, char* name){
	if(type == VAR)
		sprintf(Name,"T%d",varC++); //generate a variable
	else
		sprintf(Name,"L%d",labelC++); //generate a label
	strcpy(name,Name);
} 


int isGenerateCode(struct node* ptr){
	if ((strcmp(ptr->fName,"program")==0) || (strcmp(ptr->fName,"vars")==0)
	 || (strcmp(ptr->fName,"block")==0) || (strcmp(ptr->fName,"stats")==0)
         || (strcmp(ptr->fName,"mStat")==0) || (strcmp(ptr->fName,"R0")==0)  
	 || (strcmp(ptr->fName,"N")==0) || (strcmp(ptr->fName,"stat")==0) 
	 || (strcmp(ptr->fName,"expr")==0) ||(strcmp(ptr->fName,"loop")==0) ) {
		return 0;
	}

	return 1;

}
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
			fprintf(ptrFile,"PUSH\n");
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

		if(isGenerateCode(ptr) == 1){

			recGen(ptr,stack);
		}
	
		else {
			preorder(ptr->firstN, stack);
        		preorder(ptr->secondN, stack);
        		preorder(ptr->thirdN, stack);
        		preorder(ptr->fourthN, stack);
		}


		if(strcmp(ptr->fName,"block")==0) {
			while(varCount[indexVar]>0){
				pop(stack);
				fprintf(ptrFile,"POP\n");
				varCount[indexVar]--;
			}
			indexVar--;	
		}
		
		if(strcmp(ptr->fName,"program") == 0){
			fprintf(ptrFile,"STOP\n");
			int i;
			for(i=0;i<varC;i++) {
				fprintf(ptrFile,"T%d 0\n",i);

			}

		}	
	}

}

void recGen(struct node *ptr,struct Stack* stack) {
	char argR[20], label[20];
	if(strcmp(ptr->fName,"expr1") == 0 && ptr->firstN != NULL){
		char argR1[20];
		newName(VAR,argR);
		fprintf(ptrFile,"STORE %s\n",argR);
		preorder(ptr->firstN,stack);
		newName(VAR,argR1);
		fprintf(ptrFile,"STORE %s\n",argR1);
		fprintf(ptrFile,"LOAD %s\n",argR);
		fprintf(ptrFile,"SUB %s\n",argR1);			

		preorder(ptr->secondN,stack);	
	} else if (strcmp(ptr->fName,"N1") == 0) {
		if (ptr->firstN!=NULL) {
			char argR1[20];
			newName(VAR,argR);
                	fprintf(ptrFile,"STORE %s\n",argR);
			preorder(ptr->firstN,stack);
			newName(VAR,argR1);
			fprintf(ptrFile,"STORE %s\n",argR1);
			fprintf(ptrFile,"LOAD %s\n",argR);
			if(strcmp(ptr->arrToken[0]->tokenIns,"/")==0) {
				fprintf(ptrFile,"DIV %s\n",argR1);
			} else {
				fprintf(ptrFile,"ADD %s\n",argR1);
			}

			preorder(ptr->secondN,stack); 
			
		}
		
	} else if (strcmp(ptr->fName,"A") == 0) {
		preorder(ptr->firstN,stack);
		if(ptr->secondN != NULL) {
                	newName(VAR,argR);
			fprintf(ptrFile,"STORE %s\n",argR);
			preorder(ptr->secondN,stack);
			fprintf(ptrFile,"MULT %s\n",argR);
		} 	
	} else if (strcmp(ptr->fName,"M") == 0) {
		preorder(ptr->firstN,stack);

		if(strcmp(ptr->firstN->fName,"M")==0) {
			fprintf(ptrFile,"MULT -1\n");
		}

	} else if (strcmp(ptr->fName,"R") == 0) { 
		if(ptr->arrToken[0]->tokenID == NUMBER){
			fprintf(ptrFile,"LOAD %s\n",ptr->arrToken[0]->tokenIns);
		} else if (ptr->arrToken[0]->tokenID == IDENT) {
			int index = find(stack,ptr->arrToken[0]);

			fprintf(ptrFile,"STACKR %d\n",index);

		}else{
			preorder(ptr->firstN, stack);
		}
	} else if (strcmp(ptr->fName,"out") == 0) {
		preorder(ptr->firstN,stack);
		newName(VAR,argR);
		fprintf(ptrFile,"STORE %s\n",argR);
		fprintf(ptrFile,"WRITE %s\n",argR);
	} else if (strcmp(ptr->fName,"in") == 0) {
		newName(VAR,argR);
		fprintf(ptrFile,"READ %s\n",argR);
		int index = find(stack,ptr->arrToken[1]);
		fprintf(ptrFile,"LOAD %s\n",argR);
		fprintf(ptrFile,"STACKW %d\n",index);	
	}else if(strcmp(ptr->fName,"assign") == 0) {
		preorder(ptr->firstN,stack);
		int index = find(stack,ptr->arrToken[1]);
		fprintf(ptrFile,"STACKW %d\n",index);
	
	} else if (strcmp(ptr->fName,"if") == 0) {
		char argR1[20], label1[20], label2[20], label3[20];
		newName(VAR,argR);
		newName(VAR,argR1);
		newName(LABEL,label);
		newName(LABEL, label3);
		preorder(ptr->firstN,stack);
		fprintf(ptrFile,"STORE %s\n",argR);
		preorder(ptr->thirdN,stack);
		fprintf(ptrFile,"STORE %s\n",argR1);
		
		if(strcmp(ptr->secondN->arrToken[0]->tokenIns,".") == 0){
			fprintf(ptrFile,"LOAD %s\n",argR);
			newName(LABEL, label1);
			fprintf(ptrFile,"BRZERO %s\n",label1);
			fprintf(ptrFile,"LOAD %s\n",argR1);
			newName(LABEL,label2);
			fprintf(ptrFile,"BRZERO %s\n",label2);
			fprintf(ptrFile,"MULT %s\n",argR);

			fprintf(ptrFile,"BRNEG %s\n",label3);
			fprintf(ptrFile,"BRPOS %s\n",label);

			fprintf(ptrFile,"%s: NOOP\n",label1);
			fprintf(ptrFile,"LOAD %s\n",argR1);
			fprintf(ptrFile,"BRZERO %s\n",label2);
			
			fprintf(ptrFile,"BRNEG %s\n",label3);
                        fprintf(ptrFile,"BRPOS %s\n",label);

			fprintf(ptrFile,"%s: NOOP\n",label2);
			fprintf(ptrFile,"LOAD %s\n",argR);
	
			fprintf(ptrFile,"BRNEG %s\n",label3);
                        fprintf(ptrFile,"BRZPOS %s\n",label);		

		}else{		
			fprintf(ptrFile,"LOAD %s\n",argR);
			fprintf(ptrFile,"SUB %s\n",argR1);
	
			if(strcmp(ptr->secondN->arrToken[0]->tokenIns,">=")==0) {
				fprintf(ptrFile,"BRNEG %s\n",label);
			} else if (strcmp(ptr->secondN->arrToken[0]->tokenIns,"<=")==0) {
				fprintf(ptrFile,"BRPOS %s\n",label);
			} else if (strcmp(ptr->secondN->arrToken[0]->tokenIns,"==")==0) {
				fprintf(ptrFile,"BRPOS %s\n",label);
				fprintf(ptrFile,"BRNEG %s\n",label);
			} else if (strcmp(ptr->secondN->arrToken[0]->tokenIns,"!=") == 0){
				fprintf(ptrFile,"BRZERO %s\n",label);	
			}
		}
		fprintf(ptrFile,"%s: NOOP\n",label3);
		preorder(ptr->fourthN,stack);
		fprintf(ptrFile,"%s: NOOP\n",label);
	} else if (strcmp(ptr->fName,"loop1") == 0) {
		char argR1[20], label1[20], label2[20], label3[20],label4[20];
		newName(LABEL, label4);
                newName(VAR,argR);
                newName(VAR,argR1);
                newName(LABEL,label);
                newName(LABEL, label3);
		fprintf(ptrFile,"%s: NOOP\n",label4);
                preorder(ptr->firstN,stack);
                fprintf(ptrFile,"STORE %s\n",argR);
                preorder(ptr->thirdN,stack);
                fprintf(ptrFile,"STORE %s\n",argR1);

                if(strcmp(ptr->secondN->arrToken[0]->tokenIns,".") == 0){
                        fprintf(ptrFile,"LOAD %s\n",argR);
                        newName(LABEL, label1);
                        fprintf(ptrFile,"BRZERO %s\n",label1);
                        fprintf(ptrFile,"LOAD %s\n",argR1);
                        newName(LABEL,label2);
                        fprintf(ptrFile,"BRZERO %s\n",label2);
                        fprintf(ptrFile,"MULT %s\n",argR);

                        fprintf(ptrFile,"BRNEG %s\n",label3);
                        fprintf(ptrFile,"BRPOS %s\n",label);

                        fprintf(ptrFile,"%s: NOOP\n",label1);
                        fprintf(ptrFile,"LOAD %s\n",argR1);
                        fprintf(ptrFile,"BRZERO %s\n",label2);

                        fprintf(ptrFile,"BRNEG %s\n",label3);
                        fprintf(ptrFile,"BRPOS %s\n",label);

                        fprintf(ptrFile,"%s: NOOP\n",label2);
                        fprintf(ptrFile,"LOAD %s\n",argR);

                        fprintf(ptrFile,"BRNEG %s\n",label3);
                        fprintf(ptrFile,"BRZPOS %s\n",label);

                }else{
			fprintf(ptrFile,"LOAD %s\n",argR);
                        fprintf(ptrFile,"SUB %s\n",argR1);

                        if(strcmp(ptr->secondN->arrToken[0]->tokenIns,">=")==0) {
                                fprintf(ptrFile,"BRNEG %s\n",label);
                        } else if (strcmp(ptr->secondN->arrToken[0]->tokenIns,"<=")==0) {
                                fprintf(ptrFile,"BRPOS %s\n",label);
                        } else if (strcmp(ptr->secondN->arrToken[0]->tokenIns,"==")==0) {
                                fprintf(ptrFile,"BRPOS %s\n",label);
                                fprintf(ptrFile,"BRNEG %s\n",label);
                        } else if (strcmp(ptr->secondN->arrToken[0]->tokenIns,"!=") == 0){
                                fprintf(ptrFile,"BRZERO %s\n",label);
                        }
                }
                fprintf(ptrFile,"%s: NOOP\n",label3);
                preorder(ptr->fourthN,stack);
		fprintf(ptrFile,"BR %s\n",label4);
                fprintf(ptrFile,"%s: NOOP\n",label);	
	} else if (strcmp(ptr->fName,"loop2") == 0) {
		char argR1[20], label1[20], label2[20], label3[20], label4[20];
                newName(LABEL, label4);
		fprintf(ptrFile,"%s: NOOP\n",label4);
		preorder(ptr->firstN,stack);

		newName(VAR,argR);
                newName(VAR,argR1);
                newName(LABEL,label);
                newName(LABEL, label3);
                preorder(ptr->secondN,stack);
                fprintf(ptrFile,"STORE %s\n",argR);
                preorder(ptr->fourthN,stack);
                fprintf(ptrFile,"STORE %s\n",argR1);

                if(strcmp(ptr->thirdN->arrToken[0]->tokenIns,".") == 0){
                        fprintf(ptrFile,"LOAD %s\n",argR);
                        newName(LABEL, label1);
                        fprintf(ptrFile,"BRZERO %s\n",label1);
                        fprintf(ptrFile,"LOAD %s\n",argR1);
                        newName(LABEL,label2);
                        fprintf(ptrFile,"BRZERO %s\n",label2);
                        fprintf(ptrFile,"MULT %s\n",argR);

                        fprintf(ptrFile,"BRNEG %s\n",label3);
                        fprintf(ptrFile,"BRPOS %s\n",label);

                        fprintf(ptrFile,"%s: NOOP\n",label1);
                        fprintf(ptrFile,"LOAD %s\n",argR1);
                        fprintf(ptrFile,"BRZERO %s\n",label2);

                        fprintf(ptrFile,"BRNEG %s\n",label3);
                        fprintf(ptrFile,"BRPOS %s\n",label);

                        fprintf(ptrFile,"%s: NOOP\n",label2);
                        fprintf(ptrFile,"LOAD %s\n",argR);

                        fprintf(ptrFile,"BRNEG %s\n",label3);
                        fprintf(ptrFile,"BRZPOS %s\n",label);

                }else{
                        fprintf(ptrFile,"LOAD %s\n",argR);
                        fprintf(ptrFile,"SUB %s\n",argR1);

                        if(strcmp(ptr->thirdN->arrToken[0]->tokenIns,">=")==0) {
                                fprintf(ptrFile,"BRNEG %s\n",label);
                        } else if (strcmp(ptr->thirdN->arrToken[0]->tokenIns,"<=")==0) {
                                fprintf(ptrFile,"BRPOS %s\n",label);
                        } else if (strcmp(ptr->thirdN->arrToken[0]->tokenIns,"==")==0) {
                                fprintf(ptrFile,"BRPOS %s\n",label);
                                fprintf(ptrFile,"BRNEG %s\n",label);
                        } else if (strcmp(ptr->thirdN->arrToken[0]->tokenIns,"!=") == 0){
                                fprintf(ptrFile,"BRZERO %s\n",label);
                        }
                }
                fprintf(ptrFile,"%s: NOOP\n",label3);
                fprintf(ptrFile,"BR %s\n",label4);
		fprintf(ptrFile,"%s: NOOP\n",label);
	} else if (strcmp(ptr->fName,"label") == 0) {
		fprintf(ptrFile,"%s: NOOP\n",ptr->arrToken[1]->tokenIns);
	} else if (strcmp(ptr->fName,"goto") == 0) {
		fprintf(ptrFile,"BR %s\n",ptr->arrToken[1]->tokenIns);
	}

 
}












