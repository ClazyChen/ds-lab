#include "stack.hpp"

using dslab::Algorithm;
using dslab::TestItem;
using dslab::Random;

// this experiment is used to demonstrate and verify the basic operations of the shared stack
// you can replace the shared stack below with your own implementation to participate in the test
template <typename T>
using SharedStack = dslab::stack::SharedStack<T>;

// experiment process:
// 1. create an empty shared stack, size=0
// 2. insert N elements (copy) at the top of stack 1, size=N
// 3. insert N elements (move) at the top of stack 2, no copy, size=N
// 4. remove N elements at the top of stack 1 and stack 2 simultaneously, no copy, size=0

constexpr std::size_t N { 5 };

// use std::vector and the shared stack we implemented for comparison
std::vector<std::size_t> stdVector1 {};
std::vector<std::size_t> stdVector2 {};

void check(SharedStack<TestItem>& S) {
    const auto& [S1, S2] { S.getStacks() };
    if (S1.empty() != stdVector1.empty() || S2.empty() != stdVector2.empty()) {
        throw std::runtime_error("empty not match");
    }
    if (!S1.empty()) {
        if (S1.top().m_value != stdVector1.back()) {
            throw std::runtime_error("top not match");
        }
    }
    if (!S2.empty()) {
        if (S2.top().m_value != stdVector2.back()) {
            throw std::runtime_error("top not match");
        }
    }
}

using AVS = Algorithm<void(SharedStack<TestItem>&)>;

class CreateStack : public AVS {
public:
    void operator()(SharedStack<TestItem>& S) override {
        S = { {}, {} };
        stdVector1 = {};
        stdVector2 = {};
        check(S);
    }
    std::string type_name() const override {
        return "Create Stack (Shared)";
    }
};

class PushCopyStack : public AVS {
public:
    void operator()(SharedStack<TestItem>& S) override {
        TestItem::reset();
        TestItem tmp { 0uz };
        for (auto i { 0uz }; i < N; i++) {
            tmp.m_value = i;
            S.getStacks().first.push(tmp);
            stdVector1.push_back(i);
            std::cout << std::format("push {} -> {}", i, S) << std::endl;
            check(S);
        }
        if (S.size() != N || S.getStacks().first.size() != N) {
            throw std::runtime_error("size != N");
        }
    }
    std::string type_name() const override {
        return "Push Copy Stack (Shared)";
    }
};

class PushMoveStack : public AVS {
public:
    void operator()(SharedStack<TestItem>& S) override {
        TestItem::reset();
        for (auto i { 0uz }; i < N; i++) {
            S.getStacks().second.push(TestItem { i + N });
            stdVector2.push_back(i + N);
            std::cout << std::format("push {} -> {}", i + N, S) << std::endl;
            check(S);
        }
        if (S.size() != 2 * N || S.getStacks().second.size() != N) {
            throw std::runtime_error("size != 2 * N");
        }
        if (TestItem::s_copyCount != 0) {
            throw std::runtime_error("copyCount != 0");
        }
    }
    std::string type_name() const override {
        return "Push Move Stack (Shared)";
    }
};

class PopStack : public AVS {
public:
    void operator()(SharedStack<TestItem>& S) override {
        TestItem::reset();
        for (auto i { 0uz }; i < N; i++) {
            S.getStacks().first.pop();
            stdVector1.pop_back();
            S.getStacks().second.pop();
            stdVector2.pop_back();
            std::cout << std::format("pop -> {}", S) << std::endl;
            check(S);
        }
        if (S.size() != 0 || S.getStacks().first.size() != 0 || S.getStacks().second.size() != 0) {
            throw std::runtime_error("size != 0");
        }
        if (TestItem::s_copyCount != 0) {
            throw std::runtime_error("copyCount != 0");
        }
    }
    std::string type_name() const override {
        return "Pop Stack (Shared)";
    }
};

dslab::StructureTestFramework<SharedStack
    , TestItem
    , CreateStack
    , PushCopyStack
    , PushMoveStack
    , PopStack
> test {};

int main() {
    SharedStack<TestItem> S;
    test.initialize();
    test(S);
    return 0;
}