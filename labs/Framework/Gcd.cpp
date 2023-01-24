#include <vector>
#include <format>
#include <iostream>
import Framework;
import Gcd;
using namespace dslab;
using namespace std;

// ���Լ������
// ����μ�Gcd.ixx

// ����ö�ٷ�
class GcdBasic : public GcdProblem {
public:
    int operator()(int a, int b) override {
        int gcd { 0 };
        for (int i { 1 }; i <= a && i <= b; ++i) {
            if (a % i == 0 && b % i == 0) {
                gcd = i;
            }
        }
        return gcd;
    }
};

// �ݹ��ŷ������㷨
// �ڱ���֮�������ᱻ�Ż��ɵ�������ʽ
// �������ݽṹ�У����Ǳ����ݹ���ʽ���з���
class GcdEuclid : public GcdProblem {
public:
    int operator()(int a, int b) override {
        if (b == 0) {
            return a;
        } else {
            return (*this)(b, a % b);
        }
    }
};

// ��Gcd.ixx����һ������ʵ��
using GcdEuclidIterative = Gcd;

vector<pair<int, int>> testData {
    {1, 1}, {1, 6},
    { 100'000'008, 12'345'678 },
    {24, 18}, {720, 1024},
    {0, 0},   // δ������Ϊ
    {0, 6},   // δ������Ϊ
    {-8, 6},  // δ������Ϊ
    {6, -8},  // δ������Ϊ
    {-8, -8}, // δ������Ϊ
    {-8, 4},  // δ������Ϊ
    {8, -6}   // δ������Ϊ
};

// ����������㷨��ʱ�򣬼ٶ���������a��b����������
// ���Ե��������븺������0ʱ��ʵ������δ������Ϊ
// ��ʱ���صĽ�����޷����Ƶģ�����һ��һ��������£�����ķ����ǲ�ȷ����
// �����׷����õ��Ƚ��ԣ�����Ϊ������������²������Ķ���

// ���Խ�����ʵ�ּ��뵽���Կ�ܵĲ����б���
TestFramework<GcdProblem, GcdBasic, GcdEuclid, GcdEuclidIterative> test;

int main() {
    for (auto& [a, b] : testData) {
        cout << format("Gcd({}, {})", a, b) << endl;
        test(a, b);
    }
}