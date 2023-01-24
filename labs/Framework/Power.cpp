#include <vector>
#include <format>
#include <iostream>
import Framework;
import Power;
using namespace dslab;
using namespace std;

// ��������
// ����μ�Power.ixx

// ֱ�Ӽ���ķ���
class PowerBasic : public PowerProblem {
public:
    int operator()(int a, int b) override {
        int result { 1 };
        for (int i { 0 }; i < b; ++i) {
            result *= a;
        }
        return result;
    }
};

// �����ݣ��ݹ�汾��
class PowerFastRecursive : public PowerProblem {
public:
    int operator()(int a, int b) override {
        if (b == 0) {
            return 1;
        } else if (b % 2 == 1) {
            return a * (*this)(a * a, b / 2);
        } else {
            return (*this)(a * a, b / 2);
        }
    }
};

using PowerFastIterative = Power;

// ��������
vector<pair<int, int>> testData {
    {3, 4}, {2, 30}, {-3, 3}, // һЩ��������
    {1, 1'000'000'000} // ��b�ܴ�ʱ��Ч�ʲ�������
};

// ���Խ�����ʵ�ּ��뵽���Կ�ܵĲ����б���
TestFramework<PowerProblem, PowerBasic, PowerFastRecursive, PowerFastIterative> test;

int main() {
    for (auto& [a, b] : testData) {
        cout << format("a={}, b={}", a, b) << endl;
        test(a, b);
    }
    return 0;
}