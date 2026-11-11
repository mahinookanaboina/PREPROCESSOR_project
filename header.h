#ifndef _HEADER_H
#define _HEADER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void headerInclusion(FILE*,char*,int);
void UserDefineHeader(FILE*,char*,int);
void removeComments(char*);
void replaceMacro(char **,char**,char*,char*,int*);
char *replceArguments(char*,char*,char*);

typedef struct st
{
char formal[20];
char actual[20];
struct st*next;
}arg;


#endif
