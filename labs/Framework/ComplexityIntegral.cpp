#include <vector>
#include <format>
#include <iostream>
import Framework;
import Power;
using namespace dslab;
using namespace std;

// һ��ʾ������
// ����������У����ǿ��Կ��������ĸ��Ӷ���O(n^3 * log(n))
// �벻Ҫ����O2�Ż�������������Ż���Ӱ�쵽���ǵĲ��Խ��
int f(int n) {
    int result = 0;
    for (int i { 1 }; i <= n; ++i) {
        for (int j { 1 }; j <= i; ++j)
            for (int k { 1 }; k <= j; ++k)
                for (int l { 1 }; l <= j; l *= 2)
                    result += k * l;
    }
    return result;
}

vector testData {
    5, 10, 30, 100, 250, 400, 700, 800, 900, 1000
};

int main() {
    for (auto n : testData) {
        auto [result, time] = reportTime<int>([&] { return f(n); });
        auto relative = time * 1e9 / (n * n * n * log2(n));
        cout << format("f({:>4}) = {:>12} ({:.6f}s, {:.6f}ns/n^3logn)", n, result, time, relative) << endl;
    }
}

