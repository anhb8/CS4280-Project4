#ifndef PARSER_H
#define PARSER_H

int isfileEmpty(FILE* ptr, char* c);
struct node * parser(char* fileName);
void filter();
void attachToken();
struct node *  program(); 
struct node *  vars();
struct node * block();
struct node * expr();
struct node * N();
struct node * N1();
struct node * A();
struct node * M();
struct node * R();
struct node * stats();
struct node * mStat();
struct node * stat();
struct node * in();
struct node * out();
struct node * if1();
struct node * loop1();
struct node * loop2();
struct node * loop();
struct node * assign();
struct node * R0();
struct node * label();
struct node * goto1();

#endif
