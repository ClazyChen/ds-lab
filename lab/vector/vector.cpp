#include "vector.hpp"

using dslab::Algorithm;
using dslab::TestItem;
using dslab::Random;

// this experiment is to test the vector
// you can replace the vector with your own implementation
template <typename T>
using Vector = dslab::DefaultVector<T>;

// test process:
// 1. create an empty vector, size=0, capacity=0
// 2. modify capacity to N, size=0, capacity=N
// 3. insert N elements at the end (copy), no move, size=N, capacity unchanged
// 4. insert N elements at random position (move), no copy, size=2N
// 5. remove N elements at random position, no copy, size=N
// 6. find N elements at random position, no move or copy, size, capacity unchanged
// 7. call copy constructor, no move, same before and after
// 8. call move constructor, no copy or move, same before and after
// 9. modify size to decrease, no move or copy, size decrease

constexpr size_t N { 5 };

// use std::vector and our own vector to compare
std::vector<std::size_t> stdVector {};

void check(Vector<TestItem>& V) {
    if (V.size() != stdVector.size()) {
        throw std::runtime_error("size not match");
    }
    auto it { V.begin() };
    for (auto& x : stdVector) {
        if (it->m_value != x) {
            throw std::runtime_error("value not match");
        }
        ++it;
    }
}

using AVT = Algorithm<void(Vector<TestItem>&)>;

class CreateVector : public AVT {
public:
    void operator()(Vector<TestItem>& V) override {
        V = {};
        stdVector = {};
        check(V);
        if (V.size() != 0) {
            throw std::runtime_error("size != 0");
        }
        if (V.capacity() != 0) {
            throw std::runtime_error("capacity != 0");
        }
    }
    std::string type_name() const override {
        return "Create Vector";
    }
};

class ModifyCapacity : public AVT {
public:
    void operator()(Vector<TestItem>& V) override {
        V.reserve(N);
        stdVector.reserve(N);
        check(V);
        if (V.size() != 0) {
            throw std::runtime_error("size != 0");
        }
        if (V.capacity() != N) {
            throw std::runtime_error("capacity != N");
        }
    }
    std::string type_name() const override {
        return "Modify Capacity";
    }
};

class PushBackCopy : public AVT {
public:
    void operator()(Vector<TestItem>& V) override {
        TestItem::reset();
        TestItem tmp { 0uz };
        for (auto i { 0uz }; i < N; ++i) {
            tmp.m_value = i;
            V.push_back(tmp);
            stdVector.push_back(i);
            check(V);
            std::cout << std::format("push back {} -> {}", i, V) << std::endl;
        }
        if (TestItem::s_moveCount != 0) {
            throw std::runtime_error("move count != 0");
        }
        if (V.size() != N) {
            throw std::runtime_error("size != N");
        }
        if (V.capacity() != N) {
            throw std::runtime_error("capacity != N");
        }
    }
    std::string type_name() const override {
        return "Push Back (Copy)";
    }
};

class InsertMove : public AVT {
public:
    void operator()(Vector<TestItem>& V) override {
        TestItem::reset();
        for (auto i { 0uz }; i < N; ++i) {
            auto pos { Random::get(V.size()) };
            V.insert(V.begin() + pos, TestItem { i + N });
            stdVector.insert(stdVector.begin() + pos, i + N);
            check(V);
            std::cout << std::format("insert {} @ {:2} -> {}", i + N, pos, V) << std::endl;
        }
        if (TestItem::s_copyCount != 0) {
            throw std::runtime_error("copy count != 0");
        }
        if (V.size() != 2 * N) {
            throw std::runtime_error("size != 2N");
        }
    }
    std::string type_name() const override {
        return "Insert (Move)";
    }
};

class EraseVector : public AVT {
public:
    void operator()(Vector<TestItem>& V) override {
        TestItem::reset();
        for (auto i { 0uz }; i < N; ++i) {
            auto pos { Random::get(V.size() - 1) };
            V.erase(V.begin() + pos);
            stdVector.erase(stdVector.begin() + pos);
            check(V);
            std::cout << std::format("erase @ {:2} -> {}", pos, V) << std::endl;
        }
        if (TestItem::s_copyCount != 0) {
            throw std::runtime_error("copy count != 0");
        }
        if (V.size() != N) {
            throw std::runtime_error("size != N");
        }
    }
    std::string type_name() const override {
        return "Erase Vector";
    }
};

class FindVector : public AVT {
public:
    void operator()(Vector<TestItem>& V) override {
        TestItem::reset();
        TestItem tmp { 0uz };
        for (auto i { 0uz }; i < N; ++i) {
            tmp.m_value = Random::get(2 * N - 1);
            auto pos { V.find(tmp) };
            auto stdPos { std::find(stdVector.begin(), stdVector.end(), tmp.m_value) };
            if (std::distance(stdVector.begin(), stdPos) != std::distance(V.begin(), pos)) {
                throw std::runtime_error("find error");
            }
            std::cout << format("find {} @ {:2} -> {}", tmp.m_value, std::distance(V.begin(), pos), V) << std::endl;
        }
        if (TestItem::s_copyCount != 0) {
            throw std::runtime_error("copy count != 0");
        }
        if (TestItem::s_moveCount != 0) {
            throw std::runtime_error("move count != 0");
        }
        if (V.size() != N) {
            throw std::runtime_error("size != N");
        }
    }
    std::string type_name() const override {
        return "Find Vector";
    }
};

class CopyConstructor : public AVT {
public:
    void operator()(Vector<TestItem>& V) override {
        TestItem::reset();
        auto V2 { V };
        check(V2);
        std::cout << format("copy -> {}", V2) << std::endl;
        if (TestItem::s_moveCount != 0) {
            throw std::runtime_error("move count != 0");
        }
        if (V.size() != V2.size()) {
            throw std::runtime_error("size != size");
        }
    }
    std::string type_name() const override {
        return "Copy Constructor";
    }
};

class MoveConstructor : public AVT {
public:
    void operator()(Vector<TestItem>& V) override {
        TestItem::reset();
        auto V2 { std::move(V) };
        check(V2);
        std::cout << format("move -> {}", V2) << std::endl;
        if (TestItem::s_copyCount != 0) {
            throw std::runtime_error("copy count != 0");
        }
        if (TestItem::s_moveCount != 0) {
            throw std::runtime_error("move count != 0");
        }
        if (V.size() != 0) {
            throw std::runtime_error("size != 0");
        }
        V = std::move(V2);
    }
    std::string type_name() const override {
        return "Move Constructor";
    }
};

class ModifySize : public AVT {
public:
    void operator()(Vector<TestItem>& V) override {
        TestItem::reset();
        for (auto i { N }; i > 0; --i) {
            V.resize(i);
            stdVector.resize(i);
            check(V);
            std::cout << format("resize -> {}", V) << std::endl;
            if (V.size() != i) {
                throw std::runtime_error("size != i");
            }
        }
        if (TestItem::s_copyCount != 0) {
            throw std::runtime_error("copy count != 0");
        }
        if (TestItem::s_moveCount != 0) {
            throw std::runtime_error("move count != 0");
        }
    }
    std::string type_name() const override {
        return "Modify Size";
    }
};

dslab::StructureTestFramework<Vector
    , TestItem
    , CreateVector
    , ModifyCapacity
    , PushBackCopy
    , InsertMove
    , EraseVector
    , FindVector
    , CopyConstructor
    , MoveConstructor
    , ModifySize
    > test {};

int main() {
    Vector<TestItem> V {};
    test.initialize();
    test(V);
    std::cout << "All tests passed!" << std::endl;
    return 0;
};