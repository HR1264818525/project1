#include<stdio.h>
#include<stdlib.h>
#include<string>

#define MAXSIZE 100
#define CODE 6
#define LEVEL 20
#define SHELF 10

typedef struct package
{
    char ID[MAXSIZE];
    char name[MAXSIZE];
    char phone[12];
    char company[MAXSIZE];
    char code[CODE];
    struct package *next;

}Package;

typedef struct shelf
{

    
}Shelf;


