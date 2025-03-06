#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../lib/doctest.h"

#include "../src/ds/st_b_tree.hpp"
#include <iostream>

TEST_CASE("StaticTr initialization") {
    StaticTr tree(32);
    CHECK(tree.search(0) == 0);
}

TEST_CASE("StaticTr build and search") {
    StaticTr tree(32);
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
    tree.build(data);

    SUBCASE("Search for existing elements") {
        for (int i = 1; i <= 32; ++i) {
            CHECK(tree.search(i) == i);
        }
    }

    SUBCASE("Search for non-existing elements") {
        CHECK(tree.search(33) == std::numeric_limits<int>::max());
        CHECK(tree.search(0) == std::numeric_limits<int>::max());
    }
}

TEST_CASE("StaticTr edge cases") {
    StaticTr tree_single(1);
    std::vector<int> data_single = {42};
    tree_single.build(data_single);
    tree_single.print();
    CHECK(tree_single.search(42) == 42);
    CHECK(tree_single.search(0) == std::numeric_limits<int>::max());
}
