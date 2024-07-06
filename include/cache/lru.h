// LRU Cache
// Created by zpx on 2024/07/04.
//

#ifndef SMALL_KV_LRU_H
#define SMALL_KV_LRU_H

#include <cassert>
#include <unordered_map>
#include <list>
#include <memory>
#include "cache_policy.h"
#include <mutex>

namespace small_kv {
    template<typename K, typename V>
    class LRU : public CachePolicy<K, V> {
    public:
        explicit LRU(uint32_t capacity) : capacity_(capacity) {}

        ~LRU() = default;
    public:
        /**
         * insert operation for lru
         * @param key
         * @param value
         */
        void insert(const K &key, V *value) {
            std::scoped_lock<std::mutex> locker(mutex_);
            auto *data = new Node<K, V>(key, value);
            data->in_cache_ = true;
            data->ref_count_ = 1;
            data->hash_val_ = hash_fn(key);
            data->last_access_time_ = time(nullptr);
            // if existing update the order of node, else insert new node
            if (index_.find(key) != index_.end()) {
                nodes_.splice(nodes_.begin(), nodes_, index_[key]);
                (*nodes_.begin())->val_ = value;
            } else {
                if (nodes_.size() >= capacity_) {
                    phantom_erase_node(nodes_.back());
                }
                nodes_.push_front(data);
            }
            index_[key] = nodes_.begin();
        }

        /**
         * delete operation
         * @param key
         */
        void erase(const K &key) {
            std::scoped_lock<std::mutex> locker(mutex_);
            auto target = index_.find(key);
            if (target != index_.end()) {
                phantom_erase_node(*(target->second));
            }
        }

        /**
         * query operation
         * @param key
         * @return
         */
        Node<K, V> *get(const K &key) {
            std::scoped_lock<std::mutex> locker(mutex_);
            auto target = index_.find(key);
            if (target == index_.end()) {
                return nullptr;
            }
            nodes_.push_front(*(target->second));
            nodes_.erase(target->second);
            index_[key] = nodes_.begin();
            ref(*(target->second));
            return *(target->second);
        }

        /**
         * check contains operation
         * @param key
         * @return
         */
        bool contains(const K &key) {
            std::scoped_lock<std::mutex> locker(mutex_);
            return index_.find(key) != index_.end();
        }

        /**
         * reference count decrease
         * @param key
         */
        void release(const K &key) {
            std::scoped_lock<std::mutex> locker(mutex_);
            unref(*(index_[key]));
        }

        /**
         * destroy handler callback function
         */
        virtual void register_clean_handler(std::function<void(const K &k, V *val)> destructor) {
            this->destructor_ = destructor;
        }

    private:
        void phantom_erase_node(Node<K, V> *node) {
            if (node) {
                node->in_cache_ = false;
                nodes_.erase(index_[node->key_]);
                index_.erase(node->key_);
                garbage_station_[node->key_] = node;
                unref(node);
            }
        }

        void ref(Node<K, V> *node) {
            if (node) {
                ++node->ref_count_;
            }
        }

        void unref(Node<K, V> *node) {
            if (node) {
                --node->ref_count_;
                if (node->ref_count_ <= 0) {
                    destructor_(node->key_, node->val_);
                }
            }
        }

    public:
        const uint32_t capacity_ = 0;   // max capacity
        std::unordered_map<K, typename std::list<Node<K, V> *>::iterator> index_;
        std::list<Node<K, V> *> nodes_;
        std::unordered_map<K, Node<K, V> *> garbage_station_;
        std::function<void(const K &k, V *val)> destructor_;
        std::mutex mutex_;
        static constexpr std::hash<K> hash_fn{};
    };
}
#endif //SMALL_KV_LRU_H
