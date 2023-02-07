#include <vector>
#include <format>
#include <iostream>
import Framework;
import Vector;
using namespace std;

// 这个实验用于展示和验证向量的基本操作
// 您可以将下面的向量替换为自己实现的向量参与测试
template <typename T>
using Vector = dslab::DefaultVector<T>;

// 用于进行测试的元素类型
class TestItem {
public:
    size_t m_value { 0 };
    static size_t s_copyCount;
    static size_t s_moveCount;
    TestItem() = default;
    TestItem(size_t value) : m_value(value) {}
    TestItem(const TestItem& other) : m_value(other.m_value) {
        ++s_copyCount;
    }
    TestItem(TestItem&& other) : m_value(other.m_value) {
        ++s_moveCount;
    }
    TestItem& operator=(const TestItem& other) {
        m_value = other.m_value;
        ++s_copyCount;
        return *this;
    }
    TestItem& operator=(TestItem&& other) {
        m_value = other.m_value;
        ++s_moveCount;
        return *this;
    }
    bool operator==(const TestItem& other) {
        return m_value == other.m_value;
    }
    auto operator<=>(const TestItem& other) const = default;
};

ostream& operator<<(ostream& os, const TestItem& item) {
    return os << format("{}", item.m_value);
}

size_t TestItem::s_copyCount { 0 };
size_t TestItem::s_moveCount { 0 };

// 测试流程：
// 1. 创建一个空向量，要求规模=0，容量=0
// 2. 修改容量为N，要求规模=0，容量=N
// 3. 在尾部插入N个元素（复制），要求不触发移动，规模=N，容量不变
// 4. 在随机位置插入N个元素（移动），要求不触发复制，规模=2N
// 5. 在随机位置删除N个元素，要求不触发复制，规模=N
// 6. 查找随机的N个元素，要求不触发移动或复制，规模、容量不变
// 7. 调用复制构造，要求不触发移动，前后相同
// 8. 调用移动构造，要求不触发复制或移动，前后相同
// 9. 修改规模逐次减小，要求不触发移动或复制，规模逐次减小

constexpr size_t N { 5 };

class CreateVector : public dslab::Algorithm<void(Vector<TestItem>&)> {
public:
    void operator()(Vector<TestItem>& V) {
        V = {};
        if (V.size() != 0) {
            throw runtime_error("size != 0");
        }
        if (V.capacity() != 0) {
            throw runtime_error("capacity != 0");
        }
    }
};

class ReserveVector : public dslab::Algorithm<void(Vector<TestItem>&)> {
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

class PushBackVector : public dslab::Algorithm<void(Vector<TestItem>&)> {
public:
    void operator()(Vector<TestItem>& V) {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        TestItem tmp { 0 };
        for (size_t i { 0 }; i < N; ++i) {
            tmp.m_value = i;
            V.push_back(tmp);
            cout << format("push back {} -> ", tmp.m_value) << V << endl;
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

class InsertVector : public dslab::Algorithm<void(Vector<TestItem>&)> {
public:
    void operator()(Vector<TestItem>& V) {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        for (size_t i { 0 }; i < N; ++i) {
            auto pos { rand() % (V.size() + 1) };
            V.insert(pos, TestItem { i + N });
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

class EraseVector : public dslab::Algorithm<void(Vector<TestItem>&)> {
public:
    void operator()(Vector<TestItem>& V) {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        for (size_t i { 0 }; i < N; ++i) {
            auto pos { rand() % V.size() };
            auto e { V.remove(pos) };
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

class FindVector : public dslab::Algorithm<void(Vector<TestItem>&)> {
public:
    void operator()(Vector<TestItem>& V) {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        TestItem tmp { 0 };
        for (size_t i { 0 }; i < N; ++i) {
            tmp.m_value = rand() % (2 * N);
            auto pos { V.find(tmp) };
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

class CopyVector : public dslab::Algorithm<void(Vector<TestItem>&)> {
public:
    void operator()(Vector<TestItem>& V) {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        Vector<TestItem> V2 { V };
        cout << format("copy -> ") << V2 << endl;
        if (TestItem::s_moveCount != 0) {
            throw runtime_error("moveCount != 0");
        }
        if (V.size() != V2.size()) {
            throw runtime_error("size != size");
        }
        for (size_t i { 0 }; i < V.size(); ++i) {
            if (V[i].m_value != V2[i].m_value) {
                throw runtime_error("value != value");
            }
        }
    }
};

class MoveVector : public dslab::Algorithm<void(Vector<TestItem>&)> {
public:
    void operator()(Vector<TestItem>& V) {
        Vector<TestItem> V2 { V };
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        Vector<TestItem> V3 { move(V) };
        cout << format("move -> ") << V3 << endl;
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (TestItem::s_moveCount != 0) {
            throw runtime_error("moveCount != 0");
        }
        if (V.size() != 0) {
            throw runtime_error("size != 0");
        }
        if (V3.size() != V2.size()) {
            throw runtime_error("size != size");
        }
        for (size_t i { 0 }; i < V3.size(); ++i) {
            if (V3[i].m_value != V2[i].m_value) {
                throw runtime_error("value != value");
            }
        }
        V = move(V3);
    }
};

class ResizeVector : public dslab::Algorithm<void(Vector<TestItem>&)> {
public:
    void operator()(Vector<TestItem>& V) {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        for (size_t i { N }; i > 0; --i) {
            V.resize(i);
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
    Vector<TestItem> V {};
    test.initialize();
    test(V);
    return 0;
}
