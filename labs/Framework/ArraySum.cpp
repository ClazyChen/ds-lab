#include <vector>
#include <span>
#include <format>
#include <iostream>
#include <numeric>
import Framework;
using namespace dslab;
using namespace std;

// �����������
class ArraySum : public Algorithm<int, span<const int>> {};

// ��ͨ������ͣ�������֤��ȷ��
// ����ʹ����STL�ṩ����ͺ���std::accumulate��std::reduce
class ArraySumBasic : public ArraySum {
public:
    int operator()(span<const int> data) override {
        return accumulate(begin(data), end(data), 0);
    }
};

class ArraySumReduce : public ArraySum {
public:
    int operator()(span<const int> data) override {
        return reduce(begin(data), end(data), 0);
    }
};

// ���η����
class ArraySumDivideAndConquer : public ArraySum {
public:
    int operator()(span<const int> data) override {
        if (data.size() == 0) {
            return 0;
        } else if (data.size() == 1) {
            return data[0];
        } else {
            auto mid { data.size() / 2 };
            return (*this)(data.first(mid)) + (*this)(data.last(data.size() - mid));
        }
    }
};

vector testSize { 1, 5, 100, 10000, 1'000'000, 100'000'000 };

// ���Խ�����ʵ�ּ��뵽���Կ�ܵĲ����б���
TestFramework<ArraySum, ArraySumBasic, ArraySumReduce, ArraySumDivideAndConquer> test;

int main() {
    for (auto n : testSize) {
        cout << format("n = {}", n) << endl;
        vector<int> data(n);
        iota(begin(data), end(data), 1);
        test(data);
    }
}