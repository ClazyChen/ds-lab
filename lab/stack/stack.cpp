#include "stack.hpp"

using dslab::Algorithm;
using dslab::TestItem;
using dslab::Random;

// this experiment is used to demonstrate and verify the basic operations of the stack
// you can replace the stack below with your own implementation to participate in the test
template <typename T>
using Stack = dslab::stack::Stack<T>;

// experiment process:
// 1. create an empty stack, size=0
// 2. insert N elements (copy) at the top of the stack, size=N
// 3. insert N elements (move) at the top of the stack, no copy, size=2N
// 4. remove 2N elements at the top of the stack, no copy, size=0

constexpr std::size_t N { 5 };

// use std::vector and the stack we implemented for comparison
std::vector<std::size_t> stdVector {};

void check(Stack<TestItem>& S) {
    if (S.empty() != stdVector.empty()) {
        throw std::runtime_error("empty not match");
    }
    if (!S.empty()) {
        if (S.top().m_value != stdVector.back()) {
            throw std::runtime_error("top not match");
        }
    }
}

using AVS = Algorithm<void(Stack<TestItem>&)>;

class CreateStack : public AVS {
public:
    void operator()(Stack<TestItem>& S) override {
        S = {};
        stdVector = {};
        check(S);
    }
    std::string type_name() const override {
        return "Create Stack";
    }
};

class PushCopyStack : public AVS {
public:
    void operator()(Stack<TestItem>& S) override {
        TestItem::reset();
        TestItem tmp { 0uz };
        for (auto i { 0uz }; i < N; i++) {
            tmp.m_value = i;
            S.push(tmp);
            stdVector.push_back(i);
            std::cout << std::format("push {} -> {}", i, S) << std::endl;
            check(S);
        }
        if (S.size() != N) {
            throw std::runtime_error("size != N");
        }
    }
    std::string type_name() const override {
        return "Push Stack (Copy)";
    }
};

class PushMoveStack : public AVS {
public:
    void operator()(Stack<TestItem>& S) override {
        TestItem::reset();
        for (auto i { 0uz }; i < N; i++) {
            S.push(TestItem { i + N });
            stdVector.push_back(i + N);
            std::cout << std::format("push {} -> {}", i + N, S) << std::endl;
            check(S);
        }
        if (TestItem::s_copyCount != 0) {
            throw std::runtime_error("copyCount != 0");
        }
        if (S.size() != 2 * N) {
            throw std::runtime_error("size != 2N");
        }
    }
    std::string type_name() const override {
        return "Push Stack (Move)";
    }
};

class PopStack : public AVS {
public:
    void operator()(Stack<TestItem>& S) override {
        TestItem::reset();
        for (auto i { 0uz }; i < 2 * N; i++) {
            S.pop();
            stdVector.pop_back();
            std::cout << std::format("pop -> {}", S) << std::endl;
            check(S);
        }
        if (TestItem::s_copyCount != 0) {
            throw std::runtime_error("copyCount != 0");
        }
        if (S.size() != 0) {
            throw std::runtime_error("size != 0");
        }
    }
    std::string type_name() const override {
        return "Pop Stack";
    }
};

dslab::StructureTestFramework<Stack
    , TestItem
    , CreateStack
    , PushCopyStack
    , PushMoveStack
    , PopStack
> test {};

int main() {
    Stack<TestItem> S {};
    test.initialize();
    test(S);
    return 0;
}
    