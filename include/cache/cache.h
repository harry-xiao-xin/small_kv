//
// Created by zpx on 2024/07/06.
//

#ifndef SMALL_KV_CACHE_H
#define SMALL_KV_CACHE_H

#include "cache_policy.h"
#include "lru.h"

namespace small_kv {
    template<typename K, typename V>
    class Cache {
    public:
        explicit Cache(uint32_t capacity) {
            caches_.resize(SHARDING_NUM);
            for (auto &item: caches_) {
                item = std::make_shared<LRU<K, V> >(capacity);
            }
        }

        ~Cache() = default;
    public:
        /**
         * insert node
         * @param key
         * @param val
         */
        void insert(const K &key, V *val) {
            uint64_t sharding_index = hash_fn(key) % SHARDING_NUM;
            caches_[sharding_index]->insert(key, val);
        }
        /**
         * erase by key
         * @param key
         */
        void erase(const K &key) {
            uint64_t sharding_index = hash_fn(key) % SHARDING_NUM;
            caches_[sharding_index]->erase(key);
        }
        /**
         * get node
         * @param key
         * @return
         */
        Node<K, V> *get(const K &key) {
            uint64_t sharding_index = hash_fn(key) % SHARDING_NUM;
            return caches_[sharding_index]->get(key);
        }
        /**
         * check whether contain node by key
         * @param key
         * @return
         */
        bool contains(const K &key) {
            uint64_t sharding_index = hash_fn(key) % SHARDING_NUM;
            return caches_[sharding_index]->contains(key);
        }
        /**
         * release the reference count
         * @param key
         */
        void release(const K &key) {
            uint64_t sharding_index = hash_fn(key) % SHARDING_NUM;
            caches_[sharding_index]->release(key);
        }

        /**
         * callback function
         * @param destructor
         */
        void register_clean_handle(std::function<void(const K &key, V *val)> destructor) {
            for (auto &item: caches_) {
                item->register_clean_handler(destructor);
            }
        }

    private:
        static constexpr uint64_t SHARDING_NUM = 5;
        std::vector<std::shared_ptr<CachePolicy<K, V>>> caches_; // hash slot
        static constexpr std::hash<K> hash_fn{};
    };
}
#endif //SMALL_KV_CACHE_H
