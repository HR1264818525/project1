#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "struct.h"
#include "init.h"

// 根据货架ID查找货架
Shelf *findShelfById(int shelfId)
{
    Shelf *shelf = shelfHead;
    while (shelf != NULL)
    {
        if (shelf->shelfId == shelfId)
        {
            return shelf;
        }
        shelf = shelf->next;
    }
    return NULL;
}

// 找到指定货架的指定层
ShelfLevel *findLevel(Shelf *shelf, int levelNum)
{
    if (shelf == NULL)
        return NULL;

    ShelfLevel *level = shelf->levelHead;
    while (level != NULL)
    {
        if (level->levelNumber == levelNum)
        {
            return level;
        }
        level = level->next;
    }
    return NULL;
}

// 入库
int storePackage(Shelf *shelf, int levelNum, Package *package)
{
    if (shelf == NULL || package == NULL)
    {
        printf("货架或包裹无效！\n");
        return 0;
    }

    ShelfLevel *level = findLevel(shelf, levelNum);
    if (level == NULL)
    {
        printf("未找到第%d层！\n", levelNum);
        return 0;
    }

    if (level->currentCount >= level->maxCapacity)
    {
        printf("第%d层已满，无法存放包裹！\n", levelNum);
        return 0;
    }

    // 检查是否重复入库
    Package *temp = level->packageHead;
    while (temp != NULL)
    {
        if (strcmp(temp->ID, package->ID) == 0)
        {
            printf("包裹%s已在该层存在！\n", package->ID);
            return 0;
        }
        temp = temp->next;
    }

    // 头插法入库
    package->next = level->packageHead;
    level->packageHead = package;
    level->currentCount++;
    package->status = IN_STORAGE;

    printf("包裹 %s 已入库到货架%d第%d层，取件码：%s\n",
           package->ID, shelf->shelfId, levelNum, package->pickupCode);
    return 1;
}

// 根据手机号查找包裹
void findByPhone(const char *phone)
{
    int found = 0;
    Shelf *shelf = shelfHead;
    while (shelf != NULL)
    {
        ShelfLevel *level = shelf->levelHead;
        while (level != NULL)
        {
            Package *pkg = level->packageHead;
            while (pkg != NULL)
            {
                if (strcmp(pkg->phone, phone) == 0 && pkg->status == IN_STORAGE)
                {
                    printf("找到包裹：单号%s, 收件人：%s, 取件码：%s, 位置：货架%d第%d层\n",
                           pkg->ID, pkg->name, pkg->pickupCode,
                           shelf->shelfId, level->levelNumber);
                    found++;
                }
                pkg = pkg->next;
            }
            level = level->next;
        }
        shelf = shelf->next;
    }
    if (found == 0)
    {
        printf("未找到手机号%s对应的在库包裹\n", phone);
    }
}

// 打印所有货架信息
void printAllShelves()
{
    if (shelfHead == NULL)
    {
        printf("当前没有货架！\n");
        return;
    }

    Shelf *shelf = shelfHead;
    while (shelf != NULL)
    {
        printf("\n========== 货架 %d 信息 ==========\n", shelf->shelfId);
        printf("总层数：%d\n", shelf->totalLevels);

        ShelfLevel *level = shelf->levelHead;
        while (level != NULL)
        {
            printf("  第%d层：容量%d/%d\n",
                   level->levelNumber, level->currentCount, level->maxCapacity);

            Package *pkg = level->packageHead;
            while (pkg != NULL)
            {
                printf("    单号：%s, 收件人：%s, 取件码：%s, 状态：",
                       pkg->ID, pkg->name, pkg->pickupCode);
                switch (pkg->status)
                {
                case IN_STORAGE:
                    printf("在库");
                    break;
                case PICK_UP:
                    printf("已取件");
                    break;
                }
                printf("\n");
                pkg = pkg->next;
            }
            level = level->next;
        }
        shelf = shelf->next;
    }
}

// 清理内存
void freeAll()
{
    Shelf *shelf = shelfHead;
    while (shelf != NULL)
    {
        ShelfLevel *level = shelf->levelHead;
        while (level != NULL)
        {
            Package *pkg = level->packageHead;
            while (pkg != NULL)
            {
                Package *tempPkg = pkg;
                pkg = pkg->next;
                free(tempPkg);
            }
            ShelfLevel *tempLevel = level;
            level = level->next;
            free(tempLevel);
        }
        Shelf *tempShelf = shelf;
        shelf = shelf->next;
        free(tempShelf);
    }
    shelfHead = NULL;
}