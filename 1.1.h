#include <time.h>

typedef enum {
    IN_STORAGE,  
    PICK_UP,    
} PackageStatus;

// 包裹结构体
typedef struct Package {
    char ID[20];   // 快递单号
    char name[50];         // 收件人
    char phone[12];            // 联系电话
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