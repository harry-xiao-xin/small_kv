//
// Created by zpx on 2024/07/04.
//

#ifndef SMALL_KV_CACHE_POLICY_H
#define SMALL_KV_CACHE_POLICY_H

#include <functional>
#include <mutex>
#include <vector>

namespace small_kv {
    template<typename K, typename V>
    class Node {
    public:
        Node() = default;

        Node(const K &key, V *val) : key_(key), val_(val) {}

        K key_;
        V *val_;
        int32_t ref_count_ = 0;      // 引用计数
        uint32_t hash_val_ = 0;       //
        bool in_cache_ = false;      // 是否在内存中
        uint64_t last_access_time_;   // 最后修改时间
    };

    template<typename K, typename V>
    class CachePolicy {
    public:
        CachePolicy() = default;
        virtual ~CachePolicy() = default;
        /**
         * insert operation
         * @param key
         * @param value
         */
        virtual void insert(const K &key, V *value) = 0;
        /**
         * delete operation
         * @param key
         */
        virtual void erase(const K &key) = 0;
        /**
         * query operation
         * @param key
         * @return
         */
        virtual Node<K, V> *get(const K &key) = 0;
        /**
         * check contains operation
         * @param key
         * @return
         */
        virtual bool contains(const K &key) = 0;
        /**
         * reference count decrease
         * @param key
         */
        virtual void release(const K &key) = 0;
        /**
         * destroy handler callback function
         */
        virtual void register_clean_handler(std::function<void(const K &k, V *val)> destructor) = 0;
    };
}
#endif //SMALL_KV_CACHE_POLICY_H
