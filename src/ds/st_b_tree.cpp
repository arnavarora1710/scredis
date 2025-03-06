#include "st_b_tree.hpp"

StaticTr::StaticTr(int n) {
    this->n = n;
    this->blocks = (n + this->B - 1) / this->B;
    this->tr.resize(this->blocks);
}

StaticTr::~StaticTr() { 
    this->tr.clear(); 
}

int StaticTr::go(int k, int i) { 
    return k * (this->B + 1) + i + 1;  
}

void StaticTr::build(std::vector<int>& data, int k) {
    static int t = 0;
    if (k < this->blocks) {
        for (int i = 0; i < this->B; i++) {
            build(data, go(k, i));
            this->tr[k][i] = (t < this->n ? data[t++] : std::numeric_limits<int>::max());
        }
        build(data, go(k, this->B));
    }
}

void StaticTr::print() {
    std::cout << "Tree parameters:" << std::endl;
    std::cout << "n: " << this->n << std::endl;
    std::cout << "B: " << this->B << std::endl;
    std::cout << "blocks: " << this->blocks << std::endl;
    for (int k = 0; k < this->blocks; ++k) {
        std::cout << "Block " << k << ": ";
        for (int i = 0; i < this->B; ++i) {
            if (this->tr[k][i] == std::numeric_limits<int>::max()) {
                std::cout << "inf ";
            } else {
                std::cout << this->tr[k][i] << " ";
            }
        }
        std::cout << std::endl;
    }
}

int StaticTr::cmp(neon_reg x_vec, int* y_ptr) {
    int32x4x2_t y = vld1q_s32_x2((int32_t*)&y_ptr[0]);
    int32x4_t y_low = y.val[0];
    int32x4_t y_high = y.val[1];
    
    uint32x4_t mask_low = vcgtq_s32(x_vec.low, y_low);
    uint32x4_t mask_high = vcgtq_s32(x_vec.high, y_high);
    
    int mask = 0;
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

int StaticTr::search(int _x) {
    int k = 0, res = std::numeric_limits<int>::max();
    neon_reg x;
    x.low = vdupq_n_s32(_x);
    x.high = vdupq_n_s32(_x);
    
    while (k < this->blocks) {
        int mask1 = cmp(x, &this->tr[k][0]);
        int mask2 = cmp(x, &this->tr[k][8]);
        int mask = ~(mask1 + (mask2 << 8));
        int i = __builtin_ffs(mask) - 1;
        if (i < B)
            res = this->tr[k][i];
        k = go(k, i);
    }
    return res;
} 