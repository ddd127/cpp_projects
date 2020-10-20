#include <iostream>

#include "hash_set.h"
#include "randomized_queue.h"

void set_example() {
    std::cout << "set example:" << std::endl << std::endl;
    HashSet<int> set;
    int n = 5;

    for (int i = 0; i < n; ++i) {
        set.insert(2 * i);
        std::cout << "add " << (2 * i) << std::endl;
    }
    std::cout << std::endl;

    for (int i = 0; i < 2 * n; ++i) {
        if (set.contains(i)) {
            std::cout << "set contains " << i << std::endl;
        } else {
            std::cout << "set doesn't contain " << i << std::endl;
        }
    }
    std::cout << std::endl;

    std::cout << "set example end" << std::endl;
}

void rand_queue_example() {
    std::cout << "rand queue example:" << std::endl << std::endl;
    randomized_queue<int> rand_q;
    int n = 5;

    for (int i = 0; i < n; ++i) {
        rand_q.enqueue(2 * i);
        std::cout << "enqueue " << (2 * i) << std::endl;
    }
    std::cout << std::endl;

    std::cout << "first iterator runs: " << std::endl;
    for (const auto& item : rand_q) {
        std::cout << item << " ";
    }
    std::cout << std::endl << std::endl;

    std::cout << "second iterator runs: " << std::endl;
    for (const auto& item : rand_q) {
        std::cout << item << " ";
    }
    std::cout << std::endl << std::endl;

    std::cout << "rand queue example end" << std::endl;
}

int main() {
    set_example();
    std::cout << std::endl << std::endl;
    rand_queue_example();

    return 0;
}
