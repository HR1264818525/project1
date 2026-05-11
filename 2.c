#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ============ 定义包裹结构体 ============
// 结构体就是一种"自定义的数据类型"，把多个信息打包在一起
typedef struct Package {
    int id;                    // 包裹编号（自动生成）
    char name[50];            // 收件人姓名
    char phone[15];           // 收件人电话
    char code[7];             // 取件码（6位数字）
    int shelfNum;             // 放在哪个货架（1-3号柜）
    int positionNum;          // 放在哪个位置（1-5号格）
    struct Package* next;     // 指针：指向同一个格子的下一个包裹
} Package;

// ============ 定义货架结构体 ============
// 每个格子就是一个"栈"，用链表实现
typedef struct {
    Package* top;    // 指向这个格子最上面的包裹（栈顶）
    int count;       // 这个格子里有几个包裹
} Position;

// 全球变量（全局都能访问的变量）
Position shelves[3][5];  // 3个货架，每个货架5个位置
int totalPackages = 0;   // 总包裹数

// ============ 初始化函数 ============
void initSystem() {
    // 把所有格子清空
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            shelves[i][j].top = NULL;  // NULL表示空
            shelves[i][j].count = 0;
        }
    }
    printf("系统初始化完成！\n");
    printf("我们有3个货架(1-3号)，每个货架有5个位置(1-5号)\n");
}

// ============ 生成随机取件码 ============
void generateCode(char* code) {
    for (int i = 0; i < 6; i++) {
        code[i] = '0' + rand() % 10;  // 随机生成0-9的数字
    }
    code[6] = '\0';  // 字符串结束符
}

// ============ 核心功能1：存放包裹（入库）============
void storePackage() {
    char name[50], phone[15];
    int shelfNum, posNum;
    
    printf("\n========== 存放包裹 ==========\n");
    printf("请输入收件人姓名：");
    scanf("%s", name);
    printf("请输入收件人电话：");
    scanf("%s", phone);
    
    // 显示可用的位置
    printf("\n请选择存放位置：\n");
    printf("货架编号：1-3\n");
    printf("位置编号：1-5\n");
    printf("请输入货架编号：");
    scanf("%d", &shelfNum);
    printf("请输入位置编号：");
    scanf("%d", &posNum);
    
    // 检查输入是否合法
    if (shelfNum < 1 || shelfNum > 3 || posNum < 1 || posNum > 5) {
        printf("✗ 位置无效！货架(1-3)，位置(1-5)\n");
        return;
    }
    
    // 创建新包裹
    Package* newPkg = (Package*)malloc(sizeof(Package));
    newPkg->id = ++totalPackages;  // 包裹编号自动递增
    strcpy(newPkg->name, name);
    strcpy(newPkg->phone, phone);
    generateCode(newPkg->code);    // 自动生成取件码
    newPkg->shelfNum = shelfNum;
    newPkg->positionNum = posNum;
    
    // 【关键】栈操作：新包裹放在最上面
    // 获取对应的格子
    Position* pos = &shelves[shelfNum-1][posNum-1];
    
    // 新的包裹指向原来的栈顶
    newPkg->next = pos->top;
    // 新的包裹成为新的栈顶
    pos->top = newPkg;
    pos->count++;
    
    // 打印存放信息
    printf("\n✓ 存放成功！\n");
    printf("┌──────────────────────┐\n");
    printf("│ 取件码：%s      │\n", newPkg->code);
    printf("│ 位置：%d号柜 %d号格  │\n", shelfNum, posNum);
    printf("│ 该位置已有%d个包裹   │\n", pos->count);
    printf("└──────────────────────┘\n");
    printf("请记住取件码！\n");
}

// ============ 核心功能2：取走包裹（出库）============
void pickupPackage() {
    char code[7];
    
    printf("\n========== 取走包裹 ==========\n");
    printf("请输入取件码：");
    scanf("%s", code);
    
    // 遍历所有位置查找包裹
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            Package* current = shelves[i][j].top;
            Package* prev = NULL;
            
            // 在这个位置的包裹链中查找
            while (current != NULL) {
                if (strcmp(current->code, code) == 0) {
                    // 找到了！
                    printf("\n找到包裹：\n");
                    printf("收件人：%s\n", current->name);
                    printf("位置：%d号柜 %d号格\n", i+1, j+1);
                    
                    // 【关键】从栈中移除包裹
                    if (prev == NULL) {
                        // 要取的是栈顶包裹
                        shelves[i][j].top = current->next;
                    } else {
                        // 要取的不是栈顶
                        prev->next = current->next;
                    }
                    
                    shelves[i][j].count--;
                    free(current);  // 释放内存
                    
                    printf("✓ 取件成功！\n");
                    return;
                }
                prev = current;
                current = current->next;
            }
        }
    }
    
    printf("✗ 未找到该取件码！\n");
}

// ============ 核心功能3：查看所有包裹 ============
void viewAllPackages() {
    printf("\n========== 当前所有包裹 ==========\n");
    
    int found = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            if (shelves[i][j].top != NULL) {
                printf("\n【%d号柜 %d号格】有%d个包裹：\n", 
                       i+1, j+1, shelves[i][j].count);
                
                Package* current = shelves[i][j].top;
                int level = 1;
                while (current != NULL) {
                    printf("  第%d层(从上往下)：%s (%s) 取件码:%s\n",
                           level++, current->name, current->phone, current->code);
                    current = current->next;
                    found++;
                }
            }
        }
    }
    
    if (found == 0) {
        printf("当前没有包裹！\n");
    } else {
        printf("\n总共有%d个包裹\n", found);
    }
}

// ============ 主程序 ============
int main() {
    srand(time(NULL));  // 初始化随机数（用于生成取件码）
    initSystem();
    
    int choice;
    
    while (1) {
        printf("\n╔════════════════════════╗\n");
        printf("║  快递驿站系统 v1.0   ║\n");
        printf("╠════════════════════════╣\n");
        printf("║ 1. 存放包裹          ║\n");
        printf("║ 2. 取走包裹          ║\n");
        printf("║ 3. 查看所有包裹      ║\n");
        printf("║ 0. 退出              ║\n");
        printf("╚════════════════════════╝\n");
        printf("请选择：");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: storePackage(); break;
            case 2: pickupPackage(); break;
            case 3: viewAllPackages(); break;
            case 0: 
                printf("再见！\n");
                return 0;
            default:
                printf("无效选择！\n");
        }
    }
    
    return 0;
}