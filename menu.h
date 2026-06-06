#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "struct.h"
#include "function.h"
// 包裹入库功能
void menu_StorePackage()
{
    char ID[20];
    char name[50];
    char phone[12];
    int shelfId, levelNum;

    printf("请输入快递单号：");
    scanf("%s", ID);
    printf("请输入收件人姓名：");
    scanf("%s", name);
    printf("请输入联系电话：");
    scanf("%s", phone);

    // 创建包裹
    Package *newPkg = createPackage(ID, name, phone);
    if (newPkg == NULL)
    {
        printf("包裹创建失败\n");
        return;
    }

    // 显示可用货架
    printf("\n当前可用货架：\n");
    Shelf *shelf = shelfHead;
    while (shelf != NULL)
    {
        printf("  货架%d (%d层)\n", shelf->shelfId, shelf->totalLevels);
        shelf = shelf->next;
    }

    printf("\n请输入存入货架号：");
    scanf("%d", &shelfId);
    printf("请输入存入层号：");
    scanf("%d", &levelNum);

    Shelf *targetShelf = findShelfById(shelfId);
    if (targetShelf == NULL)
    {
        printf("货架%d不存在\n", shelfId);
        free(newPkg);
        return;
    }

    if (levelNum < 1 || levelNum > targetShelf->totalLevels)
    {
        printf("层号无效\n");
        free(newPkg);
        return;
    }

    storePackage(targetShelf, levelNum, newPkg);
}

// 取件功能
void menu_PickupPackage()
{
    char pickupCode[7];

    printf("请输入6位取件码：");
    scanf("%s", pickupCode);

    Shelf *shelf = shelfHead;

    while (shelf != NULL)
    {
        ShelfLevel *level = shelf->levelHead;
        while (level != NULL)
        {
            Package *prev = NULL;
            Package *pkg = level->packageHead;

            while (pkg != NULL)
            {
                if (strcmp(pkg->pickupCode, pickupCode) == 0)
                {
                    // 从链表中删除节点
                    if (prev == NULL)
                    {
                        level->packageHead = pkg->next;
                    }
                    else
                    {
                        prev->next = pkg->next;
                    }

                    printf("取件成功，包裹信息ID：%s，收件人：%s，联系电话：%s\n", pkg->ID, pkg->name, pkg->phone);
                    free(pkg);
                    level->currentCount--;

                    return;
                }
                prev = pkg;
                pkg = pkg->next;
            }
            level = level->next;
        }
        shelf = shelf->next;
    }

    printf("未找到取件码为%s的包裹\n", pickupCode);
}

// 根据手机号查询包裹
void menu_SearchByPhone()
{
    char phone[12];

    printf("请输入手机号码：");
    scanf("%s", phone);

    findByPhone(phone);
}

// 查看所有货架信息
void menu_ViewAllShelves()
{
    printAllShelves();
}

// 主菜单
void showMenu()
{
    printf("1. 包裹入库\n");
    printf("2. 包裹取件\n");
    printf("3. 手机号查询\n");
    printf("4. 查看所有货架\n");
    printf("0. 退出系统\n");

    printf("请选择操作：");
}

// 主函数