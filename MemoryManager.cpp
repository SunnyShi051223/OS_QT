#include "MemoryManager.h"
#include <climits> // 用于 INT_MAX

MemoryManager::MemoryManager() : totalMemory(0) {}

MemoryManager::~MemoryManager() {
    freeList.clear();
    allocList.clear();
}

void MemoryManager::initMemory(int totalSize) {
    this->totalMemory = totalSize;
    freeList.clear();
    allocList.clear();
    freeList.push_back({0, totalSize});
}

// --- 修改：实现三种算法逻辑 ---
bool MemoryManager::requestMemory(int pid, int segCount, const std::vector<int>& segSizes, AllocationAlgo algo) {
    auto backupFree = freeList;
    auto backupAlloc = allocList;

    for (int i = 0; i < segCount; ++i) {
        int reqSize = segSizes[i];

        // 目标空闲块的迭代器
        auto targetIt = freeList.end();

        if (algo == FIRST_FIT) {
            // --- 最先适应算法 (First Fit) ---
            // 策略：找到第一个能容纳的块
            for (auto it = freeList.begin(); it != freeList.end(); ++it) {
                if (it->size >= reqSize) {
                    targetIt = it;
                    break; // 找到了就立刻停止
                }
            }
        }
        else if (algo == BEST_FIT) {
            // --- 最佳适应算法 (Best Fit) ---
            // 策略：找到能容纳且剩余空间最小的块
            int minDiff = INT_MAX;
            for (auto it = freeList.begin(); it != freeList.end(); ++it) {
                if (it->size >= reqSize) {
                    int diff = it->size - reqSize;
                    if (diff < minDiff) {
                        minDiff = diff;
                        targetIt = it;
                    }
                }
            }
        }
        else {
            // --- 最坏适应算法 (Worst Fit) ---
            // 策略：找到容量最大的块
            int maxSize = -1;
            for (auto it = freeList.begin(); it != freeList.end(); ++it) {
                if (it->size >= reqSize && it->size > maxSize) {
                    maxSize = it->size;
                    targetIt = it;
                }
            }
        }

        // 如果没找到合适的块，回滚
        if (targetIt == freeList.end()) {
            freeList = backupFree;
            allocList = backupAlloc;
            return false;
        }

        // --- 执行分配 ---
        allocList.push_back({pid, i, targetIt->startAddr, reqSize});
        targetIt->startAddr += reqSize;
        targetIt->size -= reqSize;

        if (targetIt->size == 0) {
            freeList.erase(targetIt);
        }
    }
    return true;
}

void MemoryManager::releaseMemory(int pid) {
    auto it = allocList.begin();
    while (it != allocList.end()) {
        if (it->pid == pid) {
            freeList.push_back({it->startAddr, it->size});
            it = allocList.erase(it);
        } else {
            ++it;
        }
    }
    coalesce();
}

void MemoryManager::coalesce() {
    if (freeList.empty()) return;

    freeList.sort([](const FreeBlock& a, const FreeBlock& b) {
        return a.startAddr < b.startAddr;
    });

    auto it = freeList.begin();
    while (it != freeList.end()) {
        auto nextIt = std::next(it);
        if (nextIt == freeList.end()) break;

        if (it->startAddr + it->size == nextIt->startAddr) {
            it->size += nextIt->size;
            freeList.erase(nextIt);
        } else {
            ++it;
        }
    }
}

const std::list<FreeBlock>& MemoryManager::getFreeList() const { return freeList; }
const std::list<AllocBlock>& MemoryManager::getAllocList() const { return allocList; }
int MemoryManager::getTotalMemory() const { return totalMemory; }