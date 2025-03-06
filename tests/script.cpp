#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../lib/doctest.h"

#include "../src/ds/st_b_tree.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <limits>
#include <iomanip>

TEST_CASE("StaticTr basic lower bound functionality") {
    StaticTr tree(5);
    std::vector<int> data = {10, 20, 30, 40, 50};
    tree.build(data);

    SUBCASE("Exact matches") {
        CHECK(tree.search(10) == 10);
        CHECK(tree.search(20) == 20);
        CHECK(tree.search(30) == 30);
        CHECK(tree.search(40) == 40);
        CHECK(tree.search(50) == 50);
    }

    SUBCASE("Lower bound queries") {
        CHECK(tree.search(5) == 10);   // First element
        CHECK(tree.search(15) == 20);  // Between elements
        CHECK(tree.search(25) == 30);  // Between elements
        CHECK(tree.search(45) == 50);  // Between elements
        CHECK(tree.search(55) == std::numeric_limits<int>::max());  // Beyond last element
    }
}

TEST_CASE("StaticTr edge cases") {
    SUBCASE("Single element tree") {
        StaticTr tree_single(1);
        std::vector<int> data = {42};
        tree_single.build(data);
        
        CHECK(tree_single.search(41) == 42);
        CHECK(tree_single.search(42) == 42);
        CHECK(tree_single.search(43) == std::numeric_limits<int>::max());
    }

    SUBCASE("Large gaps between elements") {
        StaticTr tree(3);
        std::vector<int> data = {1, 1000, 1000000};
        tree.build(data);
        
        CHECK(tree.search(0) == 1);
        CHECK(tree.search(1) == 1);
        CHECK(tree.search(2) == 1000);
        CHECK(tree.search(999) == 1000);
        CHECK(tree.search(1001) == 1000000);
    }
}

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

TEST_CASE("StaticTr performance benchmarking") {
    const std::vector<int> sizes = {1000, 10000, 100000, 1000000, 10000000};
    
    for (int size : sizes) {
        SUBCASE(("Performance test with size " + std::to_string(size)).c_str()) {
            // Generate sorted data
            std::vector<int> data(size);
            for (int i = 0; i < size; ++i) {
                data[i] = i * 2;  // Even numbers to leave gaps
            }
            
            StaticTr tree(size);
            tree.build(data);
            
            // Prepare queries
            const int num_queries = 10000;
            std::vector<int> queries(num_queries);
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, size * 2);
            
            for (int i = 0; i < num_queries; ++i) {
                queries[i] = dis(gen);
            }
            
            // Benchmark StaticTr search
            auto start_tree = std::chrono::high_resolution_clock::now();
            for (int query : queries) {
                tree.search(query);
            }
            auto end_tree = std::chrono::high_resolution_clock::now();
            
            // Benchmark std::lower_bound
            auto start_std = std::chrono::high_resolution_clock::now();
            for (int query : queries) {
                auto it = std::lower_bound(data.begin(), data.end(), query);
                if (it == data.end()) {
                    volatile int dummy = std::numeric_limits<int>::max();  // Ensure fair comparison with similar work
                } else {
                    volatile int dummy = *it;  // Ensure fair comparison with similar work
                }
            }
            auto end_std = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_std);
            
            auto tree_time = std::chrono::duration_cast<std::chrono::microseconds>(end_tree - start_tree).count();
            auto std_time = end_std.count();
            
            double tree_avg = static_cast<double>(tree_time) / num_queries;
            double std_avg = static_cast<double>(std_time) / num_queries;
            double speedup = std_avg / tree_avg;
            
            std::stringstream ss;
            ss << std::fixed << std::setprecision(3);
            ss << size << "\t\t" << tree_avg << "\t\t" << std_avg << "\t\t" << speedup << "x";
            MESSAGE(ss.str());
            
            // Verify results are still correct
            for (int i = 0; i < std::min(100, num_queries); ++i) {  // Check first 100 queries
                int query = queries[i];
                int tree_result = tree.search(query);
                auto std_it = std::lower_bound(data.begin(), data.end(), query);
                int std_result = (std_it == data.end()) ? std::numeric_limits<int>::max() : *std_it;
                CHECK(tree_result == std_result);
            }
        }
    }
}
