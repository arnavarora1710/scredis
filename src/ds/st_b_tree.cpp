#include "st_b_tree.hpp"
#include <limits>
#include <iostream>

#define TEMPLATE_T template <typename T>

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
int StaticTr<T>::cmp(neon_reg x_vec, T* y_ptr) {
    // Load 8 elements at a time
    int32x4x2_t y = vld1q_s32_x2((int32_t*)&y_ptr[0]);
    int32x4_t y_low = y.val[0];
    int32x4_t y_high = y.val[1];
    
    // Compare against the key (> comparison)
    uint32x4_t mask_low = vcgtq_s32(x_vec.low, y_low);
    uint32x4_t mask_high = vcgtq_s32(x_vec.high, y_high);
    
    // Extract the comparison results to create an 8-bit mask
    int mask = 0;
    // Extract each bit and combine into a mask
    mask |= (vgetq_lane_u32(mask_low, 0) & 1);
    mask |= ((vgetq_lane_u32(mask_low, 1) & 1) << 1);
    mask |= ((vgetq_lane_u32(mask_low, 2) & 1) << 2);
    mask |= ((vgetq_lane_u32(mask_low, 3) & 1) << 3);
    mask |= ((vgetq_lane_u32(mask_high, 0) & 1) << 4);
    mask |= ((vgetq_lane_u32(mask_high, 1) & 1) << 5);
    mask |= ((vgetq_lane_u32(mask_high, 2) & 1) << 6);
    mask |= ((vgetq_lane_u32(mask_high, 3) & 1) << 7);
    
    return mask;
}

TEMPLATE_T
int StaticTr<T>::search(T _x) {
    int k = 0, res = std::numeric_limits<T>::max();
    // Create vector with all elements set to _x
    neon_reg x;
    x.low = vdupq_n_s32(_x);
    x.high = vdupq_n_s32(_x);
    
    while (k < this->blocks) {
        // First 8 elements
        int mask1 = cmp(x, &this->tr[k][0]);
        // Next 8 elements
        int mask2 = cmp(x, &this->tr[k][8]);
        
        // Match original logic: combine using addition, not OR
        int mask = ~(mask1 + (mask2 << 8));
        int i = __builtin_ffs(mask) - 1;
        if (i < B)
            res = this->tr[k][i];
        k = go(k, i);
    }
    return res;
}

int main() {
    StaticTr<int> st(10000000);
    std::vector<int> data(10000000);
    for (int i = 0; i < 10000000; i++) data[i] = i;
    st.build(data);
    for (int i = 0; i < 10000000; i++) {
        // std::cout << st.search(i) << " ";
        // st.search(i);
        std::lower_bound(data.begin(), data.end(), i);
    }
    std::cout << std::endl;
}