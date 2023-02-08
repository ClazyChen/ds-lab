#include <vector>
#include <format>
#include <ranges>
#include <iostream>
import Framework;
import Vector;
using namespace std;

using dslab::Algorithm;
using dslab::TestItem;

// 这个实验用于展示和验证向量的基本操作
// 您可以将下面的向量替换为自己实现的向量参与测试
template <typename T>
using Vector = dslab::DefaultVector<T>;

// 测试流程：
// 1. 创建一个空向量，要求规模=0，容量=0
// 2. 修改容量为N，要求规模=0，容量=N
// 3. 在尾部插入N个元素（复制），要求不触发移动，规模=N，容量不变
// 4. 在随机位置插入N个元素（移动），要求不触发复制，规模=2N
// 5. 在随机位置删除N个元素，要求不触发复制，规模=N
// 6. 查找随机的N个元素，要求不触发移动或复制，规模、容量不变
// 7. 调用复制构造函数，要求不触发移动，前后相同
// 8. 调用移动构造函数，要求不触发复制或移动，前后相同
// 9. 修改规模逐次减小，要求不触发移动或复制，规模逐次减小

constexpr size_t N { 5 };

// 使用std::vector和我们自己实现的向量进行对拍
vector<size_t> stdVector {};

void check(Vector<TestItem>& V) {
    if (V.size() != stdVector.size()) {
        throw runtime_error("size not match");
    }
    auto it = begin(V);
    for (auto& x : stdVector) {
        if (it->m_value != x) {
            throw runtime_error("value not match");
        }
        ++it;
    }
}

class CreateVector : public Algorithm<void(Vector<TestItem>&)> {
public:
    void operator()(Vector<TestItem>& V) {
        V = {};
        stdVector = {};
        check(V);
        if (V.size() != 0) {
            throw runtime_error("size != 0");
        }
        if (V.capacity() != 0) {
            throw runtime_error("capacity != 0");
        }
    }
};

class ReserveVector : public Algorithm<void(Vector<TestItem>&)> {
public:
    void operator()(Vector<TestItem>& V) {
        V.reserve(N);
        if (V.size() != 0) {
            throw runtime_error("size != 0");
        }
        if (V.capacity() != N) {
            throw runtime_error("capacity != N");
        }
    }
};

class PushBackVector : public Algorithm<void(Vector<TestItem>&)> {
public:
    void operator()(Vector<TestItem>& V) {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        TestItem tmp { 0 };
        for (size_t i { 0 }; i < N; ++i) {
            tmp.m_value = i;
            V.push_back(tmp);
            stdVector.push_back(i);
            check(V);
            cout << format("push back {} -> ", i) << V << endl;
        }
        if (TestItem::s_moveCount != 0) {
            throw runtime_error("moveCount != 0");
        }
        if (V.size() != N) {
            throw runtime_error("size != N");
        }
        if (V.capacity() != N) {
            throw runtime_error("capacity != N");
        }
    }
};

class InsertVector : public Algorithm<void(Vector<TestItem>&)> {
public:
    void operator()(Vector<TestItem>& V) {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        for (size_t i { 0 }; i < N; ++i) {
            auto pos { rand() % (V.size() + 1) };
            V.insert(pos, TestItem { i + N });
            stdVector.insert(stdVector.begin() + pos, i + N);
            check(V);
            cout << format("insert {} @ {:2} -> ", i + N, pos) << V << endl;
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (V.size() != 2 * N) {
            throw runtime_error("size != 2 * N");
        }
    }
};

class EraseVector : public Algorithm<void(Vector<TestItem>&)> {
public:
    void operator()(Vector<TestItem>& V) {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        for (size_t i { 0 }; i < N; ++i) {
            auto pos { rand() % V.size() };
            auto e { V.remove(pos) };
            stdVector.erase(stdVector.begin() + pos);
            check(V);
            cout << format("erase {} @ {:2} -> ", e.m_value, pos) << V << endl;
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (V.size() != N) {
            throw runtime_error("size != N");
        }
    }
};

class FindVector : public Algorithm<void(Vector<TestItem>&)> {
public:
    void operator()(Vector<TestItem>& V) {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        TestItem tmp { 0 };
        for (size_t i { 0 }; i < N; ++i) {
            tmp.m_value = rand() % (2 * N);
            auto pos { V.find(tmp) };
            auto stdPos { ranges::find(stdVector, tmp.m_value) - begin(stdVector) };
            if (pos != stdPos) {
                throw runtime_error("pos != stdPos");
            }
            cout << format("find {} @ {:2} -> ", tmp.m_value, pos) << V << endl;
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (TestItem::s_moveCount != 0) {
            throw runtime_error("moveCount != 0");
        }
        if (V.size() != N) {
            throw runtime_error("size != N");
        }
    }
};

class CopyVector : public Algorithm<void(Vector<TestItem>&)> {
public:
    void operator()(Vector<TestItem>& V) {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        auto V2 { V };
        cout << format("copy -> ") << V2 << endl;
        if (TestItem::s_moveCount != 0) {
            throw runtime_error("moveCount != 0");
        }
        if (V.size() != V2.size()) {
            throw runtime_error("size != size");
        }
        check(V2);
    }
};

class MoveVector : public Algorithm<void(Vector<TestItem>&)> {
public:
    void operator()(Vector<TestItem>& V) {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        auto V2 { move(V) };
        cout << format("move -> ") << V2 << endl;
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (TestItem::s_moveCount != 0) {
            throw runtime_error("moveCount != 0");
        }
        if (V.size() != 0) {
            throw runtime_error("size != 0");
        }
        check(V2);
        V = move(V2);
    }
};

class ResizeVector : public Algorithm<void(Vector<TestItem>&)> {
public:
    void operator()(Vector<TestItem>& V) {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        for (size_t i { N }; i > 0; --i) {
            V.resize(i);
            stdVector.resize(i);
            check(V);
            cout << format("resize {} -> ", i) << V << endl;
            if (V.size() != i) {
                throw runtime_error("size != i");
            }
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (TestItem::s_moveCount != 0) {
            throw runtime_error("moveCount != 0");
        }
    }
};

dslab::StructureTestFramework<
    Vector, TestItem,
    CreateVector,
    ReserveVector,
    PushBackVector,
    InsertVector,
    EraseVector,
    FindVector,
    CopyVector,
    MoveVector,
    ResizeVector
> test;

int main() {
    srand(time(nullptr));
    Vector<TestItem> V {};
    test.initialize();
    test(V);
    return 0;
}
