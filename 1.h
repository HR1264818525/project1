#define MAXSIZE 100
#define CODE 6
#include <stdlib.h>

typedef struct package
{
    char ID;
    int phone;
    char name[MAXSIZE];
    char code[CODE];
    int posNum;
    int shelfNum;
    struct package*next;
}Package;           //包裹结构体

//栈
typedef struct position
{
    Package *top;
    int count;
}Position;

//全局变量
Position shelves[10][20];       //货架
int totalpackage;

//初始化货架
int initshelves()
{
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 21; i++)
        {
            shelves[i][j].top = NULL;
            shelves[i][j].count = 0;
        }
    }
}

void generateCode(char *code)
{
    for (int i = 0; i < CODE; i++)
    {
        code[i] = '0' + rand() % 10;
    }
    code[7] = '\0';
}


//包裹入库
void storePackage()
{
    char name[MAXSIZE];
    Package *newPac = malloc(sizeof(Package));
    printf("请输入客户的名字：");
    scanf("%c",name);
    printf("请输入客户的电话号码：");
    scanf("%d",&newPac->phone);

    printf("包裹存放货架位置：");
    scanf("%d",&newPac->posNum);
    printf("包裹所在在货架位置：");
    scanf("%d",&newPac->shelfNum);

    generateCode(newPac->code);         //随机取件码

    newPac->ID = ++totalpackage;

    Position *pos = &shelves[newPac->posNum-1][newPac->shelfNum-1];
    newPac->next = pos->top;
    pos->top = newPac;
    pos->count++;

    printf("包裹已入库");
    printf("包裹编号为:%d",newPac->ID);
    printf("姓名：%s",newPac->name);
    printf("电话：%d",newPac->phone);
    printf("所在位置：%d号货架 %d号柜",newPac->posNum,newPac->shelfNum);
    printf("取件码：%s",newPac->code);
}