#pragma once
#include <vector>
#include <string>
#include <functional>

template<typename K, typename V>
class SimpleMap {
private:
    struct Entry {
        K key;
        V value;
        Entry* next;
        Entry(K k, V v) : key(k), value(v), next(nullptr) {}
    };
    std::vector<Entry*> buckets;
    size_t bucketCount;

    size_t hashKey(const K& key) const {
        std::hash<K> hasher;
        return hasher(key) % bucketCount;
    }

public:
    SimpleMap(size_t n = 1024) {
        bucketCount = n;
        buckets.resize(bucketCount, nullptr);
    }

    ~SimpleMap() {
        for (auto head : buckets) {
            while (head) {
                Entry* tmp = head;
                head = head->next;
                delete tmp;
            }
        }
    }

    void put(const K& key, const V& value) {
        size_t idx = hashKey(key);
        Entry* curr = buckets[idx];
        while (curr) {
            if (curr->key == key) {
                curr->value = value;
                return;
            }
            curr = curr->next;
        }
        Entry* newEntry = new Entry(key, value);
        newEntry->next = buckets[idx];
        buckets[idx] = newEntry;
    }

    bool contains(const K& key) const {
        size_t idx = hashKey(key);
        Entry* curr = buckets[idx];
        while (curr) {
            if (curr->key == key) return true;
            curr = curr->next;
        }
        return false;
    }

    V& operator[](const K& key) {
        size_t idx = hashKey(key);
        Entry* curr = buckets[idx];
        while (curr) {
            if (curr->key == key) return curr->value;
            curr = curr->next;
        }
        Entry* newEntry = new Entry(key, V());
        newEntry->next = buckets[idx];
        buckets[idx] = newEntry;
        return buckets[idx]->value;
    }
    std::vector<K> getKeys() const {
        std::vector<K> keys;
        for (auto head : buckets) {
            while (head) {
                keys.push_back(head->key);
                head = head->next;
            }
        }
        return keys;
    }
};



