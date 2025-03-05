#ifndef ST_B_TREE_H_
#define ST_B_TREE_H_
#include <arm_neon.h>
#include <vector>
#include <array>

typedef struct {
    int32x4_t low;
    int32x4_t high;
} neon_reg;

template <typename T>
class StaticTr {
    public:
        StaticTr(int n);
        ~StaticTr();
        int go(int k, int i);
        void build(std::vector<T>& data, int k = 0);
        int search(T _x);
    private:
        const static int B = 16;
        std::vector< std::array<T, B> > tr;
        int n, blocks;
        int cmp(neon_reg x_vec, T* y_ptr);
};

#endif // ST_B_TREE_H_
