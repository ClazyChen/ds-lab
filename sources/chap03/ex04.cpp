#include "list.h"
#include "vector.h"
#include "random.h"
#include <cassert>
using namespace clazy_framework;

// 这个例子展示了循环链表
// 我现在并没有理解循环链表有什么用

template <typename T>
using CircularForwardList = clazy::List<T, clazy::ForwardListNode<T>, true>;

template <typename T>
using CircularBidirectionalList = clazy::List<T, clazy::ListNode<T>, true>;

// 作为对照组，同样使用向量
template <typename T>
using Vector = clazy::Vector<T>;

// 随机操作的控制
const int start_size = 4;       // 向量的初始长度
const int op_count = 20;        // 连续操作序列的长度
const double insert_cdf = 0.75; // 在连续操作序列中“插入”的占比
const double front_cdf  = 0.6;  // 在连续操作序列中前端操作的占比

Random random;                  // 随机数发生器
CircularForwardList<int> Lf;    // 单向循环链表
CircularBidirectionalList<int> Lb; // 双向循环链表
const vector<clazy_framework::AbstractList<int>*> Ls {
    &Lf, &Lb
};                              // 用于测试的循环链表
Vector<int> V;                  // 用于对比的向量

int e = 0;                      // 用于向向量中加入元素

void check() {
    for (auto L : Ls) {
        assert(L->size() == V.size());
        auto itL = begin(*L);
        auto itV = begin(V);
        for (; itV != end(V); itV++, itL++) {
            assert(*itL == *itV);
        }
    }
}

void insert() {
    bool front = random.nextDouble() <= front_cdf;
    if (front) {
        cout << "push front(" << e << ")" << endl;
        for (auto L : Ls) {
            L->push_front(e);
        }
        V.push_front(e++);
    } else {
        cout << "push back(" << e << ")" << endl;
        for (auto L : Ls) {
            L->push_back(e);
        }
        V.push_back(e++);
    }
}

void remove() {
    bool front = random.nextDouble() <= front_cdf;
    if (front) {
        cout << "pop front(" << V[0] << ")" << endl;
        for (auto L : Ls) {
            L->pop_front();
        }
        V.pop_front();
    } else {
        cout << "pop back(" << V[V.size()-1] << ")" << endl;
        for (auto L : Ls) {
            L->pop_back();
        }
        V.pop_back();
    }
}

int main() {
    while (e < start_size) {
        insert(); // 实验开始，首先进行一些插入
    }
    check();
    for (int i : views::iota(0, op_count)) { // 实验中，进行若干次随机操作
        double x = random.nextDouble();
        if (x < insert_cdf || V.size() == 0) {
            insert();
        } else {
            remove();
        }
        cout << "\tLf = " << Lf << endl;
        cout << "\tLb = " << Lb << endl;
        cout << "\t V =  " << V << endl;
        check();
    }
    return 0;
}
