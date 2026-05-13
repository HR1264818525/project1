#define MAXSIZE 100
#define CODE 6
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

typedef struct package
{
    char ID;
    char phone[15];
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
    char phone[15];
    Package *newPac = malloc(sizeof(Package));
    printf("请输入客户的名字：");
    scanf("%s",name);
    printf("请输入客户的电话号码：");
    scanf("%s",phone);

    printf("包裹存放货架位置：");
    scanf("%d",&newPac->posNum);
    printf("包裹所在在货架位置：");
    scanf("%d",&newPac->shelfNum);
    strcpy(newPac->name,name);
    strcpy(newPac->phone,phone);

    generateCode(newPac->code);         //随机取件码

    newPac->ID = ++totalpackage;

    Position *pos = &shelves[newPac->posNum-1][newPac->shelfNum-1];
    newPac->next = pos->top;
    pos->top = newPac;
    pos->count++;

    printf("包裹已入库\n");
    printf("包裹编号为:%d\n",newPac->ID);
    printf("姓名：%s\n",newPac->name);
    printf("电话：%s\n",newPac->phone);
    printf("所在位置：%d号货架 %d号柜\n",newPac->posNum,newPac->shelfNum);
    printf("取件码：%s\n",newPac->code);
}


// 取件操作：输入取件码
void pickupPackage()
{
    char code[CODE];
    printf("请输入取件码：");
    scanf("%s",code);
    Package *current = malloc(sizeof(Package));
    Package *prev = NULL;
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 21; j++)
        {
            if(shelves[i][j].top!=NULL)
            {
                current = &shelves[i][j];
                if (current->code == code)
                {
                    
                }
                
            }
            
        }
        
    }
    

}