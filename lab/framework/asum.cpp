#include "framework.hpp"
#include <numeric>
#include <algorithm>
#include <span>
#include <ranges>

using namespace dslab;

using ArraySum = Algorithm<int(std::span<const int>)>;

class ArraySumBasic : public ArraySum {
public:
    int operator()(std::span<const int> data) override {
        return std::accumulate(data.begin(), data.end(), 0);
    }

    std::string type_name() const override {
        return "ArraySum (std::accumulate)";
    }
};

class ArraySumReduce : public ArraySum {
public:
    int operator()(std::span<const int> data) override {
        return std::reduce(data.begin(), data.end(), 0);
    }

    std::string type_name() const override {
        return "ArraySum (std::reduce)";
    }
};

class ArraySumFold : public ArraySum {
public:
    int operator()(std::span<const int> data) override {
        return std::ranges::fold_left(data, 0, std::plus{});
    }

    std::string type_name() const override {
        return "ArraySum (std::ranges::fold_left)";
    }
};

// use naive iterative method to calculate sum
class ArraySumIterative : public ArraySum {
public:
    int operator()(std::span<const int> data) override {
        int sum { 0 };
        for (auto x : data) {
            sum += x;
        }
        return sum;
    }

    std::string type_name() const override {
        return "ArraySum (Iterative)";
    }
};

// reduce and conquer method to calculate sum
// when size > 10000, it will throw stack overflow
class ArraySumRnC : public ArraySum {
    static constexpr int THRESHOLD { 10000 };
    int cal(std::span<const int> data) {
        if (data.size() == 0) {
            return 0;
        }
        return cal(data.first(data.size() - 1)) + data.back();
    }
public:
    int operator()(std::span<const int> data) override {
        if (data.size() > THRESHOLD) {
            throw std::runtime_error { "stack overflow" };
        }
        return cal(data);
    }

    std::string type_name() const override {
        return "ArraySum (Reduce and Conquer)";
    }
};

// divide and conquer method to calculate sum
class ArraySumDnC : public ArraySum {
public:
    int operator()(std::span<const int> data) override {
        if (auto sz { data.size() }; sz == 0) {
            return 0;
        } else if (sz == 1) {
            return data.front();
        } else {
            auto mid { sz / 2 };
            return (*this)(data.first(mid)) + (*this)(data.subspan(mid));
        }
    }

    std::string type_name() const override {
        return "ArraySum (Divide and Conquer)";
    }
};

std::vector testSize { 10, 100, 1000, 10000, 10'0000, 100'0000, 1000'0000, 1'0000'0000, 10'0000'0000 };

TestFramework<ArraySum, 
    ArraySumBasic, 
    ArraySumReduce,
	ArraySumFold, 
    ArraySumIterative,
    ArraySumRnC,
    ArraySumDnC
    > test;

int main() {
    for (auto size : testSize) {
        std::cout << std::format("Test for size {}", size) << std::endl;
        std::vector<int> data(size);
        std::iota(data.begin(), data.end(), 1);
        test(data);
    }
    return 0;
}