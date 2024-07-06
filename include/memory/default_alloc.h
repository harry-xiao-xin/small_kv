//
// Created by zpx on 2024/07/06.
//

#ifndef SMALL_KV_DEFAULT_ALLOC_H
#define SMALL_KV_DEFAULT_ALLOC_H

#include <memory>
#include <cstdint>

namespace small_kv {
    class DefaultAlloc {
    public:
        DefaultAlloc() = default;
        ~DefaultAlloc() = default;
        void *allocate(int32_t n);
        void deallocate(void *p, int32_t n);
        void *reallocate(void *p, int32_t old_size, int32_t new_size);
    };
}
#endif //SMALL_KV_DEFAULT_ALLOC_H
