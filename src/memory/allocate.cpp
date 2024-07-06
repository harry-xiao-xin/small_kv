//
// Created by zpx on 2024/07/06.
//
#include <assert.h>
#include <glog/logging.h>
#include "memory/allocate.h"

namespace small_kv {
    FreeListAllocate::FreeListAllocate() {
        memory_slot_.fill(nullptr);
        mem_pool_start_ = (char *) malloc(CHUNK_SIZE);
        mem_pool_size_ = CHUNK_SIZE;
    }

    int FreeListAllocate::get_index(int32_t n) {
        return static_cast<int>(n + 7) / 8 - 1;
    }

    void *FreeListAllocate::allocate(int32_t n) {
        if (n <= 0) {
            LOG(ERROR) << "allocate size should large than zero: n=" << n << "\n";
            return nullptr;
        }
        if (n > MAX_OBJ_SIZE) {
            return malloc(n);
        }
        auto slot_index = get_index(n);
        if (memory_slot_[slot_index] == nullptr) {
            fill_slot(slot_index);
        }
        BlockNode *ret = memory_slot_[slot_index];
        memory_slot_[slot_index] = ret->next;
        return ret;
    }

    void FreeListAllocate::fill_slot(int32_t slot_index) {
        int32_t block_size = (slot_index + 1) * 8;
        int32_t needed_size = FILL_BLOCK_CNT * block_size;
        if (mem_pool_size_ >= needed_size) {
            for (int i = 0; i < FILL_BLOCK_CNT; ++i) {
                auto node = reinterpret_cast<BlockNode *>(mem_pool_size_ + i * block_size);
                if (i == 0) {
                    node->next = nullptr;
                }
                node->next = memory_slot_[slot_index];
                memory_slot_[slot_index] = node;
            }
        } else if (mem_pool_size_ >= block_size) {
            int32_t cnt = mem_pool_size_ / block_size;
            for (int i = 0; i < cnt; i++) {
                auto node = reinterpret_cast<BlockNode *>(mem_pool_start_ + i * block_size);
                if (i == 0) {
                    node->next = nullptr;
                }
                node->next = memory_slot_[slot_index];
                memory_slot_[slot_index] = node;
            }
            mem_pool_start_ += cnt * block_size;
            mem_pool_size_ -= cnt * block_size;
        } else { // 内存池大小连一个block都无法满足
            // 内存按照8字节对其，所以内存块的大小一定是8的整数倍
            assert(mem_pool_size_ % 8 == 0);
            // 将内存池的剩余部分挂载到slot上面，避免浪费
            if (mem_pool_size_ >= 8) {
                int32_t target_slot_index = get_index(mem_pool_size_);
                auto node = reinterpret_cast<BlockNode *>(mem_pool_start_);
                node->next = memory_slot_[target_slot_index];
                memory_slot_[target_slot_index]->next = node;
                mem_pool_start_ = nullptr;
                mem_pool_size_ = 0;
            }
            LOG(INFO) << "func fill_mem_pool is called.\n";
            // 重新申请一块内存池
            fill_mem_pool();
            // 重新执行本函数
            fill_slot(slot_index);
        }
    }

    void FreeListAllocate::fill_mem_pool() {
        mem_pool_start_ = (char *) malloc(CHUNK_SIZE);
        if (mem_pool_start_ == nullptr) {
            LOG(ERROR) << "Memory allocation failed.\n";
            return;
        }
        LOG(INFO) << CHUNK_SIZE / 1024 / 1024 << " MB memory is allocated once.\n";
        mem_pool_size_ = CHUNK_SIZE;
    }

    void FreeListAllocate::dealLocate(void *p, int32_t n) {
        if (p == nullptr || n == 0) {
            LOG(ERROR) << "p cannot be nullptr, n cannot be zero. p=" << p << ", n=" << n << ".\n";
            return;
        }
        // 大于MAX_OBJ_SIZE是malloc分配的，也由free回收
        if (n > MAX_OBJ_SIZE) {
            free(p);
            p = nullptr;
            return;
        }
        auto slot_index = get_index(n);
        auto node = static_cast<BlockNode *>(p);
        node->next = memory_slot_[slot_index];
        memory_slot_[slot_index] = node;
    }

    void *FreeListAllocate::reallocate(void *p, int32_t old_size, int32_t new_size) {
        dealLocate(p, old_size);
        return allocate(new_size);
    }
}