#include <vector>
#include <format>
#include <iostream>
import Framework;
import Stack;
using namespace std;

using dslab::Algorithm;
using dslab::TestItem;

// 这个实验用于展示和验证栈的基本操作
// 您可以将下面的栈替换为自己实现的栈参与测试
template <typename T>
using Stack = dslab::Stack<T>;

// 测试流程：
// 1. 创建一个空栈，规模=0
// 2. 在栈顶插入N个元素（复制），规模=N
// 3. 在栈顶插入N个元素（移动），要求不触发复制，规模=2N
// 4. 在栈顶删除2N个元素，要求不触发复制，规模=0

constexpr size_t N { 5 };

// 使用std::vector和我们自己实现的栈进行对拍
vector<size_t> stdVector {};

void check(Stack<TestItem>& S) {
    if (S.empty() != stdVector.empty()) {
        throw runtime_error("empty not match");
    }
    if (!S.empty()) {
        if (S.top().m_value != stdVector.back()) {
            throw runtime_error("top not match");
        }
    }
}

class CreateStack : public Algorithm<void(Stack<TestItem>&)> {
public:
    void operator()(Stack<TestItem>& S) override {
        S = {};
        stdVector = {};
        check(S);
    }
};

class PushCopyStack : public Algorithm<void(Stack<TestItem>&)> {
public:
    void operator()(Stack<TestItem>& S) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        TestItem tmp { 0 };
        for (size_t i { 0 }; i < N; i++) {
            tmp.m_value = i;
            S.push(tmp);
            stdVector.push_back(i);
            cout << format("push {} -> ", i) << S << endl;
            check(S);
        }
        if (S.size() != N) {
            throw runtime_error("size != N");
        }
    }
};

class PushMoveStack : public Algorithm<void(Stack<TestItem>&)> {
public:
    void operator()(Stack<TestItem>& S) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        for (size_t i { 0 }; i < N; i++) {
            S.push(TestItem { i + N });
            stdVector.push_back(i + N);
            cout << format("push {} -> ", i + N) << S << endl;
            check(S);
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (S.size() != 2 * N) {
            throw runtime_error("size != 2 * N");
        }
    }
};

class PopStack : public Algorithm<void(Stack<TestItem>&)> {
public:
    void operator()(Stack<TestItem>& S) override {
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
};

dslab::StructureTestFramework<
    Stack, TestItem,
    CreateStack, PushCopyStack, PushMoveStack, PopStack> test;

int main() {
    Stack<TestItem> S {};
    test.initialize();
    test(S);
    return 0;
}