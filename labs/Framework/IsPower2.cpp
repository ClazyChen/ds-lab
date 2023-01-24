#include <vector>
#include <format>
#include <bit>
#include <iostream>
import Framework;
using namespace dslab;
using namespace std;

// �ж��Ƿ���2���ݴ�
class IsPower2 : public Algorithm<bool, int> {};

// ֱ���ж��ķ���
class IsPower2Basic : public IsPower2 {
public:
    bool operator()(int n) override {
        return n > 0 && (n & (n - 1)) == 0;
    }
};

// ����std::has_single_bit
class IsPower2SingleBit : public IsPower2 {
public:
    bool operator()(int n) override {
        return n > 0 && has_single_bit(static_cast<unsigned>(n));
    }
};

// �ݹ�ķ���
class IsPower2Recursive : public IsPower2 {
public:
    bool operator()(int n) override {
        if (n % 2 == 1) {
            return n == 1;
        } else {
            return (*this)(n / 2);
        }
    }
};

// �����ķ���
class IsPower2Iterative : public IsPower2 {
public:
    bool operator()(int n) override {
        int m { 1 };
        while (m < n) {
            m *= 2;
        }
        return m == n;
    }
};

vector testData {
    1, 2, 4, 6, 256, 65536,
    0x80000, 0x88888, 0x08'00'00'00 // �����Ǽ��鳣������
    // ,0x7f'ff'ff'ff // �ϴ�����ݣ����ǻ�������ĵ����汾��������ѭ��
    // ,0             // ���������ݣ����ǻ�������ĵݹ�汾�������޵ݹ�
    // ,-1            // ���������ݣ����ǻ�������ĵݹ�汾�������޵ݹ�
};

// ���Խ�����ʵ�ּ��뵽���Կ�ܵĲ����б���
TestFramework<IsPower2, IsPower2Basic, IsPower2SingleBit, IsPower2Recursive, IsPower2Iterative> test;

int main() {
    for (auto n : testData) {
        cout << format("n = {:>10} : ", n) << endl;
        test(n);
    }
    return 0;
}
