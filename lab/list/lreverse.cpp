#include "vector.hpp"
#include "list.hpp"

using namespace dslab;

template <typename T>
using ReverseList = Algorithm<void(List<T>&)>;

template <typename T>
class ReverseStd : public ReverseList<T> {
public:
    void operator()(List<T>& L) override {
        std::reverse(L.begin(), L.end());
    }
    std::string type_name() const override {
        return "Reverse (std::reverse)";
    }
};

template <typename T>
class ReverseBasic : public ReverseList<T> {
public:
    void operator()(List<T>& L) override {
        auto l { L.begin() }, r { --L.end() };
        for (auto n { L.size() }; n >= 2; n -= 2) {
            std::iter_swap(l++, r--);
        }
    }
    std::string type_name() const override {
        return "Reverse (swap head & tail)";
    }
};

template <typename T>
class ReverseReduceAtHead : public ReverseList<T> {
    void reverse(List<T>& L) {
        if (L.size() <= 1) {
            return;
        }
        auto first { std::move(*L.begin()) };
        L.pop_front();
        reverse(L);
        L.push_back(first);
    }
public:
    void operator()(List<T>& L) override {
        if (L.size() >= 10000) {
            throw std::runtime_error {"stack overflow"};
        }
        reverse(L);
    }
    std::string type_name() const override {
        return "Reverse (reduce at head / recursive)";
    }
};

template <typename T>
class ReverseMove : public ReverseList<T> {
public:
    void operator()(List<T>& L) override {
        Vector<T> V(L.size());
        std::move(L.begin(), L.end(), V.begin());
        std::reverse(V.begin(), V.end());
        std::move(V.begin(), V.end(), L.begin());
    }
    std::string type_name() const override {
        return "Reverse (move to vector and back)";
    }
};

template <typename T>
class ReverseReduceAtTail : public ReverseList<T> {
    using iterator = typename List<T>::iterator;
    iterator reverse(iterator head, std::size_t sz) {
        if (sz <= 1) {
            return ++head;
        }
        auto p { reverse(head, sz - 1) };
        auto q { p + 1 }, r { q + 1 };
        r.node()->prev() = p.node();
        q.node()->prev() = head.node();
        head.node()->next()->prev() = q.node();
        auto tmp { std::move(p.node()->next()) };
        p.node()->next() = std::move(q.node()->next());
        q.node()->next() = std::move(head.node()->next());
        head.node()->next() = std::move(tmp);
        return p;
    }
public:
    void operator()(List<T>& L) override {
        if (L.size() >= 10000) {
            throw std::runtime_error {"stack overflow"};
        }
        reverse(--L.begin(), L.size());
    }
    std::string type_name() const override {
        return "Reverse (reduce at tail / recursive)";
    }
};

template <typename T>
class ReverseIterative : public ReverseList<T> {
    using iterator = typename List<T>::iterator;
public:
    void operator()(List<T>& L) override {
        auto head { --L.begin() }, p { head + 1 };
        for (auto i { 1uz }; i < L.size(); ++i) {
            auto q { p + 1 }, r { q + 1 };
            r.node()->prev() = p.node();
            q.node()->prev() = head.node();
            head.node()->next()->prev() = q.node();
            auto tmp { std::move(p.node()->next()) };
            p.node()->next() = std::move(q.node()->next());
            q.node()->next() = std::move(head.node()->next());
            head.node()->next() = std::move(tmp);
        }
    }
    std::string type_name() const override {
        return "Reverse (iterative)";
    }
};

std::vector<std::size_t> testData { 10, 1000, 10000, 100'000, 1'000'000, 10'000'000 };

TestFramework<ReverseList<std::size_t>, 
    ReverseStd<std::size_t>, 
    ReverseBasic<std::size_t>, 
    ReverseReduceAtHead<std::size_t>, 
    ReverseMove<std::size_t>, 
    ReverseReduceAtTail<std::size_t>, 
    ReverseIterative<std::size_t>> test;

int main() {
    for (auto n : testData) {
        std::cout << std::format("n = {:>10d}", n) << std::endl;
        List<std::size_t> L;
        for (auto i { 0uz }; i < n; ++i) {
            L.push_back(i);
        }
        test(std::ref(L));
    }
}