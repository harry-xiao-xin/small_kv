//
// Created by zpx on 2024/07/06.
//
#include "memory/default_alloc.h"

namespace small_kv {
    void *DefaultAlloc::allocate(int32_t n) {
        return malloc(n);
    }

    void DefaultAlloc::deallocate(void *p, int32_t n) {
        free(p);
    }

    void *DefaultAlloc::reallocate(void *p, int32_t old_size, int32_t new_size) {
        return realloc(p, new_size);
    }
}
