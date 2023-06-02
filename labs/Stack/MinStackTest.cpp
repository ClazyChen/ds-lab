import Framework;
import Stack.MinStack;
import std;

using namespace std;

using dslab::Algorithm;
using dslab::TestItem;

// 这个实验用于展示和验证最小栈的基本操作
// 您可以将下面的最小栈替换为自己实现的栈参与测试
template <typename T>
using MinStack = dslab::MinStack<T>;

// 测试流程：
// 1. 创建一个空栈，规模=0
// 2. 在栈顶插入N个元素（复制），规模=N
// 3. 在栈顶插入N个元素（移动），规模=2N
// 4. 在栈顶删除2N个元素，要求不触发复制，规模=0
//    在上述2、3、4中，持续维护最小值

constexpr size_t N { 5 };

// 使用std::vector和我们自己实现的栈进行对拍
vector<size_t> stdVector {};

void check(MinStack<TestItem>& S) {
    if (S.empty() != stdVector.empty()) {
        throw runtime_error("empty not match");
    }
    if (!S.empty()) {
        if (S.top().m_value != stdVector.back()) {
            throw runtime_error("top not match");
        }
        if (S.min().m_value != *min_element(stdVector.begin(), stdVector.end())) {
            throw runtime_error("min not match");
        }
    }
}

class CreateStack : public Algorithm<void(MinStack<TestItem>&)> {
public:
    void operator()(MinStack<TestItem>& S) override {
        S = {};
        stdVector = {};
        check(S);
    }
    string type_name() const override {
        return "Create Min Stack";
    }
};

default_random_engine engine { random_device {}() };

class PushCopyStack : public Algorithm<void(MinStack<TestItem>&)> {
public:
    void operator()(MinStack<TestItem>& S) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        TestItem tmp { 0 };
        vector<size_t> v(N);
        iota(begin(v), end(v), 0);
        shuffle(begin(v), end(v), engine);
        for (size_t i : v) {
            tmp.m_value = i + N;
            S.push(tmp);
            stdVector.push_back(i + N);
            cout << format("push {} -> ", i + N) << S << endl;
            check(S);
        }
        if (S.size() != N) {
            throw runtime_error("size != N");
        }
    }
    string type_name() const override {
        return "Push Min Stack (Copy)";
    }
};

class PushMoveStack : public Algorithm<void(MinStack<TestItem>&)> {
public:
    void operator()(MinStack<TestItem>& S) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        vector<size_t> v(N);
        iota(begin(v), end(v), 0);
        shuffle(begin(v), end(v), engine);
        for (size_t i : v) {
            S.push(TestItem { i });
            stdVector.push_back(i);
            cout << format("push {} -> ", i + N) << S << endl;
            check(S);
        }
        if (S.size() != 2 * N) {
            throw runtime_error("size != 2 * N");
        }
    }
    string type_name() const override {
        return "Push Min Stack (Move)";
    }
};

class PopStack : public Algorithm<void(MinStack<TestItem>&)> {
public:
    void operator()(MinStack<TestItem>& S) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        for (size_t i { 0 }; i < 2 * N; i++) {
            S.pop();
            stdVector.pop_back();
            cout << format("pop -> ") << S << endl;
            check(S);
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (S.size() != 0) {
            throw runtime_error("size != 0");
        }
    }
    string type_name() const override {
        return "Pop Min Stack";
    }
};

dslab::StructureTestFramework<
    MinStack, TestItem,
    CreateStack, PushCopyStack, PushMoveStack, PopStack> test;

int main() {
    MinStack<TestItem> S {};
    test.initialize();
    test(S);
    return 0;
}