#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "tree.h"
#include <string.h>
#include "parser.h"
#include "adapter.h"
#define BUFF 500


char progFile[BUFF];
char fileName[BUFF];
char targetFile[BUFF];
int isfileEmpty(FILE* fp, char* caller);
FILE * ptrFile;

int main(int argc, char** argv)
{
	strcpy(progFile,argv[0]);
	
	if(argc == 2)
	{
		strcpy(fileName, argv[1]);
		strcpy(targetFile, argv[1]);
		strcat(targetFile,".asm");

	}
	else if(argc < 2)
	{
		strcpy(fileName,"output.log");
		strcpy(targetFile,"kb.asm");
		FILE *ptr = fopen(fileName, "w");
		int input;	
	
		if(ptr == NULL){
			fprintf(stderr,"ERROR: %s: Cannot open file\"%s\"\n",progFile,fileName);
			return EXIT_FAILURE;
		}

		if((input = getchar()) == EOF)
		{
			fprintf(stderr,"ERROR: %s: Input is empty\n",progFile);
			return EXIT_FAILURE;
		}	
		else
		{
			fprintf(ptr,"%c", input);
		}
		
		while((input = getchar()) != EOF)
			fprintf(ptr,"%c",input);	
		
		fclose(ptr);	
	}
	else
	{
		fprintf(stderr,"ERROR: %s: Please provide one file as an argument to the program\n",progFile);
		return EXIT_FAILURE;
	}

	struct node* root = parser(fileName); 
	struct Stack* stack = createStack(MAXVARS);
	
	ptrFile = fopen(targetFile,"w");		
	preorder(root,stack);	

	printf("Parser successfully\n");	
	
	return EXIT_SUCCESS;		
}







