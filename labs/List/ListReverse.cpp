#include <vector>
#include <numeric>
#include <format>
#include <iostream>
import Framework;
import Vector;
import List;
using namespace dslab;
using namespace std;

template <typename T>
class ReverseListProblem : public Algorithm<void(List<T>&)> {};

template <typename T>
class ReverseStd : public ReverseListProblem<T> {
public:
    void operator()(List<T>& L) override {
        reverse(begin(L), end(L));
    }
};

template <typename T>
class ReverseBasic : public ReverseListProblem<T> {
public:
    void operator()(List<T>& L) override {
        auto head { L.head() };
        auto tail { L.tail() };
        for (auto size { L.size() }; size >= 2; size -= 2) {
            head = head->next().get();
            tail = tail->prev();
            std::swap(head->data(), tail->data());
        }
    }
};

template <typename T>
class ReverseForwardList : public ReverseListProblem<T> {
protected:
    virtual void apply(List<T>& L) = 0;
public:
    void operator()(List<T>& L) override {
        apply(L);
        for (auto p { L.head() }; p != L.tail(); p = p->next().get()) {
            p->next()->prev() = p;
        }
    }
};


template <typename T>
class ReverseReduceAtHead : public ReverseForwardList<T> {
    void reverse(List<T>& L) {
        if (L.size() < 2) {
            return;
        }
        auto first { L.pop_front() };
        reverse(L);
        L.push_back(first);
    }
protected:
    void apply(List<T>& L) override {
        if (L.size() > 10000) {
            throw runtime_error { "stack overflow" };
        }
        reverse(L);
    }
};

template <typename T>
class ReverseMove : public ReverseListProblem<T> {
public:
    void operator()(List<T>& L) override {
        Vector<T> V(L.size());
        move(begin(L), end(L), begin(V));
        reverse(begin(V), end(V));
        move(begin(V), end(V), begin(L));
    }
};

template <typename T>
class ReverseReduceAtTail : public ReverseForwardList<T> {
    unique_ptr<ListNode<T>> reverse(unique_ptr<ListNode<T>>& start, size_t size) {
        if (size == 1) {
            return move(start->next());
        }
        auto last { reverse(start, size - 1) };
        auto tmp { move(last->next()) };
        last->next() = move(start);
        start = move(last);
        return tmp;
    }
protected:
    void apply(List<T>& L) override {
        if (L.size() > 10000) {
            throw runtime_error { "stack overflow" };
        }
        auto old_first { L.first() };
        auto start { move(L.head()->next()) };
        auto last { reverse(start, L.size()) };
        L.head()->next() = move(start);
        old_first->next() = move(last);
    }
};

template <typename T>
class ReverseReduceAtTailIterative : public ReverseForwardList<T> {
protected:
    void apply(List<T>& L) override {
        auto old_first { L.first() };
        auto start { move(L.head()->next()) };
        auto last { move(start->next()) };
        for (auto size { L.size() }; size > 1; --size) {
            auto tmp { move(last->next()) };
            last->next() = move(start);
            start = move(last);
            last = move(tmp);
        }
        L.head()->next() = move(start);
        old_first->next() = move(last);
    }
};

vector testData { 10, 1000, 10000, 100'000, 1'000'000, 10'000'000 };

TestFramework<ReverseListProblem<int>, 
    ReverseStd<int>,
    ReverseBasic<int>,
    ReverseMove<int>,
    ReverseReduceAtHead<int>,
    ReverseReduceAtTail<int>,
    ReverseReduceAtTailIterative<int>> test;

int main() {
    for (auto n : testData) {
        cout << format("n = {:10d}", n) << endl;
        List<int> L;
        for (int i { 0 }; i < n; ++i) {
            L.push_back(i);
        }
        test(ref(L));
    }
}