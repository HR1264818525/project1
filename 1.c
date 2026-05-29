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



int Pacstore(Shelf *shelf,int levelnum,Package *package)
{
    ShelfLevel *level = findlevel(shelf,levelnum);
    if (shelf == NULL||package ==NULL)
    {
        printf("没有包裹信息\n");
        return 0;
    }

    if (level == NULL)
    {
        printf("该货架没有该层\n");
        return 0;
    }
    
    if (level->CurSize>=level->MaxSize)
    {
        printf("该层包裹已满\n");
        return 0;
    }
    package->next = level->HeadPac;
    level->HeadPac = package;
    level->CurSize++;
    printf("包裹%s已入库，%d货架%d层取件码%s\n",package->name,shelf->ShelfNum,
        level->LevNum,package->code);
    return 1;

}

Package *findbycode(const char * code)
{
    Shelf *shelf = shelfhead;
    while (shelf!=NULL)
    {
        ShelfLevel *level = shelf->HeadLevel;
        while (level!=NULL)
        {
            Package *package = level->HeadPac;
            while (package!=NULL)
            {
                if (strcmp(package->code,code)==0)
                {
                    printf("成功找到取件码为%s的包裹",code);
                    return package;
                }
                package = package->next;
            }
            level = level->next;
        }
        shelf = shelf->next;
    }
    return NULL;
}

void menu_storepackage()
{
    char ID[20];
    char phone[12];
    char name[20];
    int shelfnum;
    int levelnum;

    printf("请输入包裹ID：");
    scanf("%s",ID);
    printf("请输入电话号码：");
    scanf("%s",phone);
    printf("请输入取件人姓名：");
    scanf("%s",name);

    Package *package = CreatPac(ID,name,phone);
    if (package == NULL)
    {
        printf("包裹创建失败\n");
        return;
    }
    
    printf("已记录包裹信息\n");

    Shelf *shelf = shelfhead;
    while (shelf!=NULL)
    {
        printf("目前可用货架：%d，层数%d\n",shelf->ShelfNum,shelf->MaxSize);
        shelf = shelf->next;
    }
    printf("请输入存入货架：");
    scanf("%d",&shelfnum);
    printf("请输入存入第：层");
    scanf("%d",&levelnum);

    Shelf *targetshelf = findshelf(shelfnum);
    if (targetshelf == NULL)
    {
        printf("该层为空\n");
        free(targetshelf);
        return;
    }
    
    if (shelfnum<=0||shelfnum>=shelf->MaxSize)
    {
        printf("超出范围\n");
        free(targetshelf);
        return;
    }
    
    Pacstore(targetshelf,levelnum,package);
}

void menu_PickupPackage()
{
    char pickupCode[7];

    printf("包裹取件\n");
    printf("请输入取件码：");
    scanf("%s",pickupCode);

    Package *package = findbycode(pickupCode);
    if (package==NULL)
    {
        printf("未找到该包裹\n");
        return;
    }

    printf("找到包裹，收件人：%s，电话：%s，ID：%s",
        package->name,package->phone,package->ID);
}

void showMenu() {
    printf("\n");
    printf("╔════════════════════════════════╗\n");
    printf("║     快递站包裹管理系统        ║\n");
    printf("╠════════════════════════════════╣\n");
    printf("║  1. 包裹入库                  ║\n");
    printf("║  2. 包裹取件                  ║\n");
    printf("╚════════════════════════════════╝\n");
    printf("请选择操作：");
}

int main()
{
    srand(time(NULL));
    Shelf *shelf1 = CreatShelf(1,10);
    Shelf *shelf2 = CreatShelf(2,10);
    addshelf(shelf1);
    addshelf(shelf2);
    printf("系统初始化完成！已创建2个默认货架\n");

    int choice;
    do
    {
        showMenu();
        scanf("%d",&choice);
        switch (choice)
        {
        case 1:
            menu_storepackage();
            break;
        case 2:
            menu_PickupPackage();
            break;
        default:
            break;
        }
    }while(choice!=0);
    

    return 0;
}
