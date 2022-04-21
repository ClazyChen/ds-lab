#include "vector.h"
#include "random.h"
using namespace clazy_framework;

// 这个例子展示了向量的插入、删除和查找操作
// 对于一个数据结构来说，这三种操作是最基本的

// 如果您想要测试自己实现的数据结构，修改下面的代码
// 比如改为 using Vector = UserVector<T> 这样就可以了

template <typename T>
using Vector = clazy::Vector<T>;

const int start_size = 4;       // 向量的初始长度
const int op_count = 20;        // 连续操作序列的长度
const double insert_cdf = 0.5;  // 在连续操作序列中“插入”的占比
const double remove_cdf = 0.7;  // 在连续操作序列中“插入”和“删除”的总占比
const double find_cdf   = 1.0;  // 这里使用累计概率函数

Vector<int> V;                  // 用于测试的向量（我们自己实现的）
vector<int> v;                  // 用于对比的向量（STL）

int e = 0;                      // 用于向向量中加入元素

void check() {
    assert(V.size() == v.size());
    for (int i = 0; i < v.size(); i++) {
        assert(V[i] == v[i]);
    }
}

void insert() {
    Rank r = Random::nextIntBetween(0, V.size()+1);
    V.insert(r, e);
    v.insert(begin(v) + r, e); // STL 向量的插入
    cout << "insert(" << e++ << " @ rank " << r << ")" << endl;
    cout << "\t V = " << V << endl;
}

void remove() {
    Rank r = Random::nextIntBetween(0, V.size());
    int Vx = V.remove(r);
    v.erase(begin(v) + r); // STL 向量的删除
    cout << "remove(" << Vx << " @ rank " << r << ")" << endl;
    cout << "\t V = " << V << endl;
}

void find() {
    int x = Random::nextIntBetween(0, e);
    Rank Vr = V.find(x);
    Rank vr = find(begin(v), end(v), x) - begin(v);     // STL 向量的查找
    assert((Vr == -1 && vr == V.size()) || (Vr == vr)); // STL 查找失败时会返回end(v)，和我们的语义不一样
    cout << "find(" << x << ") = " << Vr << endl;
}

int main() {
    while (e < start_size) {
        insert();                        // 实验开始，首先进行一些插入
    }
    check();
    for (int i = 0; i < op_count; i++) {
        double p = Random::nextDouble();
        if (p < insert_cdf) {
            insert();
        } else if (p < remove_cdf && V.size() > 0) {
            remove();
        } else {
            find();
        }
        check();
    }
    return 0;
}
