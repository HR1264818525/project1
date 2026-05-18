// #include<stdio.h>
// #include<stdlib.h>
// #include <ctype.h>

#define MAXSIZE 100
#define CODE 6
#define LEVELMAXSIZE 20
#define BIGLEVELNUM 5
#define BIGSHELFNUM 10

typedef struct package
{
    char ID[MAXSIZE];
    char name[MAXSIZE];
    char phone[12];
    char code[CODE];
    struct package *next;

}Package;

typedef struct shelflevel
{
    int LevNum;
    int MaxSize;
    int CurSize;
    Package *HeadPac;
    struct shelflevel *next;
}ShelfLevel;

typedef struct shelf
{
    int ShelfNum;
    int MaxSize;
    ShelfLevel *HeadLevel;
    struct shelf *next;
}Shelf;





