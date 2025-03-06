#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../lib/doctest.h"
#include "../src/ds/st_b_tree.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <limits>

TEST_CASE("StaticTr comprehensive correctness") {
    const int size = 1000;
    std::vector<int> data(size);
    
    // Generate sorted random data
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);
    
    for (int i = 0; i < size; ++i) {
        data[i] = dis(gen);
    }
    std::sort(data.begin(), data.end());
    
    StaticTr tree(size);
    tree.build(data);
    
    SUBCASE("Random queries") {
        for (int i = 0; i < 100; ++i) {
            int query = dis(gen);
            int expected = *std::lower_bound(data.begin(), data.end(), query);
            int result = tree.search(query);
            
            if (std::lower_bound(data.begin(), data.end(), query) == data.end()) {
                CHECK(result == std::numeric_limits<int>::max());
            } else {
                CHECK(result == expected);
            }
        }
    }
} 