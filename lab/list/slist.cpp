#include "static_list.hpp"

using namespace std;

using dslab::Algorithm;
using dslab::TestItem;
using dslab::Random;

// this experiment is to test the performance of the static list
// you can replace the list with your own implementation
template <typename T>
using List = dslab::StaticList<T>;

// note that the static list is similar to the list
// the differences is:
// +  the static list's insert and copy are vector operations
//    for insert, there is potential resize overhead
//    for copy, more elements need to be copied due to the load factor

// test process:
// 1. create an empty list, size=0
// 2. insert N elements at the end (copy), no move, size=N
// 3. insert N elements at random position (move), no copy, size=2N
// 4. remove N elements at random position, no copy, only move, size=N
// 5. find N elements at random position, no move or copy, size=N
// 6. insert N elements as prev (copy), no move, size=2N
// 7. call copy constructor, no move, same before and after
// 8. call move constructor, no copy or move, same before and after

constexpr std::size_t N { 5 };

// use std::vector and our list to compare
std::vector<std::size_t> stdVector {};

void check(List<TestItem>& L) {
    if (L.size() != stdVector.size()) {
        throw std::runtime_error("size not match");
    }
    auto it { L.begin() };
    for (auto& x : stdVector) {
        if (it->m_value != x) {
            throw std::runtime_error("value not match");
        }
        ++it;
    }
}

using ALT = Algorithm<void(List<TestItem>&)>;

class CreateList : public ALT {
public:
    void operator()(List<TestItem>& L) override {
        L = {};
        stdVector = {};
        check(L);
        if (L.size() != 0) {
            throw std::runtime_error("size != 0");
        }
    }
    std::string type_name() const override {
        return "Create List";
    }
};

class PushBackList : public ALT {
public:
    void operator()(List<TestItem>& L) override {
        TestItem::reset();
        TestItem tmp { 0uz };
        for (auto i { 0uz }; i < N; ++i) {
            tmp.m_value = i;
            L.push_back(tmp);
            stdVector.push_back(i);
            std::cout << std::format("push back {} -> {}", i, L) << std::endl;
            check(L);
        }
        if (L.size() != N) {
            throw std::runtime_error("size != N");
        }
    }
    std::string type_name() const override {
        return "Push Back";
    }
};

class InsertAsNextList : public ALT {
public:
    void operator()(List<TestItem>& L) override {
        TestItem::reset();
        for (auto i { 0uz }; i < N; ++i) {
            auto pos { Random::get(L.size() - 1) };
            auto p { L.begin() + pos };
            L.insertAsNext(p, TestItem { i + N });
            stdVector.insert(stdVector.begin() + pos + 1, i + N);
            std::cout << std::format("insert as next {} @ {:2} -> {}", i + N, *p, L) << std::endl;
            check(L);
        }
        if (TestItem::s_copyCount != 0) {
            throw std::runtime_error("copyCount != 0");
        }
        if (L.size() != 2 * N) {
            throw std::runtime_error("size != 2N");
        }
    }
    std::string type_name() const override {
        return "Insert As Next";
    }
};


class EraseList : public ALT {
public:
    void operator()(List<TestItem>& L) override {
        TestItem::reset();
        for (auto i { 0uz }; i < N; ++i) {
            auto pos { Random::get(L.size() - 1) };
            auto p { L.begin() + pos };
            auto e { std::move(*p) };
            L.erase(p);
            stdVector.erase(stdVector.begin() + pos);
            std::cout << std::format("erase {:2} -> {}", e, L) << std::endl;
            check(L);
        }
        if (TestItem::s_copyCount != 0) {
            throw std::runtime_error("copyCount != 0");
        }
        if (L.size() != N) {
            throw std::runtime_error("size != N");
        }
    }
    std::string type_name() const override {
        return "Erase List";
    }
};

class FindList : public ALT {
public:
    void operator()(List<TestItem>& L) override {
    	TestItem::reset();
        TestItem tmp { 0uz };
        for (auto i { 0uz }; i < N; ++i) {
            tmp.m_value = Random::get(2 * N - 1);
            auto pos { L.find(tmp) };
            auto stdPos { std::find(stdVector.begin(), stdVector.end(), tmp.m_value) };
            if (std::distance(stdVector.begin(), stdPos) != std::distance(L.begin(), pos)) {
                throw std::runtime_error("find error");
            }
            std::cout << std::format("find {} ({}) -> {}", tmp.m_value, pos != L.end(), L) << std::endl;
        }
        if (TestItem::s_copyCount != 0) {
            throw std::runtime_error("copyCount != 0");
        }
        if (TestItem::s_moveCount != 0) {
            throw std::runtime_error("moveCount != 0");
        }
        if (L.size() != N) {
            throw std::runtime_error("size != N");
        }
    }
    std::string type_name() const override {
        return "Find List";
    }
};

class InsertAsPrevList : public ALT {
public:
    void operator()(List<TestItem>& L) override {
        TestItem::reset();
        TestItem tmp { 0uz };
        for (auto i { 0uz }; i < N; ++i) {
            auto pos { Random::get(L.size()) };
            auto p { L.begin() + pos };
            tmp.m_value = i + 2 * N;
            L.insertAsPrev(p, tmp);
            stdVector.insert(stdVector.begin() + pos, i + 2 * N);
            std::cout << std::format("insert as prev {} @ {:2} -> {}", i + 2 * N, *p, L) << std::endl;
            check(L);
        }
        if (L.size() != 2 * N) {
            throw std::runtime_error("size != 2N");
        }
    }
    std::string type_name() const override {
        return "Insert As Prev List";
    }
};

class CopyConstructor : public ALT {
public:
    void operator()(List<TestItem>& L) override {
        TestItem::reset();
        auto L2 { L };
        check(L2);
        std::cout << std::format("copy -> {}", L2) << std::endl;
        if (L.size() != L2.size()) {
            throw std::runtime_error("size not match");
        }
        if (TestItem::s_moveCount != 0) {
            throw std::runtime_error("moveCount != 0");
        }
    }
    std::string type_name() const override {
        return "Copy Constructor";
    }
};

class MoveConstructor : public ALT {
public:
    void operator()(List<TestItem>& L) override {
        TestItem::reset();
        auto L2 { std::move(L) };
        check(L2);
        std::cout << std::format("move -> {}", L2) << std::endl;
        if (L.size() != 0) {
            throw std::runtime_error("size != 0");
        }
        if (TestItem::s_copyCount != 0) {
            throw std::runtime_error("copyCount != 0");
        }
        if (TestItem::s_moveCount != 0) {
            throw std::runtime_error("moveCount != 0");
        }
    }
    std::string type_name() const override {
        return "Move Constructor";
    }
};

dslab::StructureTestFramework<List
    , TestItem
    , CreateList
    , PushBackList
    , InsertAsNextList
    , EraseList
    , FindList
    , InsertAsPrevList
    , CopyConstructor
    , MoveConstructor
> test {};

int main() {
    List<TestItem> L {};
    test.initialize();
    test(L);
    return 0;
}