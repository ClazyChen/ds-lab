#include <vector>
#include <format>
#include <iostream>
import Framework;
import Vector;
using namespace std;

// 这个实验用于展示和验证向量的基本操作
// 您可以将下面的向量替换为自己实现的向量参与测试
template <typename T>
using Vector = dslab::Vector<T>;

// 用于进行测试的元素类型
class TestItem {
public:
    int m_value { 0 };
    static int s_copyCount;
    static int s_moveCount;
    TestItem() = default;
    TestItem(int value) : m_value(value) {}
    TestItem(const TestItem& other) : m_value(other.m_value) {
        ++s_copyCount;
    }
    TestItem(TestItem&& other) : m_value(other.m_value) {
        ++s_moveCount;
    }
};

int TestItem::s_copyCount = 0;
int TestItem::s_moveCount = 0;

// 测试流程：
// 1. 创建一个空向量，要求规模=0，容量=0
// 2. 修改容量为N，要求规模=0，容量=N
// 3. 在尾部插入N个元素，要求不触发移动，规模=N，容量不变
// 4. 在随机位置插入N个元素，要求不触发复制，规模=2N
// 5. 在随机位置删除N个元素，要求不触发复制，规模=N
// 6. 查找随机的N个元素，要求不触发移动或复制，规模、容量不变
// 7. 修改规模逐次减小，要求不触发移动或复制，规模逐次减小

class VectorTest : public dslab::Algorithm<void(Vector<TestItem>&)> {
};
