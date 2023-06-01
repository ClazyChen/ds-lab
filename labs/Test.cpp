import std;
import Framework;

using namespace dslab;
using namespace std;

// 从1加到N的结果
class Sum : public Object {};

vector testData {
    10, // 都能输出正确的值
    50'000, // AP溢出，AP2正确
    70'000, // 全部溢出，但AP2Limited能返回饱和值
    -8 // 负输入，AP2Limited能返回0
};

int main() {
    for (auto data : testData) {
        cout << format("Input: {}", data) << endl;
    }
    Sum a;
    cout << a.type_name() << endl;
    return 0;
}