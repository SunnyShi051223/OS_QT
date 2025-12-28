#include "MemoryManager.h"

MemoryManager::MemoryManager() : totalMemory(0) {}

MemoryManager::~MemoryManager() {
    freeList.clear();
    allocList.clear();
}

void MemoryManager::initMemory(int totalSize) {
    this->totalMemory = totalSize;
    freeList.clear();
    allocList.clear();
    // 初始状态：整个内存是一个大的空闲块
    freeList.push_back({0, totalSize});
}

bool MemoryManager::requestMemory(int pid, int segCount, const std::vector<int>& segSizes) {
    // 备份当前状态，以便分配失败时“回滚”
    auto backupFree = freeList;
    auto backupAlloc = allocList;

    for (int i = 0; i < segCount; ++i) {
        int reqSize = segSizes[i];

        // --- 最坏适应算法 (Worst Fit) ---
        // 策略：遍历链表，找到容量最大的那个块
        auto worstIt = freeList.end();
        int maxSize = -1;

        for (auto it = freeList.begin(); it != freeList.end(); ++it) {
            if (it->size >= reqSize && it->size > maxSize) {
                maxSize = it->size;
                worstIt = it;
            }
        }

        // 如果没找到合适的块
        if (worstIt == freeList.end()) {
            // 回滚到分配前的状态 (原子性操作：要么全成功，要么全失败)
            freeList = backupFree;
            allocList = backupAlloc;
            return false;
        }

        // --- 执行分配 ---
        // 1. 记录到已分配表
        allocList.push_back({pid, i, worstIt->startAddr, reqSize});

        // 2. 切分空闲块
        worstIt->startAddr += reqSize;
        worstIt->size -= reqSize;

        // 3. 如果剩余大小为0，删除该节点
        if (worstIt->size == 0) {
            freeList.erase(worstIt);
        }
    }
    return true;
}

void MemoryManager::releaseMemory(int pid) {
    // 1. 从已分配表中找到该进程的所有段，移动到空闲表
    auto it = allocList.begin();
    while (it != allocList.end()) {
        if (it->pid == pid) {
            // 归还空间
            freeList.push_back({it->startAddr, it->size});
            it = allocList.erase(it); // 删除并获取下一个迭代器
        } else {
            ++it;
        }
    }

    // 2. 执行合并逻辑
    coalesce();
}

void MemoryManager::coalesce() {
    if (freeList.empty()) return;

    // 第一步：按地址从小到大排序
    freeList.sort([](const FreeBlock& a, const FreeBlock& b) {
        return a.startAddr < b.startAddr;
    });

    // 第二步：遍历合并
    auto it = freeList.begin();
    while (it != freeList.end()) {
        auto nextIt = std::next(it);
        if (nextIt == freeList.end()) break;

        // 如果 当前块尾地址 == 下一块首地址
        if (it->startAddr + it->size == nextIt->startAddr) {
            it->size += nextIt->size; // 大小累加
            freeList.erase(nextIt);   // 删除后一个节点
            // 注意：不移动 it，因为可能还要和新的下一个合并
        } else {
            ++it;
        }
    }
}

const std::list<FreeBlock>& MemoryManager::getFreeList() const { return freeList; }
const std::list<AllocBlock>& MemoryManager::getAllocList() const { return allocList; }
int MemoryManager::getTotalMemory() const { return totalMemory; }