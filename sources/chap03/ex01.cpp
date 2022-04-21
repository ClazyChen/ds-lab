#include "list.h"
#include "list_forward.h"
#include "vector.h"
#include "test_framework.h"
using namespace clazy_framework;

// 这个例子展示了单向链表和双向链表的区别

// 单向链表因为只有一根链，所以空间上肯定是比双向链表小一个常数的
// 另一方面，单向链表在处理动态操作插入、删除的时候，需要的次数也比双向链表少

// 双向链表的优势则在于可以向反方向遍历
// 比如查找特定节点的前驱，双向链表的时间远远小于单向链表

// 在这个例子将比较单向链表、双向链表和向量三个数据结构，其中向量作为参照组
// 可以看出单向链表的效率高于双向链表，而向量和它们有很大的差距
// 对于push_back（尾部插入），向量的效率远远高于列表（常数级）
// 对于push_front（头部插入），向量的效率远远低于列表（复杂度级）

template <typename T>
using ForwardList = clazy::ForwardList<T>;

template <typename T>
using BidirectionalList = clazy::List<T>;

template <typename T>
using Vector = clazy::Vector<T>;

// 实验包括两个部分：
// 连续进行后插和连续进行前插

class PushProblem : public Algorithm<void, int> {

};

template <typename Container>
class PushBack : public PushProblem {
private:
	Container C;
public:
	void clear() override {
		C.clear();
	}

	void apply(int n) override {
		for (int i = 0; i < n; i++) {
			C.push_back(i);
		}	
	}
};

template <typename Container>
class PushFront : public PushProblem {
private:
	Container C;
public:
	void clear() override {
		C.clear();
	}

	void apply(int n) override {
		if constexpr (is_same_v<Vector<int>, Container>) {
			if (n > 100'000) {
				cout << " NO ACTION ";
				return; // 向量在超大规模的数据下，前插需要过多的时间，所以短路返回了
			}
		}
		for (int i = 0; i < n; i++) {
			C.push_front(i);
		}
	}
};

// 测试数据的规模
int testData[] { 10, 1000, 10000, 100'000, 1'000'000, 10'000'000, 50'000'000 };

int main() {
	TestFramework<PushProblem, PushBack<ForwardList<int>>, PushBack<BidirectionalList<int>>, PushBack<Vector<int>>> back_tf;
	TestFramework<PushProblem, PushFront<ForwardList<int>>, PushFront<BidirectionalList<int>>, PushFront<Vector<int>>> front_tf;
	cout << "Push Back Test" << endl;
	for (int n : testData) {
		cout << "Testing n = " << n << endl;
		back_tf.test(n);
		back_tf.clear();
	}
	cout << "Push Front Test" << endl;
	for (int n : testData) {
		cout << "Testing n = " << n << endl;
		front_tf.test(n);
		front_tf.clear();
	}
    return 0;
}

// 这里需要注意的是，链表在实验表现出的性能并不等同于实际环境中可以达到的性能
// 这一点将在后面的实验中展示出来（主要影响因素是内存连续性）
// 可以看出，单向链表的效率是比双向高的，而双向链表的优势在于：
// 1. 向前驱方向查找，单向链表无法完成
// 2. 方便进行尾部操作，比如可以轻松实现“末尾元素的前插”，而单向链表要实现需要额外维护一个指针
// 3. 前插是安全的（单向链表的前插不安全，会导致逻辑上指向pos的指针，在前插之后指向了被插入的节点pos->pred）
// 4. 删除是安全的（同上）
