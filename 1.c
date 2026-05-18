#include <stdio.h>
#include "1.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>


void initRandomSeed() {
    srand((unsigned int)time(NULL));
}


void generatePickupCode(char* code) {
    for (int i = 0; i < 5; i++) {
        code[i] = '0' + rand() % 10;
    }
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += code[i] - '0';
    }
    code[5] = '0' + (sum % 10);
    code[6] = '\0';
}



Package *CreatPac(const char *ID,const char *name,const char *phone)
{
    Package *newPac = malloc(sizeof(Package));
    if (newPac == NULL)
    {
        return NULL;
    }
    strcpy(newPac->ID,ID);
    strcpy(newPac->name,name);
    strcpy(newPac->phone,phone);
    generatePickupCode(newPac->code);
    newPac->next = NULL;
    
    return newPac;
}

ShelfLevel *CreatLevel(int levelnum,int maxsize)
{
    ShelfLevel *level = malloc(sizeof(ShelfLevel));
    if (level == NULL)
    {
        return NULL;
    }
    level->LevNum = levelnum;
    level->MaxSize = maxsize;
    level->CurSize = 0;
    level->HeadPac = NULL;
    level->next = NULL;
    
    return level;
}

Shelf *CreatShelf(int shelfnum,int maxsize)
{
    Shelf *shelf = malloc(sizeof(Shelf));
    if (shelf == NULL)
    {
        return NULL;
    }
    shelf->ShelfNum = shelfnum;
    shelf->MaxSize = maxsize;
    shelf->HeadLevel = NULL;
    shelf->next = NULL;
    
    for (int i = 0; i < BIGLEVELNUM; i++)
    {
        ShelfLevel *newlevel = CreatLevel(i,maxsize);
        if(shelf->HeadLevel ==NULL)
        {
            shelf->HeadLevel = newlevel;
        }
        else
        {
            ShelfLevel *temp = shelf->HeadLevel;
            while (temp!=NULL)
            {
                temp=temp->next;
            }
            newlevel = temp;
        }
    }
    return shelf;
}

Shelf *shelfhead=NULL;

void addshelf(Shelf *shelf)
{
    for (int i = 0; i < BIGSHELFNUM; i++)
    {
        if (shelfhead->next == NULL)
        {
            shelfhead->next = shelf;
        }
        else
        {
            Shelf *temp = shelfhead;
            while (temp != NULL)
            {
                temp = temp->next;
            }
            temp->next = shelf;
        }
    }
    return shelf;
}


ShelfLevel *findlevel(Shelf *shelf,int levelnum)
{
    ShelfLevel *level = shelf->HeadLevel;
    while (level!=NULL)
    {
        if (level->LevNum ==levelnum)
        {
            return level;
        }
        level = level->next;
    }
    return NULL;
}

Shelf *findshelf(int shelfnum)
{
    Shelf *shelf = shelfhead->next;
    while (shelf!=NULL)
    {
        if (shelf->ShelfNum = shelfnum)
        {
            return shelf;
        }
        shelf = shelf->next;
    }
    return NULL;
}


void PacStore(Package *newPac)
{
    printf("请输入入库货架号：");


}








int main()
{
    srand(time(NULL));
    
    return 0;
}
