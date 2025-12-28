#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <list>
#include <algorithm>
#include <iostream>
#include <vector>

enum AllocationAlgo {
    FIRST_FIT,  // 最先适应
    BEST_FIT,   // 最佳适应
    WORST_FIT   // 最坏适应
};

struct FreeBlock {
    int startAddr;
    int size;
};

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

    bool requestMemory(int pid, int segCount, const std::vector<int>& segSizes, AllocationAlgo algo);

    void releaseMemory(int pid);

    const std::list<FreeBlock>& getFreeList() const;
    const std::list<AllocBlock>& getAllocList() const;
    int getTotalMemory() const;

private:
    int totalMemory;
    std::list<FreeBlock> freeList;
    std::list<AllocBlock> allocList;

    void coalesce();
};

#endif // MEMORYMANAGER_H