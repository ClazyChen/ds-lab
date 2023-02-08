﻿#include <vector>
#include <format>
#include <ranges>
#include <iostream>
import Framework;
import ForwardList;
using namespace std;

using dslab::Algorithm;
using dslab::TestItem;

// 这个实验和列表的情况几乎相同
// 区别在于，前插、删除会引发移动，相应的移动次数会发生变化

// 这个实验用于展示和验证单向列表的基本操作
// 您可以将下面的列表替换为自己实现的单向列表参与测试
template <typename T>
using ForwardList = dslab::ForwardList<T>;

template <typename T>
using ListPos = typename ForwardList<T>::position_type;

// 测试流程：
// 1. 创建一个空列表，要求规模=0
// 2. 在尾部插入N个元素（复制），只触发N次移动，规模=N
// 3. 在随机位置后插N个元素（移动），要求不触发复制，规模=2N
// 4. 在随机位置删除N个元素，要求不触发复制，只触发2N次移动，规模=N
// 5. 查找随机的N个元素，要求不触发移动或复制，规模=N
// 6. 在随机位置前插N个元素（复制），只触发N次移动，规模=2N
// 7. 调用复制构造函数，要求不触发移动，前后相同
// 8. 调用移动构造函数，要求不触发复制或移动，前后相同

constexpr size_t N { 5 };

// 使用std::vector和我们自己实现的列表进行对拍
vector<size_t> stdVector {};

void check(ForwardList<TestItem>& L) {
    if (L.size() != stdVector.size()) {
        throw runtime_error("size not match");
    }
    auto it = begin(L);
    for (auto& x : stdVector) {
        if (it->m_value != x) {
            throw runtime_error("value not match");
        }
        ++it;
    }
}

class CreateList : public Algorithm<void(ForwardList<TestItem>&)> {
public:
    void operator()(ForwardList<TestItem>& L) override {
        L = {};
        stdVector = {};
        check(L);
        if (L.size() != 0) {
            throw runtime_error("size != 0");
        }
    }
};

class PushBackList : public Algorithm<void(ForwardList<TestItem>&)> {
public:
    void operator()(ForwardList<TestItem>& L) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        TestItem tmp { 0 };
        for (size_t i { 0 }; i < N; ++i) {
            tmp.m_value = i;
            L.push_back(tmp);
            stdVector.push_back(i);
            cout << format("push back {} -> ", i) << L << endl;
            check(L);
        }
        if (TestItem::s_moveCount != N) {
            throw runtime_error("moveCount != N");
        }
        if (L.size() != N) {
            throw runtime_error("size != N");
        }
    }
};

class InsertNextList : public Algorithm<void(ForwardList<TestItem>&)> {
public:
    void operator()(ForwardList<TestItem>& L) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        for (size_t i { 0 }; i < N; ++i) {
            auto pos { rand() % (L.size() + 1) };
            auto p { L.head() };
            for (size_t j { 0 }; j < pos; ++j) {
                p = L.next(p);
            }
            L.insertAsNext(p, TestItem { i + N });
            stdVector.insert(begin(stdVector) + pos, i + N);
            cout << format("insert next {} @ {:2} -> ", i + N, L.get(p).m_value) << L << endl;
            check(L);
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (L.size() != 2 * N) {
            throw runtime_error("size != 2 * N");
        }
    }
};

class EraseList : public Algorithm<void(ForwardList<TestItem>&)> {
public:
    void operator()(ForwardList<TestItem>& L) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        for (size_t i { 0 }; i < N; ++i) {
            auto pos { rand() % L.size() };
            auto p { L.head() };
            for (size_t j { 0 }; j <= pos; ++j) {
                p = L.next(p);
            }
            auto e { L.remove(p) };
            stdVector.erase(begin(stdVector) + pos);
            cout << format("erase {} -> ", e.m_value) << L << endl;
            check(L);
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (TestItem::s_moveCount != 2 * N) {
            throw runtime_error("moveCount != 2 * N");
        }
        if (L.size() != N) {
            throw runtime_error("size != N");
        }
    }
};

class FindList : public Algorithm<void(ForwardList<TestItem>&)> {
public:
    void operator()(ForwardList<TestItem>& L) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        TestItem tmp { 0 };
        for (size_t i { 0 }; i < N; ++i) {
            tmp.m_value = rand() % (2 * N);
            auto pos { L.find(tmp) };
            auto stdPos { ranges::find(stdVector, tmp.m_value) };
            if (stdPos == end(stdVector)) {
                if (pos != L.tail()) {
                    throw runtime_error("find failed");
                }
            } else {
                if (L.get(pos).m_value != tmp.m_value) {
                    throw runtime_error("find failed");
                }
            }
            cout << format("find {} ({}) -> ", tmp.m_value, pos != L.tail()) << L << endl;
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (TestItem::s_moveCount != 0) {
            throw runtime_error("moveCount != 0");
        }
        if (L.size() != N) {
            throw runtime_error("size != N");
        }
    }
};

class InsertPrevList : public Algorithm<void(ForwardList<TestItem>&)> {
public:
    void operator()(ForwardList<TestItem>& L) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        TestItem tmp { 0 };
        for (size_t i { 0 }; i < N; ++i) {
            auto pos { rand() % (L.size() + 1) };
            auto p { L.head() };
            for (size_t j { 0 }; j <= pos; ++j) {
                p = L.next(p);
            }
            tmp.m_value = i + 2 * N;
            L.insertAsPrev(p, tmp);
            stdVector.insert(begin(stdVector) + pos, i + 2 * N);
            cout << format("insert prev {} @ {:2} -> ", i + 2 * N, L.get(L.next(p)).m_value) << L << endl;
            check(L);
        }
        if (TestItem::s_moveCount != N) {
            throw runtime_error("moveCount != N");
        }
        if (L.size() != 2 * N) {
            throw runtime_error("size != 2 * N");
        }
    }
};

class CopyList : public Algorithm<void(ForwardList<TestItem>&)> {
public:
    void operator()(ForwardList<TestItem>& L) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        auto L2 { L };
        cout << format("copy -> ") << L2 << endl;
        if (TestItem::s_copyCount != L.size()) {
            throw runtime_error("copyCount != size");
        }
        if (TestItem::s_moveCount != 0) {
            throw runtime_error("moveCount != 0");
        }
        if (L2.size() != L.size()) {
            throw runtime_error("size != size");
        }
        if (L2.head() == L.head()) {
            throw runtime_error("head == head");
        }
        if (L2.tail() == L.tail()) {
            throw runtime_error("tail == tail");
        }
        check(L2);
    }
};

class MoveList : public Algorithm<void(ForwardList<TestItem>&)> {
public:
    void operator()(ForwardList<TestItem>& L) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        auto L2 { move(L) };
        cout << format("move -> ") << L2 << endl;
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (TestItem::s_moveCount != 0) {
            throw runtime_error("moveCount != 0");
        }
        if (L.size() != 0) {
            throw runtime_error("size != 0");
        }
        check(L2);
    }
};

dslab::StructureTestFramework<
    ForwardList, TestItem,
    CreateList,
    PushBackList,
    InsertNextList,
    EraseList,
    FindList,
    InsertPrevList,
    CopyList,
    MoveList
> test;

int main() {
    srand(time(nullptr));
    ForwardList<TestItem> L {};
    test.initialize();
    test(L);
    return 0;
}