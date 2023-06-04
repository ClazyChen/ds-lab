import Framework;
import Stack.SharedStack;
import std;

using namespace std;

using dslab::Algorithm;
using dslab::TestItem;

// 这个实验用于展示和验证共享栈的基本操作
// 您可以将下面的栈替换为自己实现的共享栈参与测试
template <typename T>
using SharedStack = dslab::SharedStack<T>;

// 测试流程：
// 1. 创建一个空共享栈，规模=0
// 2. 在栈1顶插入N个元素（复制），规模=N
// 3. 在栈2顶插入N个元素（移动），要求不触发复制，规模=N
// 4. 在栈1、2顶同时删除N个元素，要求不触发复制，规模=0

constexpr size_t N { 5 };

// 使用std::vector和我们自己实现的栈进行对拍
vector<size_t> stdVector1 {};
vector<size_t> stdVector2 {};

void check(SharedStack<TestItem>& S) {
    auto [S1, S2] { S.getStacks() };
    if (S1.empty() != stdVector1.empty() || S2.empty() != stdVector2.empty()) {
        throw runtime_error("empty not match");
    }
    if (!S1.empty()) {
        if (S1.top().m_value != stdVector1.back()) {
            throw runtime_error("top not match");
        }
    }
    if (!S2.empty()) {
        if (S2.top().m_value != stdVector2.back()) {
            throw runtime_error("top not match");
        }
    }
}

class CreateStack : public Algorithm<void(SharedStack<TestItem>&)> {
public:
    void operator()(SharedStack<TestItem>& S) override {
        S = { {}, {} };
        stdVector1 = {};
        stdVector2 = {};
        check(S);
    }
    string type_name() const override {
        return "Create Stack (Shared)";
    }
};

class PushCopyStack : public Algorithm<void(SharedStack<TestItem>&)> {
public:
    void operator()(SharedStack<TestItem>& S) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        TestItem tmp { 0 };
        for (size_t i { 0 }; i < N; i++) {
            tmp.m_value = i;
            S.getStacks().first.push(tmp);
            stdVector1.push_back(i);
            cout << format("push {} -> ", i) << S << endl;
            check(S);
        }
        if (S.getStacks().first.size() != N) {
            throw runtime_error("size != N");
        }
    }
    string type_name() const override {
        return "Push Stack (Shared) (Copy)";
    }
};

class PushMoveStack : public Algorithm<void(SharedStack<TestItem>&)> {
public:
    void operator()(SharedStack<TestItem>& S) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        for (size_t i { 0 }; i < N; i++) {
            S.getStacks().second.push(TestItem {i + N});
            stdVector2.push_back(i + N);
            cout << format("push {} -> ", i + N) << S << endl;
            check(S);
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (S.getStacks().second.size() != N) {
            throw runtime_error("size != N");
        }
    }
    string type_name() const override {
        return "Push Stack (Shared) (Move)";
    }
};

class PopStack : public Algorithm<void(SharedStack<TestItem>&)> {
public:
    void operator()(SharedStack<TestItem>& S) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        for (size_t i { 0 }; i < N; i++) {
            S.getStacks().first.pop();
            S.getStacks().second.pop();
            stdVector1.pop_back();
            stdVector2.pop_back();
            cout << format("pop -> ") << S << endl;
            check(S);
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (S.getStacks().first.size() != 0) {
            throw runtime_error("size != 0");
        }
        if (S.getStacks().second.size() != 0) {
            throw runtime_error("size != 0");
        }
    }
    string type_name() const override {
        return "Pop Stack (Shared)";
    }
};

dslab::StructureTestFramework<
    SharedStack, TestItem,
    CreateStack, PushCopyStack, PushMoveStack, PopStack> test;

int main() {
    SharedStack<TestItem> S { {}, {} };
    test.initialize();
    test(S);
    return 0;
}