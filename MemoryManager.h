#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <list>
#include <algorithm>
#include <iostream>

// 空闲块结构
struct FreeBlock {
    int startAddr;
    int size;
};

// 已分配块结构
struct AllocBlock {
    int pid;
    int segId;
    int startAddr;
    int size;
};

class MemoryManager {
public:
    MemoryManager();
    ~MemoryManager();

    // 初始化内存
    void initMemory(int totalSize);

    // 申请内存 (最坏适应算法)
    // 返回 true 成功，false 失败
    bool requestMemory(int pid, int segCount, const std::vector<int>& segSizes);

    // 回收内存 (包含合并逻辑)
    void releaseMemory(int pid);

    // 获取数据供绘图使用
    const std::list<FreeBlock>& getFreeList() const;
    const std::list<AllocBlock>& getAllocList() const;
    int getTotalMemory() const;

private:
    int totalMemory;
    std::list<FreeBlock> freeList;   // 空闲链表
    std::list<AllocBlock> allocList; // 已分配链表

    // 内部辅助：合并相邻空闲区
    void coalesce();
};

#endif // MEMORYMANAGER_H