#include "list.h"
#include "random.h" 
using namespace clazy_framework;

// 这个例子展示静态链表和动态链表的区别
// 它们的主要区别在于内存池，动态链表使用的是OS的内存池，而静态链表从OS申请一块大的内存空间，然后手动进行分配
// 静态链表在插入时的效率（不考虑扩容）是高于动态链表的
// 而在删除时，由于额外需要维护一个内存池，效率比较低

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
	virtual void init(int maxn, T dummy) override {
		applyInsert(maxn, dummy);
		C.clear();
	}
	virtual void applyInsert(int n, T dummy) override {
		for (int i : views::iota(0, n)) {
			C.push_back(dummy);
		}
	}
	virtual void shufflePosition() override {
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
	virtual void applyRemove() override {
		for (auto p : pos) {
			C.remove(p);
		}
	}
	virtual string getTypename() const override {
		return C.getTypename();
	}
};

int main() {
	auto algorithms = generateInstances<
		TestProblem<int>,
		TestInstance<int, clazy::ListNodePos<int>, clazy::ListNode<int>, DynamicList<int>>,
		TestInstance<int, Rank, clazy::StaticListNode<int>, StaticList<int>>
	>();
    vector<int> testData { 10, 10000, 1'000'000, 10'000'000, 20'000'000 };
	int maxn = *max_element(begin(testData), end(testData));
	cout << "Initializing" << endl;
	for (auto algorithm : algorithms) { // 初始化，提前让静态链表完成扩容
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
