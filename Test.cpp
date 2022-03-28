#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <cassert>

#include "PriorityQueue.hpp"

int main() {
    std::ifstream f("test-cases.txt");
    if (!f.is_open()) {
        throw std::runtime_error("Need test cases file.");
    }
    // The class we're testing
    PriorityQueue<int,int> pq;

    std::string action, exception_flag;
    bool should_throw;
    int key;
    int priority;
    int read_value;
    int pq_length;
    std::vector<int> keys;
    std::vector<int> priorities;
    int line_num = 0;

    bool skip_next_part = false;
    while (!f.eof()) {
        // Read the action we're about to perform
        f >> action >> key >> priority >> exception_flag;
        if (exception_flag == "T") {
            should_throw = true;
        } else {
            should_throw = false;
        }
        // See if we need additional data
        if (action == "READ") {
            f >> read_value;
        } else {
            f >> pq_length;
            keys.clear();
            priorities.clear();
            // Read the keys
            for (int i = 0; i < pq_length; ++i) {
                int k;
                f >> k;
                keys.push_back(k);
            }
            // Read the values
            for (int i = 0; i < pq_length; ++i) {
                int p;
                f >> p;
                priorities.push_back(p);
            }
        }
        // Try the specified action
        try {
            if (action == "NEW") {
                pq.clear();
            } else if (action == "INSERT") {
                pq.insert(key, priority);
            } else if (action == "ERASE") {
                pq.remove(key);
            } else if (action == "READ") {
                assert (pq[key] == read_value);
            } else if (action == "UPDATE") {
                pq.update(key, priority);
            }
        } catch (...) {
            if (!should_throw) {
                throw std::runtime_error(
                    "Threw an error when none was expected.");
            }
            skip_next_part = true;
        }
        if (!skip_next_part && should_throw) {
            throw std::runtime_error(
                "Error not caught.");
        }
        // Make sure the contents are as expected
        if (skip_next_part && action != "READ") {
            if (!pq.check_contents(keys, priorities)) {
                throw "Contents don't match.";
            }
        }
        skip_next_part = true;
        line_num++;
    }
    f.close();
    return 0;
}