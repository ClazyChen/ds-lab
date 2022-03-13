#include "list.h"
#include "vector.h"
#include "random.h"
#include <cassert>
using namespace clazy_framework;

// 这个例子展示了单向链表和双向链表的区别
// 我们知道，列表可以按照动态静态、顺序循环和单向双向划分成8类
// 这一章中将会通过实验展示这三对性质的区别及各自的特征

// 单向链表因为只有一根链
// 所以空间上肯定是比双向链表小一个常数的（和sizeof(T)有关）
// 另一方面，单向链表在处理动态操作插入、删除的时候
// 需要的次数也比双向链表少

// 双向链表的优势则在于可以向反方向遍历
// 比如查找特定节点的前驱，双向链表的时间远远小于单向链表

template <typename T>
using ForwardList = clazy::List<T, clazy::ForwardListNode<T>>;

template <typename T>
using BidirectionalList = clazy::List<T>;

// 作为对照组，用展示向量
template <typename T>
using Vector = clazy::Vector<T>;

// 单向链表和双向链表
// 和之前一样，如果您想要测试自己实现的数据结构，修改上面的代码即可
// 当然，为了降低模板编程的难度，这里把单向和双向链表拆开了
// 您可以实现两个模板，分别设计单向链表和双向链表
// 而不需要像示例代码一样用一个模板统一描述

// 在这个例子中将进行的实验时，考虑连续的push_back和连续的push_front
// 测试单向链表、双向链表和向量的性能

class PushProblem : public Algorithm {
public:
	virtual void clear_n() = 0;
	virtual void push_back_n(int n) = 0;
	virtual void push_front_n(int n) = 0;
};

template <typename T>
class BatchedPushProblem : public PushProblem {
protected:
	virtual void reset() = 0;
	virtual void push_back(const T& e) = 0;
	virtual void push_front(const T& e) = 0;
public:
	virtual void clear_n() { return reset(); }
	virtual void push_back_n(int n) {
		for (int i = 0; i < n; i++) {
			push_back(i);
		}
	}
	virtual void push_front_n(int n) {
		for (int i = 0; i < n; i++) {
			push_front(i);
		}
	}
};

template <typename T, typename Container>
requires (is_base_of_v<AbstractLinearStructure<T>, Container>)
class Push : public BatchedPushProblem<T> {
protected:
	Container C;
	virtual void reset() { cout << "reset" << endl; cout << C << endl; C.clear(); }
	virtual void push_back(const T& e) { C.push_back(e); cout << C << endl; }
	virtual void push_front(const T& e) { C.push_front(e); }
public:
	virtual void push_front_n(int n) {
		if constexpr (is_base_of_v<AbstractVector<T>, Container>) {
			if (n > 100'000) {
				cout << " NO ACTION ";
				return; // 向量在大数据面前效率实在太低，所以忽略掉了
			}
		}
		BatchedPushProblem<T>::push_front_n(n);
	}
};

int main() {
	auto algorithms = generateInstances<
		PushProblem,
		//Push<int, ForwardList<int>>,
		Push<int, BidirectionalList<int>>,
		Push<int, Vector<int>>
	>();
	int testData[] { 10, 1000, 10000, 100'000, 10'000'000 };
	// 因为向量前插在最后1组数据中实在太慢，所以屏蔽了向量
	// 进行一组测试
	auto test = [&](string name, function<void(int, shared_ptr<PushProblem>)> func) {
		cout << name << endl;
		for (int n : testData) {
        	cout << "Testing n = " << n << endl;
			for (auto push : algorithms) {
				push->clear_n();
			}
			applyTest<PushProblem>(algorithms, bind(func, n, placeholders::_1));
		}
	};
	test("Push Back Test", [](int n, auto push) { push->push_back_n(n); });
	test("Push Front Test", [](int n, auto push) { push->push_front_n(n); });
    return 0;
}

