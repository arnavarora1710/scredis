#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../lib/doctest.h"
#include "../src/ds/st_b_tree.hpp"
#include <iostream>
#include <vector>
#include <limits>

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