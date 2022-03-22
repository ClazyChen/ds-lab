#include "vector.h"
#include <cassert>
using namespace clazy_framework;

// 这个例子讨论：如何在向量中进行批量删除？
// 输入：向量V，条件P
// 输出：向量V中删除所有符合条件P的元素，并返回删除的元素数量

template <typename T>
using Vector = clazy::Vector<T>;

template <typename T>
class BatchRemove : public Algorithm {
protected:
    // 在删除的时候，同时进行计数
    int cnt = 0;
    virtual void remove(Vector<T>& V, Rank r) {
        V.remove(r);
        cnt++;
    }
    // 进行删除的函数，这里的删除总是使用BatchRemove::remove，而不使用裸的Vector::remove
    virtual void batchRemove(Vector<T>& V, const function<bool(const T&)>& P) = 0;
public:
    virtual int apply(Vector<T>& V, const function<bool(const T&)>& P) {
        cnt = 0;
        batchRemove(V, P);
        return cnt;
    }
};

// 最朴素的想法是：逐个查找、逐个删除
template <typename T>
class BatchRemoveSFSR : public BatchRemove<T> {
protected:
    virtual void batchRemove(Vector<T>& V, const function<bool(const T&)>& P) {
        Rank r = 0;
        while (r = find_if(begin(V), end(V), P) - begin(V), r < V.size()) { // 只要有满足条件的元素
            BatchRemove<T>::remove(V, r);  // 就把该元素删除（每轮循环找到一个、删除一个）
        }                                  // 直到没有满足条件的元素为止
    }
};

// 第二种想法：全局查找、逐个删除
template <typename T>
class BatchRemoveGFSR : public BatchRemove<T> {
protected:
    virtual void batchRemove(Vector<T>& V, const function<bool(const T&)>& P) {        
        Rank r = 0;
        while (r = find_if(begin(V) + r, end(V), P) - begin(V), r < V.size()) {
            BatchRemove<T>::remove(V, r);  // 改为从V[r]开始查找，因为之前的元素已经被找过一次了
        }                                  // 这样find_if加起来，总共也只遍历了V一次
    }
};

// 第三种想法：全局查找、全局删除
template <typename T>
class BatchRemoveGFGR : public BatchRemove<T> {
protected:
    virtual void batchRemove(Vector<T>& V, const function<bool(const T&)>& P) {
        Rank r_last = 0, r = 0; // 需要记录上次的r，每次移动一个区间
        int offset = -1;        // 向前移动的偏移量
        do {
            r = find_if(begin(V) + r, end(V), P) - begin(V);
            if (++offset > 0) { // 删除一个元素之后，下一个区间移动偏移量+1
				copy(begin(V) + r_last + 1, begin(V) + r, begin(V) + r_last + 1 - offset);
            }                   // 注意最后一段也需要移动，所以这里用了do-while循环，与前面不一样
            r_last = r++;       // 更新记录上次的r，并需要让r向前移动（因为V[r]并没有被实际删除）
        } while (r < V.size());
        this->cnt = offset;     // 因为是直接copy移动的，没有调用remove，所以直接更新cnt
        V.resize(V.size() - offset); // 直接修改V的规模，丢弃后面的元素。因为后面不满足P的元素已经被移动到前面去了
    }                           // find_if加起来只遍历了V一次，copy加起来也只遍历了V一次
};

// 第三种想法：全局查找、全局删除 的 另一种实现形式（快慢指针）
// 快指针查找，慢指针赋值，写法更加简洁
template <typename T>
class BatchRemoveGFGR_FSP : public BatchRemove<T> {
protected:
    virtual void batchRemove(Vector<T>& V, const function<bool(const T&)>& P) {
        auto it_assign = begin(V);  // 赋值指针（慢指针）
        for (auto it_find = begin(V); it_find != end(V); it_find++) { // 查找指针（快指针）
            if (!P(*it_find)) {     // 如果是不会被删除的元素
                *(it_assign++) = *it_find; // 则将其赋值到赋值指针的位置上
            }                       // 否则将其丢弃，赋值指针不移动
        }
        this->cnt = end(V) - it_assign; // 快慢指针的距离就是被删除的元素数量
        V.resize(it_assign - begin(V)); // 修改V的规模，丢弃后面的元素
    }
};

int main() {
    auto algorithms = generateInstances<BatchRemove<int>, 
        BatchRemoveSFSR<int>, 
        BatchRemoveGFSR<int>, 
        BatchRemoveGFGR<int>,
        BatchRemoveGFGR_FSP<int>>();
    int testData[] { 10, 100, 1000, 10000, 100'000 };   // 测试的向量规模
    auto is_even = [](const int& x) { return x % 2 == 0; }; // 测试用例：删除所有的偶数
    for (int n : testData) {
        cout << "Testing n = " << n << endl;
        for (auto algorithm : algorithms) {
            Vector<int> V(n);                           // 测试数据：[0,n)的整数序列
            for (int i : views::iota(0, n)) {           // 因为是动态操作，每次都需要重新生成序列
                V.push_back(i); 
            }
            applyTest<BatchRemove<int>>(algorithm, [&](auto remove) {
                cout << " removed = " << setw(9) << remove->apply(V, is_even) << "\t";
            });                                         // 删除所有的偶数
            assert(none_of(begin(V), end(V), is_even)); // 验证所有偶数都已经被删掉了
            assert(V.size() == n / 2);
        }
    }
}
