#include "stack.hpp"

using dslab::Algorithm;
using dslab::TestItem;
using dslab::Random;

// this experiment is used to demonstrate and verify the basic operations of the minimum stack
// you can replace the minimum stack below with your own implementation to participate in the test
template <typename T>
using MinStack = dslab::stack::MinStack<T>;

// experiment process:
// 1. create an empty minimum stack, size=0
// 2. insert N elements (copy) at the top of the minimum stack, size=N
// 3. insert N elements (move) at the top of the minimum stack, no copy, size=2N
// 4. remove 2N elements at the top of the minimum stack, no copy, size=0
//    in the above 2, 3, 4, maintain the minimum value continuously

constexpr std::size_t N { 5 };

// use std::vector and the minimum stack we implemented for comparison
std::vector<std::size_t> stdVector {};

void check(MinStack<TestItem>& S) {
    if (S.empty() != stdVector.empty()) {
        throw std::runtime_error("empty not match");
    }
    if (!S.empty()) {
        if (S.top().m_value != stdVector.back()) {
            throw std::runtime_error("top not match");
        }
        if (S.min().m_value != *std::min_element(stdVector.begin(), stdVector.end())) {
            throw std::runtime_error("min not match");
        }
    }
}

using AVS = Algorithm<void(MinStack<TestItem>&)>;

class CreateStack : public AVS {
public:
    void operator()(MinStack<TestItem>& S) override {
        S = {};
        stdVector = {};
        check(S);
    }
    std::string type_name() const override {
        return "Create Min Stack";
    }
};

class PushCopyStack : public AVS {
public:
    void operator()(MinStack<TestItem>& S) override {
        TestItem::reset();
        TestItem tmp { 0uz };
        std::vector<std::size_t> v(N);
        std::iota(v.begin(), v.end(), 0);
        std::shuffle(v.begin(), v.end(), Random::engine());
        for (auto i : v) {
            tmp.m_value = i + N;
            S.push(tmp);
            stdVector.push_back(i + N);
            std::cout << std::format("push {} -> {}", i + N, S) << std::endl;
            check(S);
        }
        if (S.size() != N) {
            throw std::runtime_error("size != N");
        }
    }
    std::string type_name() const override {
        return "Push Copy Stack";
    }
};

class PushMoveStack : public AVS {
public:
    void operator()(MinStack<TestItem>& S) override {
        TestItem::reset();
        std::vector<std::size_t> v(N);
        std::iota(v.begin(), v.end(), 0);
        std::shuffle(v.begin(), v.end(), Random::engine());
        for (auto i : v) {
            S.push(TestItem { i });
            stdVector.push_back(i);
            std::cout << std::format("push {} -> {}", i, S) << std::endl;
            check(S);
        }
        if (S.size() != 2 * N) {
            throw std::runtime_error("size != 2N");
        }
    }
    std::string type_name() const override {
        return "Push Move Stack";
    }
};

class PopStack : public AVS {
public:
    void operator()(MinStack<TestItem>& S) override {
        TestItem::reset();
        for (auto i { 0uz }; i < 2 * N; i++) {
            S.pop();
            stdVector.pop_back();
            std::cout << std::format("pop -> {}", S) << std::endl;
            check(S);
        }
        if (!S.empty()) {
            throw std::runtime_error("size != 0");
        }
        if (TestItem::s_copyCount != 0) {
            throw std::runtime_error("copyCount != 0");
        }
    }
    std::string type_name() const override {
        return "Pop Stack";
    }
};

dslab::StructureTestFramework<MinStack
    , TestItem
    , CreateStack
    , PushCopyStack
    , PushMoveStack
    , PopStack
> test {};

int main() {
    MinStack<TestItem> S;
    test.initialize();
    test(S);
    return 0;
}