#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <list>
#include <algorithm>
#include <iostream>
#include <vector>
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

    void initMemory(int totalSize);

    bool requestMemory(int pid, int segCount, const std::vector<int>& segSizes);

    void releaseMemory(int pid);

    const std::list<FreeBlock>& getFreeList() const;
    const std::list<AllocBlock>& getAllocList() const;
    int getTotalMemory() const;

private:
    int totalMemory;
    std::list<FreeBlock> freeList;
    std::list<AllocBlock> allocList;

    // 内部辅助：合并相邻空闲区
    void coalesce();
};

#endif // MEMORYMANAGER_H