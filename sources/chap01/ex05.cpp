#include <iostream>
#include <vector>
using namespace std;

// 这个例子展示角谷猜想
// 输入：正整数x
// 输出：对x反复进行操作：如果是奇数，乘3再加1；如果是偶数，除以2。直到x变为1为止。
// 目前的猜想是，任何整数都会通过若干次操作变为1，然后重复1、4、2的循环。

// 因此，角谷猜想的验证程序，是否满足有限性是一个未解决的问题
// 从而在严格的定义下，这不能够成为一个算法

class Collatz {
public:
    vector<int> generate(int x) {
        vector<int> result {x};
        while (x > 1) {           // 执行到x变为1为止
            if (x % 2 == 1) {
                result.push_back(x = 3*x + 1);
            } else /* x % 2 == 0 */ {
                result.push_back(x = x / 2);
            }
        }
        return result;
    }
};

Collatz collatz;

// 在这个例子中，您可以自己输入一个测试数据
// 您也可以仿照之前几个例子，改写成自动选取一些节点测试的形式
int main() {
    int x; 
    // 对于int范围内的数据角谷猜想均正确
    // 但对于较大的数，进行3x+1操作可能会出现溢出
    cin >> x;      
    auto seq = collatz.generate(x);
    cout << "sequence: " << endl;
    for (int y : seq) {
        cout << y << endl;
    }
    return 0;
}
