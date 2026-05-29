#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 包裹状态枚举
typedef enum {
    STATUS_IN_STORAGE,   // 在库
    STATUS_PICKED_UP,    // 已取件
    STATUS_EXPIRED       // 已过期
} PackageStatus;

// 包裹结构体
typedef struct Package {
    char trackingNumber[20];   // 快递单号
    char receiver[50];         // 收件人
    char phone[15];            // 联系电话
    char pickupCode[7];        // 6位取件码(含1位校验位)
    time_t storageTime;        // 入库时间
    PackageStatus status;      // 包裹状态
    struct Package* next;      // 指向同一层的下一个包裹
} Package;

// 货架层结构体
typedef struct ShelfLevel {
    int levelNumber;           // 层序号(从1开始)
    int maxCapacity;          // 该层最大容量
    int currentCount;         // 当前包裹数量
    Package* packageHead;     // 该层包裹链表头指针
    struct ShelfLevel* next;  // 指向下一层
} ShelfLevel;

// 货架结构体
typedef struct Shelf {
    int shelfId;              // 货架编号
    int totalLevels;          // 总层数
    ShelfLevel* levelHead;    // 层链表头指针
    struct Shelf* next;       // 指向下一个货架
} Shelf;

// 全局货架链表头指针
Shelf* shelfHead = NULL;

// 初始化随机数种子
void initRandomSeed() {
    srand((unsigned int)time(NULL));
}

// 生成6位随机取件码(纯数字)
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

// 创建一个新包裹
Package* createPackage(const char* trackingNo, const char* receiver, const char* phone) {
    Package* newPackage = (Package*)malloc(sizeof(Package));
    if (newPackage == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    
    strcpy(newPackage->trackingNumber, trackingNo);
    strcpy(newPackage->receiver, receiver);
    strcpy(newPackage->phone, phone);
    generatePickupCode(newPackage->pickupCode);
    newPackage->storageTime = time(NULL);
    newPackage->status = STATUS_IN_STORAGE;
    newPackage->next = NULL;
    
    return newPackage;
}

// 创建新层
ShelfLevel* createLevel(int levelNum, int capacity) {
    ShelfLevel* newLevel = (ShelfLevel*)malloc(sizeof(ShelfLevel));
    if (newLevel == NULL) {
        printf("内存分配失败！\n");
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
Shelf* createShelf(int id, int levels) {
    Shelf* newShelf = (Shelf*)malloc(sizeof(Shelf));
    if (newShelf == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    
    newShelf->shelfId = id;
    newShelf->totalLevels = levels;
    newShelf->levelHead = NULL;
    newShelf->next = NULL;
    
    for (int i = 1; i <= levels; i++) {
        ShelfLevel* level = createLevel(i, 20);
        if (newShelf->levelHead == NULL) {
            newShelf->levelHead = level;
        } else {
            ShelfLevel* temp = newShelf->levelHead;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = level;
        }
    }
    
    return newShelf;
}

// 添加货架到全局链表
void addShelf(Shelf* shelf) {
    if (shelfHead == NULL) {
        shelfHead = shelf;
    } else {
        Shelf* temp = shelfHead;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = shelf;
    }
}

// 根据货架ID查找货架
Shelf* findShelfById(int shelfId) {
    Shelf* shelf = shelfHead;
    while (shelf != NULL) {
        if (shelf->shelfId == shelfId) {
            return shelf;
        }
        shelf = shelf->next;
    }
    return NULL;
}

// 找到指定货架的指定层
ShelfLevel* findLevel(Shelf* shelf, int levelNum) {
    if (shelf == NULL) return NULL;
    
    ShelfLevel* level = shelf->levelHead;
    while (level != NULL) {
        if (level->levelNumber == levelNum) {
            return level;
        }
        level = level->next;
    }
    return NULL;
}

// 入库：将包裹添加到指定货架的指定层
int storePackage(Shelf* shelf, int levelNum, Package* package) {
    if (shelf == NULL || package == NULL) {
        printf("货架或包裹无效！\n");
        return 0;
    }
    
    ShelfLevel* level = findLevel(shelf, levelNum);
    if (level == NULL) {
        printf("未找到第%d层！\n", levelNum);
        return 0;
    }
    
    if (level->currentCount >= level->maxCapacity) {
        printf("第%d层已满，无法存放包裹！\n", levelNum);
        return 0;
    }
    
    // 检查是否重复入库
    Package* temp = level->packageHead;
    while (temp != NULL) {
        if (strcmp(temp->trackingNumber, package->trackingNumber) == 0) {
            printf("包裹%s已在该层存在！\n", package->trackingNumber);
            return 0;
        }
        temp = temp->next;
    }
    
    // 添加到该层包裹链表头部
    package->next = level->packageHead;
    level->packageHead = package;
    level->currentCount++;
    package->status = STATUS_IN_STORAGE;
    
    printf("包裹 %s 已入库到货架%d第%d层，取件码：%s\n", 
           package->trackingNumber, shelf->shelfId, levelNum, package->pickupCode);
    return 1;
}

// 根据取件码查找包裹(遍历所有货架)
Package* findByPickupCode(const char* code) {
    Shelf* shelf = shelfHead;
    while (shelf != NULL) {
        ShelfLevel* level = shelf->levelHead;
        while (level != NULL) {
            Package* pkg = level->packageHead;
            while (pkg != NULL) {
                if (strcmp(pkg->pickupCode, code) == 0) {
                    return pkg;
                }
                pkg = pkg->next;
            }
            level = level->next;
        }
        shelf = shelf->next;
    }
    return NULL;
}

// 根据手机号查找包裹
void findByPhone(const char* phone) {
    int found = 0;
    Shelf* shelf = shelfHead;
    while (shelf != NULL) {
        ShelfLevel* level = shelf->levelHead;
        while (level != NULL) {
            Package* pkg = level->packageHead;
            while (pkg != NULL) {
                if (strcmp(pkg->phone, phone) == 0 && pkg->status == STATUS_IN_STORAGE) {
                    printf("找到包裹：单号%s, 收件人：%s, 取件码：%s, 位置：货架%d第%d层\n",
                           pkg->trackingNumber, pkg->receiver, pkg->pickupCode,
                           shelf->shelfId, level->levelNumber);
                    found++;
                }
                pkg = pkg->next;
            }
            level = level->next;
        }
        shelf = shelf->next;
    }
    if (found == 0) {
        printf("未找到手机号%s对应的在库包裹\n", phone);
    }
}

// 打印所有货架信息
void printAllShelves() {
    if (shelfHead == NULL) {
        printf("当前没有货架！\n");
        return;
    }
    
    Shelf* shelf = shelfHead;
    while (shelf != NULL) {
        printf("\n========== 货架 %d 信息 ==========\n", shelf->shelfId);
        printf("总层数：%d\n", shelf->totalLevels);
        
        ShelfLevel* level = shelf->levelHead;
        while (level != NULL) {
            printf("  第%d层：容量%d/%d\n", 
                   level->levelNumber, level->currentCount, level->maxCapacity);
            
            Package* pkg = level->packageHead;
            while (pkg != NULL) {
                printf("    单号：%s, 收件人：%s, 取件码：%s, 状态：",
                       pkg->trackingNumber, pkg->receiver, pkg->pickupCode);
                switch(pkg->status) {
                    case STATUS_IN_STORAGE: printf("在库"); break;
                    case STATUS_PICKED_UP: printf("已取件"); break;
                    case STATUS_EXPIRED: printf("已过期"); break;
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
void freeAll() {
    Shelf* shelf = shelfHead;
    while (shelf != NULL) {
        ShelfLevel* level = shelf->levelHead;
        while (level != NULL) {
            Package* pkg = level->packageHead;
            while (pkg != NULL) {
                Package* tempPkg = pkg;
                pkg = pkg->next;
                free(tempPkg);
            }
            ShelfLevel* tempLevel = level;
            level = level->next;
            free(tempLevel);
        }
        Shelf* tempShelf = shelf;
        shelf = shelf->next;
        free(tempShelf);
    }
    shelfHead = NULL;
}

// ==================== 用户交互功能 ====================

// 包裹入库功能
void menu_StorePackage() {
    char trackingNo[20];
    char receiver[50];
    char phone[15];
    int shelfId, levelNum;
    
    printf("\n========== 包裹入库 ==========\n");
    printf("请输入快递单号：");
    scanf("%s", trackingNo);
    printf("请输入收件人姓名：");
    scanf("%s", receiver);
    printf("请输入联系电话：");
    scanf("%s", phone);
    
    // 创建包裹
    Package* newPkg = createPackage(trackingNo, receiver, phone);
    if (newPkg == NULL) {
        printf("包裹创建失败！\n");
        return;
    }
    
    // 显示可用货架
    printf("\n当前可用货架：\n");
    Shelf* shelf = shelfHead;
    while (shelf != NULL) {
        printf("  货架%d (%d层)\n", shelf->shelfId, shelf->totalLevels);
        shelf = shelf->next;
    }
    
    printf("\n请输入存入货架号：");
    scanf("%d", &shelfId);
    printf("请输入存入层号：");
    scanf("%d", &levelNum);
    
    Shelf* targetShelf = findShelfById(shelfId);
    if (targetShelf == NULL) {
        printf("货架%d不存在！\n", shelfId);
        free(newPkg);
        return;
    }
    
    if (levelNum < 1 || levelNum > targetShelf->totalLevels) {
        printf("层号无效！货架%d只有%d层\n", shelfId, targetShelf->totalLevels);
        free(newPkg);
        return;
    }
    
    storePackage(targetShelf, levelNum, newPkg);
}

// 取件功能
void menu_PickupPackage() {
    char pickupCode[7];
    
    printf("\n========== 包裹取件 ==========\n");
    printf("请输入6位取件码：");
    scanf("%s", pickupCode);
    
    Package* pkg = findByPickupCode(pickupCode);
    if (pkg == NULL) {
        printf("未找到取件码为%s的包裹！\n", pickupCode);
        return;
    }
    
    if (pkg->status == STATUS_PICKED_UP) {
        printf("该包裹已被取走！\n");
        return;
    }
    
    if (pkg->status == STATUS_EXPIRED) {
        printf("该包裹已过期！\n");
        return;
    }
    
    // 显示包裹信息
    printf("\n找到包裹：\n");
    printf("  快递单号：%s\n", pkg->trackingNumber);
    printf("  收件人：%s\n", pkg->receiver);
    printf("  联系电话：%s\n", pkg->phone);
    printf("  入库时间：%s", ctime(&pkg->storageTime));
    
    char confirm;
    printf("确认取件？(y/n): ");
    scanf(" %c", &confirm);
    
    if (confirm == 'y' || confirm == 'Y') {
        pkg->status = STATUS_PICKED_UP;
        printf("取件成功！包裹%s已标记为已取件\n", pkg->trackingNumber);
    } else {
        printf("已取消取件操作\n");
    }
}

// 根据手机号查询包裹
void menu_SearchByPhone() {
    char phone[15];
    
    printf("\n========== 查询包裹 ==========\n");
    printf("请输入手机号码：");
    scanf("%s", phone);
    
    findByPhone(phone);
}

// 查看所有货架信息
void menu_ViewAllShelves() {
    printf("\n========== 所有货架信息 ==========");
    printAllShelves();
}

// 创建新货架
void menu_CreateShelf() {
    int shelfId, levels;
    
    printf("\n========== 创建新货架 ==========\n");
    printf("请输入货架编号：");
    scanf("%d", &shelfId);
    
    // 检查货架是否已存在
    if (findShelfById(shelfId) != NULL) {
        printf("货架%d已存在！\n", shelfId);
        return;
    }
    
    printf("请输入货架层数：");
    scanf("%d", &levels);
    
    if (levels <= 0) {
        printf("层数必须大于0！\n");
        return;
    }
    
    Shelf* newShelf = createShelf(shelfId, levels);
    if (newShelf != NULL) {
        addShelf(newShelf);
        printf("货架%d创建成功！共%d层\n", shelfId, levels);
    }
}

// 主菜单
void showMenu() {
    printf("\n");
    printf("╔════════════════════════════════╗\n");
    printf("║     快递站包裹管理系统        ║\n");
    printf("╠════════════════════════════════╣\n");
    printf("║  1. 包裹入库                  ║\n");
    printf("║  2. 包裹取件                  ║\n");
    printf("║  3. 手机号查询                ║\n");
    printf("║  4. 查看所有货架              ║\n");
    printf("║  5. 创建新货架                ║\n");
    printf("║  0. 退出系统                  ║\n");
    printf("╚════════════════════════════════╝\n");
    printf("请选择操作：");
}

// 主函数
int main() {
    initRandomSeed();
    
    // 初始化：创建2个默认货架，每个5层
    printf("初始化系统...\n");
    Shelf* shelf1 = createShelf(1, 5);
    Shelf* shelf2 = createShelf(2, 5);
    addShelf(shelf1);
    addShelf(shelf2);
    printf("系统初始化完成！已创建2个默认货架\n");
    
    int choice;
    do {
        showMenu();
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                menu_StorePackage();
                break;
            case 2:
                menu_PickupPackage();
                break;
            case 3:
                menu_SearchByPhone();
                break;
            case 4:
                menu_ViewAllShelves();
                break;
            case 5:
                menu_CreateShelf();
                break;
            case 0:
                printf("感谢使用，再见！\n");
                break;
            default:
                printf("无效选择，请重新输入！\n");
        }
    } while(choice != 0);
    
    // 清理内存
    freeAll();
    
    return 0;
}