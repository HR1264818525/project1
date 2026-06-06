#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "struct.h"

// 生成6位随机取件码
void initRandomSeed()
{
    srand((unsigned int)time(NULL));
}

void generatePickupCode(char *code)
{
    for (int i = 0; i < 6; i++)
    {
        code[i] = '0' + rand() % 10;
    }
    code[6] = '\0';
}
// 创建一个新包裹
Package *createPackage(const char *ID, const char *name, const char *phone)
{
    Package *newPackage = (Package *)malloc(sizeof(Package));
    if (newPackage == NULL)
    {
        return NULL;
    }

    strcpy(newPackage->ID, ID);
    strcpy(newPackage->name, name);
    strcpy(newPackage->phone, phone);
    generatePickupCode(newPackage->pickupCode);

    newPackage->status = IN_STORAGE;
    newPackage->next = NULL;

    return newPackage;
}

// 创建新货架层
ShelfLevel *createLevel(int levelNum, int capacity)
{
    ShelfLevel *newLevel = (ShelfLevel *)malloc(sizeof(ShelfLevel));
    if (newLevel == NULL)
    {
        return NULL;
    }

    newLevel->levelNumber = levelNum;
    newLevel->maxCapacity = capacity;
    newLevel->currentCount = 0;
    newLevel->packageHead = NULL;
    newLevel->next = NULL;

    return newLevel;
}

// 创建新货架
Shelf *createShelf(int id, int levels)
{
    Shelf *newShelf = (Shelf *)malloc(sizeof(Shelf));
    if (newShelf == NULL)
    {
        return NULL;
    }

    newShelf->shelfId = id;
    newShelf->totalLevels = levels;
    newShelf->levelHead = NULL;
    newShelf->next = NULL;

    for (int i = 1; i <= levels; i++)
    {
        ShelfLevel *level = createLevel(i, levels);
        if (newShelf->levelHead == NULL)
        {
            newShelf->levelHead = level;
        }
        else
        {
            ShelfLevel *temp = newShelf->levelHead;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = level;
        }
    }

    return newShelf;
}

// 定义一个货架头指针
Shelf *shelfHead = NULL;

// 将新建货架加入货架链表
void addShelf(Shelf *shelf)
{
    if (shelfHead == NULL)
    {
        shelfHead = shelf;
    }
    else
    {
        Shelf *temp = shelfHead;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = shelf;
    }
}