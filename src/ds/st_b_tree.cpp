#include "st_b_tree.hpp"
#include <limits>

#define TEMPLATE_T template <typename T>
typedef __m256i reg;

TEMPLATE_T
StaticTr<T>::StaticTr(int n) {
    this->n = n;
    this->blocks = (n + this->B - 1) / this->B;
    this->tr.resize(this->blocks);
}

TEMPLATE_T
StaticTr<T>::~StaticTr() { this->tr.clear(); }

TEMPLATE_T
int StaticTr<T>::go(int k, int i) { return k * (this->B + 1) + i + 1;  }

TEMPLATE_T
void StaticTr<T>::build(std::vector<T>& data, int k) {
    static int t = 0;
    if (k < this->blocks) {
       for (int i = 0; i < this->B; i++) {
           build(data, go(k, i));
           this->tr[k][i] = (t < n ? data[t++] : std::numeric_limits<T>::max());
       }
       build(data, go(k, this->B));
    }
}

TEMPLATE_T
int cmp(reg x_vec, T* y_ptr) {
    reg y_vec = _mm256_load_si256((reg*) y_ptr); // load 8 sorted elements
    reg mask = _mm256_cmpgt_epi32(x_vec, y_vec); // compare against the key
    return _mm256_movemask_ps((__m256) mask);    // extract the 8-bit mask
}

TEMPLATE_T
int StaticTr<T>::search(T _x) {
    int k = 0, res = std::numeric_limits<T>::max();
    reg x = _mm256_set1_epi32(_x);
    while (k < this->nblocks) {
        int mask = ~(
            cmp(x, &this->tr[k][0]) +
            (cmp(x, &this->tr[k][8]) << 8)
        );
        int i = __builtin_ffs(mask) - 1;
        if (i < B)
            res = this->tr[k][i];
        k = go(k, i);
    }
    return res;
}
