#ifndef ST_B_TREE_H_
#define ST_B_TREE_H_

#include <vector>

class StaticTr {
    public:
        StaticTr(int n);
        ~StaticTr();
        int go(int k, int i);
        void build();
        bool search(int k);
    private:
        const static int B = 16;
        std::vector< std::array<int, B> > tr;
};

#endif // ST_B_TREE_H_
