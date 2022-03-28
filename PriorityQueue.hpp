#ifndef _PRIORITYQUEUE
#define _PRIORITYQUEUE

#include <map>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>

// Returns an iterator to (key, val) in the multimap or else return map.end()
// Worst-case run time: O(n) when every key are the same
template <class T, class U>
auto mm_find(const std::multimap<T,U>& map, const T& key, const U& val) {
    // Find the key
    auto bounds = map.equal_range(key);
    // Find the entry that has the value
    std::pair<const T,U> kv(key, val);
    auto it = std::find(bounds.first, bounds.second, kv);
    // Either return the found iterator or end()
    if (it == bounds.second) {
        return map.end();
    } else {
        return it;
    }
}

// min priority queue
template <class Key, class Priority>
class PriorityQueue {
public:
    // Inserts a key and its priority into the priority queue
    void insert(const Key& k, const Priority& p) {
        if (contains(k)) {
            throw std::runtime_error("Key already exists.");
        }
        priority.insert({k,p});
        map.insert({p,k});
    }
    // Removes object from priority queue
    void remove(const Key& k) {
        auto it1 = priority.find(k);
        if (it1 == priority.end()) {
            // Couldn't find key.
            throw std::out_of_range("Couldn't find key.");
        }
        Priority pri = it1->second;
        priority.erase(it1);
        // Look up the (priority, key)
        auto it = mm_find(map, pri, k);
        map.erase(it);
    }
    // Returns the priority of a given object or error if not found.
    Priority operator[](const Key& k) {
        return priority.at(k);
    }
    // Whether priority queue contains key k
    bool contains(const Key& k) {
        return (priority.count(k) > 0);
    }
    // Updates key k to have a new priority p
    void update(const Key& k, const Priority& p) {
        remove(k);
        insert(k, p);
    }
    bool empty() {
        return priority.empty();
    }
    size_t size() {
        return priority.size();
    }
    void clear() {
        priority.clear();
        map.clear();
    }
    // Iterators
    auto begin() {
        return map.begin();
    }
    auto end() {
        return map.end();
    }
    // Frontmost element
    std::pair<Key,Priority> front() {
        auto mt = map.begin();
        return {mt->second, mt->first};
    }
    // Remove frontmost element and return it
    std::pair<Key,Priority> pop() {
        auto out = front();
        remove(out.first);
        return out;
    }
    // Returns true iff the priority queue's keys and priorities
    // are in the same order.
    bool check_contents(
    const std::vector<Key>& keys,
    const std::vector<Priority>& priorities) {
        // Check the size
        if (map.size() != keys.size()) {
            return false;
        }
        // Step through the entries in order
        int i = 0;
        for (const auto& pk : map) {
            if (pk.first != priorities[i]) {
                return false;
            }
            if (pk.second != keys[i]) {
                return false;
            }
            i++;
        }
        return true;
    }
private:
    std::unordered_map<Key,Priority> priority;
    std::multimap<Priority,Key> map;
};

#endif