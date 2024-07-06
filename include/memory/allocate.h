//
// Created by zpx on 2024/07/06.
//

#ifndef SMALL_KV_ALLOCATE_H
#define SMALL_KV_ALLOCATE_H

#include <cstdint>
#include <vector>
#include <array>
#include <mutex>

namespace small_kv {
    union BlockNode {
        BlockNode *next;
        void *block_addr;
    };

    class FreeListAllocate final {
    public:
        FreeListAllocate();
        ~FreeListAllocate() = default;
        /**
         * allocate n bytes memory
         * @param n
         * @return
         */
        void *allocate(int32_t n);
        /**
         * delete memory of pointer
         * @param p
         * @param n
         */
        void dealLocate(void *p, int32_t n);
        /**
         * reallocate new size bytes memory
         * @param p
         * @param old_size
         * @param new_size
         */
        void *reallocate(void *p, int32_t old_size, int32_t new_size);
    private:
        void fill_slot(int32_t slot_index);
        void fill_mem_pool();
        static inline int get_index(int32_t n);
    private:
        constexpr static int32_t SLOT_NUM = 512;
        constexpr static int32_t ALIGN_BYTES = 8;
        constexpr static int32_t MAX_OBJ_SIZE = 4 * 1024; // 设置最大的小对象为4K，超过4K就直接调用系统的malloc
        constexpr static int32_t CHUNK_SIZE = 4 * 1024 * 1024; // 每次扩展4MB的空闲内存池
        constexpr static int32_t FILL_BLOCK_CNT = 10; // 每次向内存槽填充的block数量，默认为10个
    private:
        std::array<BlockNode *, SLOT_NUM> memory_slot_;
        char *mem_pool_start_ = nullptr;
        int32_t mem_pool_size_ = 0;
        std::mutex mutex_;
    };
}
#endif //SMALL_KV_ALLOCATE_H
