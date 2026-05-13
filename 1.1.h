#include<stdio.h>
#include<stdlib.h>
#include<string>

#define MAXSIZE 100
#define CODE 6

typedef struct Package
{
    char ID[MAXSIZE];
    char name[MAXSIZE];
    char phone[12];
    char company[MAXSIZE];
    char code[CODE];

};


