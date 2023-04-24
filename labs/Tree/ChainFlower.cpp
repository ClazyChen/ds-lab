#include <vector>
#include <format>
#include <iostream>

import Framework;
import Tree;
import Tree.Traverse.AbstractTreeTraverse;
using namespace dslab;
using namespace std;

template <typename T>
class TreeTraverseTest : public Algorithm<T()> {
protected:
    Tree<T> m_tree {};
public:
    void initializeChain(size_t n) {
        m_tree = Tree<T> { 0 };
        auto pos { m_tree.root() };
        for (size_t i { 0 }; i < n; ++i) {
            pos = m_tree.insertAsChild(pos, static_cast<T>(i));
        }
    }
    void initializeFlower(size_t n) {
        m_tree = Tree<T> { 0 };
        for (size_t i { 0 }; i < n; ++i) {
            m_tree.insertAsChild(m_tree.root(), static_cast<T>(i));
        }
    }
};

int h { 0 };

template <typename T, template <typename> typename Trav>
    requires is_base_of_v<AbstractTreeTraverse<T>, Trav<T>>
class TreeTraverseTestImpl : public TreeTraverseTest<T> {
    Trav<T> traverse {};
public:
    T operator()() override {
        if constexpr (is_same_v<TreePreOrderTraverse<T>, Trav<T>> || is_same_v<TreePostOrderTraverse<T>, Trav<T>>) {
            if (h > 1000) {
                throw runtime_error("stack overflow");
            }
        }
        T sum { 0 };
        traverse(this->m_tree.root(), [&sum](const T& e) {
            sum += e;
        });
        return sum;
    }
    string type_name() const override {
        return traverse.type_name();
    }
};

vector testData { 10, 1000, 10000, 100'000, 1'000'000, 10'000'000 };

TestFramework<TreeTraverseTest<int>,
    TreeTraverseTestImpl<int, TreePreOrderTraverse>,
    TreeTraverseTestImpl<int, TreePreOrderTraverseIterative>,
    TreeTraverseTestImpl<int, TreePostOrderTraverse>,
    TreeTraverseTestImpl<int, TreeLevelOrderTraverse>
> test;

int main() {
    for (auto n : testData) {
        cout << format("n = {}", n) << endl;
        cout << "Chain  test ..." << endl;
        test.run([n](TreeTraverseTest<int>& t) {
            t.initializeChain(n);
        });
        h = n - 1;
        test();
        cout << "Flower test ..." << endl;
        test.run([n](TreeTraverseTest<int>& t) {
            t.initializeFlower(n);
        });
        h = 1;
        test();
    }
}