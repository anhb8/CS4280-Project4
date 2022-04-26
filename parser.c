#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "tree.h"
#include "token.h"
#include "parser.h"
#include "scanner.h"

char nextChar;
struct token * tok;
FILE * ptr = NULL;
int isConsumed = 1;  
int isfileEmpty(FILE * ptr, char* c)
{
        if(ptr != NULL)
        {
                fseek(ptr, 0, SEEK_END);//set the pointer to the end of the file
                int test = ftell(ptr);

                if(test == 0)
                {
                        fprintf(stderr, "ERROR: %s: Input is empty\n", c);
                        return 1;
                }


        }
        else
        {
                fprintf(stderr, "ERROR: %s: Cannot open file\n", c);
                return 1;
        }

        fseek(ptr, 0, SEEK_SET); //reset the pointer back to the beginning of the file

        return 0;
}
struct node *  parser(char* fileName)
{
	ptr = fopen(fileName, "r");
	if(isfileEmpty(ptr,"parser") == 1)
		return NULL;
	nextChar = fgetc(ptr);
	filter();

	struct node * root = program();
	//Start here
		
	fclose(ptr);
	return root;
}

void printpError(int errorCode){
	switch(errorCode) {
	case 100:
		printf("Error: Expect 'main',but received %s (Line: %d, Character: %d)\n",
		tok->tokenIns,tok->line,tok->charN);
		break;
	case 101:
		printf("Error: The token is not 'Identifier',but received %s (Line: %d, Character: %d)\n",
                  tok->tokenIns,tok->line,tok->charN);
		break;
	case 102:
		printf("Error:The token is not ':=',but received %s (Line: %d, Character: %d)\n",
                tok->tokenIns,tok->line,tok->charN);
		break;
	case 103:
		printf("Error: The token is not 'whole',but received %s (Line: %d, Character: %d)\n",
                tok->tokenIns,tok->line,tok->charN);
		break;
	case 104:
		printf("Error:The token is not ';'but received %s (Line: %d, Character: %d)\n",
                tok->tokenIns,tok->line,tok->charN);
		break;
	case 105:   
                printf("Error:The token is not '{' but received %s (Line: %d, Character: %d)\n",
                tok->tokenIns,tok->line,tok->charN);
                break;
	case 106:   
                printf("Error:The token is not '}'but received %s (Line: %d, Character: %d)\n",
                tok->tokenIns,tok->line,tok->charN);
                break;
	case 107:   
                printf("Error:The token is not ';',but received %s (Line: %d, Character: %d)\n",
                tok->tokenIns,tok->line,tok->charN);
                break;
	case 108:   
                printf("Error:The token is not 'listen',but received %s (Line: %d, Character: %d)\n",tok->tokenIns,tok->line,tok->charN);
                break;
	case 110:
                printf("Error:The token is not 'yell',but received %s (Line: %d, Character: %d)\n",tok->tokenIns,tok->line,tok->charN);
                break;
	case 111:
                printf("Error:The token is not 'if',but received %s (Line: %d, Character: %d)\n",tok->tokenIns,tok->line,tok->charN);
                break;
	case 112:
                printf("Error:The token is not '[',but received %s (Line: %d, Character: %d)\n",tok->tokenIns,tok->line,tok->charN);
                break;
	case 113:
                printf("Error:The token is not ']',but received %s (Line: %d, Character: %d)\n",tok->tokenIns,tok->line,tok->charN);
                break;
	case 114:
                printf("Error:The token is not 'then',but received %s (Line: %d, Character: %d)\n",tok->tokenIns,tok->line,tok->charN);
                break;
	case 115:
                printf("Error:The token is not 'until',but received %s (Line: %d, Character: %d)\n",tok->tokenIns,tok->line,tok->charN);
                break;
	case 116:
                printf("Error:The token is not 'assign',but received %s (Line: %d, Character: %d)\n",tok->tokenIns,tok->line,tok->charN);
                break;
	case 118:
                printf("Error:The token is not '=',but received %s (Line: %d, Character: %d)\n",tok->tokenIns,tok->line,tok->charN);
                break;
	case 121:
                printf("Error:The token is not 'label',but received %s (Line: %d, Character: %d)\n",tok->tokenIns,tok->line,tok->charN);
                break;
	case 122:
                printf("Error:The token is not 'portal',but received %s (Line: %d, Character: %d)\n",tok->tokenIns,tok->line,tok->charN);
                break;
	case 123:
		printf("Error:The token is not ')',but received %s (Line: %d, Character: %d)\n",tok->tokenIns,tok->line,tok->charN);
                break;	
	case 124:
		printf("Error:The token is not 'Identifier' or 'Integer',but received %s (Line: %d, Character: %d)\n",tok->tokenIns,tok->line,tok->charN);
                break;
        }  
	printf("Error code: %d\n",errorCode);
	exit(1);
}

void filter(){
	if(isConsumed == 1){
		tok = FSADriver(ptr);
		while(tok->tokenID == COMMENT)
			tok = FSADriver(ptr);
	}else{
		isConsumed = 1;
	}
}

void attachToken(struct node** tempN) {
	struct token * tempT  = (struct token * ) malloc (sizeof(tok));
	tempT->tokenIns=(char * ) malloc (sizeof(tok->tokenIns));
	tempT->tokenID = tok->tokenID;
	tempT->line= tok->line;
	tempT->charN=tok->charN;
	strcpy(tempT->tokenIns,tok->tokenIns);
	int i;
	for (i=0;i<5;i++) {
		if ((*tempN)->arrToken[i] == NULL) {
			(*tempN)->arrToken[i] = tempT;
			break;
		}
	}
	(*tempN)->arrSize++;	
}
struct node * vars() {
	struct node *temp = createNode("vars");

	if (strcmp(tok->tokenIns,"declare")==0) {
		attachToken(&temp);

		filter();
		if (tok->tokenID != IDENT) {
			printpError(101);	
		}
		attachToken(&temp);

		filter();		
		if(strcmp(tok->tokenIns,":=")!=0) {
			printpError(102);
		}
		attachToken(&temp);

		filter();
		if (strcmp(tok->tokenIns,"whole")!=0) {
			printpError(103);
		}
		attachToken(&temp);

		filter();
		if (strcmp(tok->tokenIns,";")!=0) {
                        printpError(104);
                }
		attachToken(&temp);
	
		filter();
		temp->firstN = vars();   	
	}else 
		isConsumed = 0;

	return temp;
}

struct node * program() {
	struct node * temp=createNode("program");
	temp->firstN=vars();
	filter();
	if (strcmp(tok->tokenIns,"main")!=0) {
		printpError(100);
	}
	attachToken(&temp);
	filter();

	temp->secondN=block();
	return temp;
}

struct node *  block(){	
	struct node * temp=createNode("block");
	if(strcmp(tok->tokenIns,"{")!=0) {
		printpError(105);
	}
	
	else {
		attachToken(&temp);
		filter();
		temp->firstN=vars();
		filter();
		temp->secondN=stats();
		filter();
		if(strcmp(tok->tokenIns,"}")!=0) {
			printpError(106);
		}
		attachToken(&temp);
	}
	return temp;
}

struct node * stats(){
	struct node * temp=createNode("stats");
	temp->firstN=stat();
	filter();
	temp->secondN=mStat();
	return temp;
}

struct node *  mStat(){
	struct node * temp=createNode("mStat");
	if(tok->tokenID == KEYWORD || strcmp(tok->tokenIns, "{") == 0) {
	
		temp->firstN=stat();
		filter();
		temp->secondN=mStat();
	}
	 
	else 
		isConsumed=0;
	return temp;
}
	
	
struct node * stat(){
	struct node * temp=createNode("stat");
	int isBlock = 0;
	if(tok->tokenID == KEYWORD || strcmp(tok->tokenIns, "{") == 0){
		if(strcmp(tok->tokenIns,"listen")==0) {
			temp->firstN=in();						
		}

		else if (strcmp(tok->tokenIns,"yell")==0) {
			temp->firstN=out();
		}

		else if (strcmp(tok->tokenIns,"{")==0) {
			temp->firstN=block();
			isBlock=1;
		}

		else if (strcmp(tok->tokenIns,"if")==0) {
                         temp->firstN=if1();
                 }

		else if (strcmp(tok->tokenIns,"repeat")==0) {
                        temp->firstN= loop();
                 }
		
		else if (strcmp(tok->tokenIns,"assign")==0) {
                  	temp->firstN=assign();
                 } 

		else if (strcmp(tok->tokenIns,"portal")==0) {
                        temp->firstN= goto1();
                 } 

		else if (strcmp(tok->tokenIns,"label")==0) {
                         temp->firstN=label();
                 } 
	}else
		printpError(121);//expect a statement
	
	if(isBlock != 1){
		filter();
		if(strcmp(tok->tokenIns,";") != 0)
			printpError(104);
	}
	return temp;	
}

struct node * loop(){
	struct node * temp=createNode("loop");
	if (strcmp(tok->tokenIns,"repeat")!=0) {
		printpError(122);
	}
	
	else{
		attachToken(&temp);	
		filter();
		if (strcmp(tok->tokenIns,"[")==0) {
			temp->firstN = loop1();
		}

		else 
			temp->firstN = loop2();
	}
	return temp;
}
struct node * in() {
	struct node * temp=createNode("in");
	if(strcmp(tok->tokenIns,"listen")!=0) {
                printpError(108);
        } 
	attachToken(&temp);	
	filter();

	if (tok->tokenID != IDENT) {
             printpError(101);    
        }  
	attachToken(&temp);
	return temp;
}

struct node * out() {
	struct node * temp=createNode("out");
	  if(strcmp(tok->tokenIns,"yell")!=0) {
                printpError(110);
          } 
	 attachToken(&temp);
	filter();
	temp->firstN=expr();
	return temp;
}

struct node * expr() {
	struct node * temp=createNode("expr");
	temp->firstN=N();
	filter();
	if(strcmp(tok->tokenIns,"-")!=0) {
                isConsumed=0;
        }
	else {
		attachToken(&temp);
		filter();
		temp->secondN=expr();
		
	} 
	return temp;
}
struct node * N(){
	struct node * temp=createNode("N");
	
	temp->firstN=A();
	filter();
	temp->secondN=N1();
	return temp;
}

struct node * N1() {
	struct node * temp=createNode("N1");
	if(strcmp(tok->tokenIns,"/")==0) {
		filter();
		temp->firstN=A();
		filter();
		temp->secondN=N1();	
	}

	else if(strcmp(tok->tokenIns,"+")==0) {
		attachToken(&temp);
		filter();
		temp->firstN=A();
		filter();
		temp->secondN=N1();
	}

	else
		isConsumed=0;
	return temp;	

}

struct node * A() {
	struct node * temp=createNode("A");
	temp->firstN=M();
	filter();
	if(strcmp(tok->tokenIns,"*")!=0) {
		isConsumed=0;
	}

	else {
		attachToken(&temp);
		filter();
		temp->secondN=A();
	
	}
	return temp;
}

struct node * M() {
	struct node * temp=createNode("M");
	if(strcmp(tok->tokenIns,"%")==0) {
		attachToken(&temp);
		filter();
		temp->firstN=M();
	}

	else 
		temp->firstN=R();
	return temp;
}

struct node * R() {
	struct node * temp=createNode("R");
	if(strcmp(tok->tokenIns,"(")==0) {
		attachToken(&temp);
		filter();
		temp->firstN=expr();
		filter();
		if(strcmp(tok->tokenIns,")")!=0)
			printpError(123);
		else
			attachToken(&temp);
	}
	
	else if (tok->tokenID == IDENT || tok->tokenID == NUMBER) {
		//printf("Token %s consumed\n", tok->tokenIns);
		attachToken(&temp);
	}

	else
		printpError(124); //expect identifier or number 
	return temp;
}
struct node * if1() {
	struct node * temp=createNode("if");
	if(strcmp(tok->tokenIns,"if")!=0) {
                printpError(111);
        } 
	
	else {
		attachToken(&temp);
		filter();
		 if(strcmp(tok->tokenIns,"[")!=0) {
                	printpError(112);
       		 }

		else {
			attachToken(&temp);
			filter();
			temp->firstN=expr();
			filter();
			temp->secondN=R0();
			filter();
			temp->thirdN=expr();
			filter();
			if(strcmp(tok->tokenIns,"]")!=0) {
                		printpError(113);
        		}	 
			
			else {
				attachToken(&temp);
				filter();
				 if(strcmp(tok->tokenIns,"then")!=0) {
               				 printpError(114);
        			 }

				else {
					attachToken(&temp);
					filter();
					
					temp->fourthN=stat();
				} 
			} 
	
		}	 		
	}

	return temp;
}

struct node * loop1() {
	struct node * temp=createNode("loop1");
	if(strcmp(tok->tokenIns,"[")!=0) {
                printpError(112);
        } 
		
	else {
			attachToken(&temp);
			filter();
			temp->firstN=expr();
			filter();
			temp->secondN=R0();
			filter();
			temp->thirdN=expr();
			filter();
			if(strcmp(tok->tokenIns,"]")!=0) {
                		printpError(113);
        		}

			else {
				 attachToken(&temp);
				filter();
				temp->fourthN=stat();
			} 			
	}
	return temp;
}

struct node * loop2() {
		struct node * temp=createNode("loop2");
		temp->firstN=stat();
		filter();
		if(strcmp(tok->tokenIns,"until")!=0) {
                	printpError(115);
        	}
		
		else {
			attachToken(&temp);
			filter();
			if(strcmp(tok->tokenIns,"[")!=0) {
                		printpError(112);
        		} 
			
			else {
				attachToken(&temp);
				filter();
				temp->secondN=expr();
				filter();
				temp->thirdN=R0();
				filter();
				temp->fourthN=expr();
				filter();
				if(strcmp(tok->tokenIns,"]")!=0) {
                			printpError(113);
        			} 
				else 
					attachToken(&temp);
			}
		} 
	return temp;
}

struct node * assign() {
	struct node * temp=createNode("assign");
	if(strcmp(tok->tokenIns,"assign")!=0) {
                printpError(116);
        }

	else {
		attachToken(&temp);
		filter();
		if (tok->tokenID != IDENT) {
                        printpError(101);    
                } 
		
		else {
			attachToken(&temp);
			filter();
			if(strcmp(tok->tokenIns,"=")!=0) {
               			printpError(118);
        		}

			else {
				attachToken(&temp);
				filter();
				temp->firstN=expr();				
			} 			

		} 
	} 
	return temp;
}

 struct node * R0() {
	struct node * temp=createNode("R0");
	if(strcmp(tok->tokenIns,">=")==0 || strcmp(tok->tokenIns,"<=")==0 || 
		strcmp(tok->tokenIns,"==")==0 || strcmp(tok->tokenIns,"!=")==0 ) {
             	attachToken(&temp);
             // printf("Consumed token %s\n",tok->tokenIns);
        }
	
	else if (strcmp(tok->tokenIns,".")==0 ) {
		attachToken(&temp);
		filter();
		if(strcmp(tok->tokenIns,".")==0) {
			attachToken(&temp);
			filter();
			if(strcmp(tok->tokenIns,".")==0) {
				attachToken(&temp);
			}
			
			else 
				printpError(119);
	
	
		}
		else
			printpError(119); 
	}

	else 
		printpError(120);


return temp;	  

}

struct node * label(){
	struct node * temp=createNode("label");
	if(strcmp(tok->tokenIns,"label")!=0) {
                printpError(121);
        }

	else {
		attachToken(&temp);
		filter();
		if (tok->tokenID != IDENT) {
                        printpError(101);    
                }
		else
			attachToken(&temp);  
	} 
	return temp;
}

struct node * goto1(){	
	struct node * temp=createNode("goto1");
	if(strcmp(tok->tokenIns,"portal")!=0) {
                printpError(122);
        } 

	else {
		attachToken(&temp);
		filter();
		if (tok->tokenID != IDENT) {
                        printpError(101);    
                } 
		else 
			attachToken(&temp);
	}
	return temp;
}

