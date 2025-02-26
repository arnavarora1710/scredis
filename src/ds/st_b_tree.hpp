#ifndef ST_B_TREE_H_
#define ST_B_TREE_H_
#pragma GCC target("avx2")
#include <vector>

typedef __m256i reg;

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
        int cmp(reg x_vec, T* y_ptr);
};

#endif // ST_B_TREE_H_
