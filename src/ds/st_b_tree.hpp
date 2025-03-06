#ifndef ST_B_TREE_H_
#define ST_B_TREE_H_

#include <arm_neon.h>
#include <vector>
#include <array>
#include <limits>
#include <iostream>

typedef struct {
    int32x4_t low;
    int32x4_t high;
} neon_reg;

class StaticTr {
    public:
        StaticTr(int n);
        ~StaticTr();
        void build(std::vector<int>& data, int k = 0);
        void print();
        int search(int _x);

    private:
        const static int B = 16;
        std::vector< std::array<int, B> > tr;
        int n, blocks;
        
        int go(int k, int i);
        int cmp(neon_reg x_vec, int* y_ptr);
};

#endif // ST_B_TREE_H_