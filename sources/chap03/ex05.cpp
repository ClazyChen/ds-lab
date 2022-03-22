#include "list.h"
#include "random.h"
using namespace clazy_framework;

// 这个例子展示静态链表和动态链表的区别
// 它们的主要区别在于内存池，动态链表使用的是OS的内存池，而静态链表一次分配较大的内存空间然后手动进行分配

// 这里将展示两个方面：
// 其一，是连续插入的情况，这里不考虑向量的扩容时间
// 其二，是连续删除的情况，这里删除的次序是随机的，从而在动态链表的场合会遇到较大的麻烦

// 动态链表和静态链表
template <typename T>
using DynamicList = clazy::List<T>;

template <typename T>
using StaticList = clazy::StaticList<T>;

template <typename T>
using Vector = clazy::Vector<T>;

template <typename T>
class TestProblem : public Algorithm {
public:
	virtual void init(int maxn, T dummy) = 0; // 初始化，在静态的场合，这里会把扩容时间解决掉
	virtual void applyInsert(int n, T dummy) = 0; // 执行连续的随机插入
	virtual void shufflePosition() = 0; // 生成随机删除的次序
	virtual void applyRemove() = 0; // 执行连续的随机删除直到空
};

template <typename T, typename P, typename Node, typename Container>
requires (is_base_of_v<AbstractList<T, P, Node>, Container>)
class TestInstance : public TestProblem<T> {
protected:
	Container C;
	Vector<P> pos;
public:
	virtual void init(int maxn, T dummy) {
		applyInsert(maxn, dummy);
		C.clear();
	}
	virtual void applyInsert(int n, T dummy) {
		for (int i : views::iota(0, n)) {
			C.push_back(dummy);
		}
	}
	virtual void shufflePosition() {
		pos.clear();
		if constexpr (is_same_v<Rank, P>) { // 静态链表
			for (int i : views::iota(0, C.size()) | views::reverse) {
				pos.push_back(Random::nextIntBetween(0, i+1));
			}
		} else { // 动态链表
			for (auto it = begin(C); it != end(C); it++) {
				pos.push_back(it.base());
			}
			random_shuffle(begin(pos), end(pos));
		}
	}
	virtual void applyRemove() {
		for (auto p : pos) {
			C.remove(p);
		}
	}
};

int main() {
	auto algorithms = generateInstances<
		TestProblem<int>,
		//TestInstance<int, clazy::ListNodePos<int>, clazy::ListNode<int>, DynamicList<int>>,
		TestInstance<int, Rank, clazy::StaticListNode<int>, StaticList<int>>
	>();
    vector<int> testData { 10, 10000, 1'000'000, 10'000'000, 20'000'000 };
	int maxn = *max_element(begin(testData), end(testData));
	cout << "initializing..." << endl;
	for (auto algorithm : algorithms) {
		algorithm->init(maxn, 0);
	}
	for (int n : testData) {
		cout << "Testing n = " << n << endl;
		cout << "Testing insert" << endl;
		applyTest<TestProblem<int>>(algorithms, [n](auto algorithm) {
			algorithm->applyInsert(n, 0);
		});
		for (auto algorithm : algorithms) {
			algorithm->shufflePosition();
		}
		cout << "Testing remove" << endl;
		applyTest<TestProblem<int>>(algorithms, [](auto algorithm) {
			algorithm->applyRemove();
		});
	}
	return 0;
} 
