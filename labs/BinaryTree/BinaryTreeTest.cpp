import Framework;
import BinaryTree;
import Vector;
import std;

using namespace std;

using dslab::Algorithm;
using dslab::TestItem;

// 这个实验用于展示和验证二叉树的基本操作
// 您可以将下面的二叉树替换为自己实现的二叉树参与测试
template <typename T>
using BinaryTree = dslab::BinaryTree<T>;

// 测试流程：
// 1. 创建一个空树，要求规模=0
// 2. 将一个元素插入到根节点（移动），要求不触发复制，规模=1
// 3. 将N-1个元素插入为左子代以创建链（复制），要求不触发移动，规模=N
// 4. 验证先序序列 = [0:N]
// 5. 验证中序序列 = [0:N:-1]
// 6. 验证后序序列 = [0:N:-1]
// 7. 提取N/2个元素组成的半条链生成新的树，要求不触发复制或移动，规模=N/2
// 8. 查找随机的N个元素，要求不触发复制或移动，规模=N/2
// 9. 将新树作为子树插入到原树的右子代，要求不触发复制或移动，规模=N
// 10. 验证层次序列 = [0, 1, N/2, 2, N/2+1, ...]
// 11. 删除树根的左子代，要求不触发复制，规模=N/2+1
// 12. 调用复制构造函数，要求不触发移动，前后相同
// 13. 调用移动构造函数，要求不触发复制或移动，前后相同

constexpr size_t N { 6 };

class CreateTree : public Algorithm<void(BinaryTree<TestItem>&)> {
public:
    void operator()(BinaryTree<TestItem>& T) override {
        T.clear();
        if (T.size() != 0) {
            throw runtime_error("size() != 0");
        }
    }
    string type_name() const override {
        return "Create Binary Tree";
    }
};

class InsertAsRoot : public Algorithm<void(BinaryTree<TestItem>&)> {
public:
    void operator()(BinaryTree<TestItem>& T) override {
        TestItem::s_copyCount = 0; 
        TestItem::s_moveCount = 0;
        T.insertAsRoot(TestItem { 0 });
        cout << "insert as root 0 -> " << T << endl;
        if (T.size() != 1) {
            throw runtime_error("size() != 1");
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copy count != 0");
        }
        if (TestItem::s_moveCount != 1) {
            throw runtime_error("move count != 1");
        }
    }
    string type_name() const override {
        return "Insert as Root Binary Tree";
    }
};

class CreateLeftChain : public Algorithm<void(BinaryTree<TestItem>&)> {
public:
void operator()(BinaryTree<TestItem>& T) override {
        TestItem::s_copyCount = 0; 
        TestItem::s_moveCount = 0;
        auto pos { T.root() };
        TestItem tmp { 0 };
        for (size_t i = 1; i < N; ++i) {
            tmp.m_value = i;
            pos = T.insertAsLeftChild(pos, tmp);
            cout << format("insert as left child {} ->  ", i) << T << endl;
        }
        if (T.size() != N) {
            throw runtime_error("size() != N");
        }
        if (TestItem::s_copyCount != N - 1) {
            throw runtime_error("copy count != N - 1");
        }
        if (TestItem::s_moveCount != 0) {
            throw runtime_error("move count != 0");
        }
    }
    string type_name() const override {
        return "Create Left Chain Binary Tree";
    }
};

template <template <typename> typename... Traverse>
class PreOrderTraverse : public Algorithm<void(BinaryTree<TestItem>&)> {
    template <template <typename> typename Trav>
    void traverse(BinaryTree<TestItem>& T) {
        dslab::Vector<size_t> V {};
        T.traverse<Trav>([&](const TestItem& item) {
            V.push_back(item.m_value);
        });
        cout << " -> " << V << "  // " << (Trav<TestItem>()).type_name() << endl;
        for (size_t i { 0 }; i < N; ++i) {
            if (V[i] != i) {
                throw runtime_error("pre order traverse error");
            }
        }
    }
public:
    void operator()(BinaryTree<TestItem>& T) override {
        (traverse<Traverse>(T), ...);
    }
    string type_name() const override {
        return "Pre Order Traverse Binary Tree";
    }
};

template <template <typename> typename... Traverse>
class InOrderTraverse : public Algorithm<void(BinaryTree<TestItem>&)> {
    template <template <typename> typename Trav>
    void traverse(BinaryTree<TestItem>& T) {
        dslab::Vector<size_t> V {};
        T.traverse<Trav>([&](const TestItem& item) {
            V.push_back(item.m_value);
        });
        cout << " -> " << V << "  // " << (Trav<TestItem>()).type_name() << endl;
        for (size_t i { 0 }; i < N; ++i) {
            if (V[i] != N - i - 1) {
                throw runtime_error("in order traverse error");
            }
        }
    }
public:
    void operator()(BinaryTree<TestItem>& T) override {
        (traverse<Traverse>(T), ...);
    }
    string type_name() const override {
        return "In Order Traverse Binary Tree";
    }
};

template <template <typename> typename... Traverse>
class PostOrderTraverse : public Algorithm<void(BinaryTree<TestItem>&)> {
    template <template <typename> typename Trav>
    void traverse(BinaryTree<TestItem>& T) {
        dslab::Vector<size_t> V {};
        T.traverse<Trav>([&](const TestItem& item) {
            V.push_back(item.m_value);
        });
        cout << " -> " << V << "  // " << (Trav<TestItem>()).type_name() << endl;
        for (size_t i { 0 }; i < N; ++i) {
            if (V[i] != N - i - 1) {
                throw runtime_error("post order traverse error");
            }
        }
    }
public:
    void operator()(BinaryTree<TestItem>& T) override {
        (traverse<Traverse>(T), ...);
    }
    string type_name() const override {
        return "Post Order Traverse Binary Tree";
    }
};

BinaryTree<TestItem> subTree;

class CreateSubTree : public Algorithm<void(BinaryTree<TestItem>&)> {
public:
void operator()(BinaryTree<TestItem>& T) override {
        TestItem::s_copyCount = 0; 
        TestItem::s_moveCount = 0;
        auto pos { T.root() };
        for (size_t i { 0 }; i < N / 2; ++i) {
            pos = pos->left();
        }
        subTree = T.detach(pos);
        cout << "detached tree -> " << T << endl;
        cout << "     sub tree -> " << subTree << endl;
        if (T.size() != N / 2) {
            throw runtime_error("size() != N / 2");
        }
        if (subTree.size() != N / 2) {
            throw runtime_error("subTree.size() != N / 2");
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copy count != 0");
        }
        if (TestItem::s_moveCount != 0) {
            throw runtime_error("move count != 0");
        }
    }
    string type_name() const override {
        return "Create Sub Binary Tree";
    }
};

class FindTree : public Algorithm<void(BinaryTree<TestItem>&)> {
public:
    void operator()(BinaryTree<TestItem>& T) override {
        TestItem::s_copyCount = 0; 
        TestItem::s_moveCount = 0;
        TestItem tmp { 0 };
        for (size_t i { 0 }; i < N; ++i) {
            tmp.m_value = rand() % N;
            auto pos { T.find(tmp) };
            if (tmp.m_value < N / 2 || tmp.m_value >= N) {
                if (pos->data().m_value != tmp.m_value) {
                    throw runtime_error("pos->data() != tmp");
                }
            } else {
                if (pos != nullptr) {
                    throw runtime_error("pos != nullptr");
                }
            }
            if (pos != nullptr) {
                cout << format("find {} -> ", tmp.m_value) << pos->data() << endl;
            } else {
                cout << format("find {} -> nullptr", tmp.m_value) << endl;
            }
        }
        if (T.size() != N / 2) {
            throw runtime_error("size() != N / 2");
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copy count != 0");
        }
        if (TestItem::s_moveCount != 0) {
            throw runtime_error("move count != 0");
        }
    }
    string type_name() const override {
        return "Find Binary Tree";
    }
};

class AttachSubTree : public Algorithm<void(BinaryTree<TestItem>&)> {
public:
    void operator()(BinaryTree<TestItem>& T) override {
        TestItem::s_copyCount = 0; 
        TestItem::s_moveCount = 0;
        T.attachAsRightChild(T.root(), subTree);
        cout << "attach sub tree -> " << T << endl;
        if (T.size() != N) {
            throw runtime_error("size() != N");
        }
        if (subTree.size() != 0) {
            throw runtime_error("subTree.size() != 0");
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copy count != 0");
        }
        if (TestItem::s_moveCount != 0) {
            throw runtime_error("move count != 0");
        }
    }
    string type_name() const override {
        return "Attach Sub Binary Tree";
    }
};

class LevelOrderTraverse : public Algorithm<void(BinaryTree<TestItem>&)> {
public:
void operator()(BinaryTree<TestItem>& T) override {
        dslab::Vector<size_t> V {};
        T.traverse<dslab::BinaryTreeLevelOrderTraverse>([&](const TestItem& item) {
            V.push_back(item.m_value);
        });
        cout << " -> " << V << "  // " << (dslab::BinaryTreeLevelOrderTraverse<TestItem>()).type_name() << endl;
        for (size_t i { 0 }; i < N; ++i) {
            size_t should { 0 };
            if (i > 0) {
                if (i % 2 == 0) {
                    should = i / 2 + N / 2 - 1;
                } else {
                    should = i / 2 + 1;
                    if (i == N - 1) {
                        should = N - 1;
                    }
                }
            }
            if (V[i] != should) {
                throw runtime_error("level order traverse error");
            }
        }
    }
    string type_name() const override {
        return "Level Order Traverse Binary Tree";
    }
};

class RemoveLeftChild : public Algorithm<void(BinaryTree<TestItem>&)> {
public:
void operator()(BinaryTree<TestItem>& T) override {
        TestItem::s_copyCount = 0; 
        TestItem::s_moveCount = 0;
        auto pos { T.root() };
        while (pos->left() != nullptr) {
            pos = pos->left();
        }
        do {
            pos = pos->parent();
            auto data { T.remove(pos->left()) };
            cout << format("remove @ {} -> ", data.m_value) << T << endl;
        } while (pos != T.root());
        if (T.size() != N / 2 + 1) {
            throw runtime_error("size() != N / 2 + 1");
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copy count != 0");
        }
        if (TestItem::s_moveCount != N / 2 - 1) {
            throw runtime_error("move count != N / 2 - 1");
        }
    }
    string type_name() const override {
        return "Remove Binary Tree";
    }
};

class CopyTree : public Algorithm<void(BinaryTree<TestItem>&)> {
public:
void operator()(BinaryTree<TestItem>& T) override {
        TestItem::s_copyCount = 0; 
        TestItem::s_moveCount = 0;
        auto T2 { T };
        cout << "copy tree -> " << T2 << endl;
        if (T2.size() != T.size()) {
            throw runtime_error("size() != T.size()");
        }
        if (TestItem::s_copyCount != T.size()) {
            throw runtime_error("copy count != T.size()");
        }
        if (TestItem::s_moveCount != 0) {
            throw runtime_error("move count != 0");
        }
        if (T2.root() == T.root()) {
            throw runtime_error("T2.root() == T.root()");
        }
    }
    string type_name() const override {
        return "Copy Binary Tree";
    }
};

class MoveTree : public Algorithm<void(BinaryTree<TestItem>&)> {
public:
    void operator()(BinaryTree<TestItem>& T) override {
        TestItem::s_copyCount = 0; 
        TestItem::s_moveCount = 0;
        auto T2 { move(T) };
        cout << "move tree -> " << T2 << endl;
        if (T2.size() != N / 2 + 1) {
            throw runtime_error("size() != N / 2 + 1");
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copy count != 0");
        }
        if (TestItem::s_moveCount != 0) {
            throw runtime_error("move count != 0");
        }
        if (T.root() != nullptr) {
            throw runtime_error("T.root() != nullptr");
        }
    }
    string type_name() const override {
        return "Move Binary Tree";
    }
};

dslab::StructureTestFramework<
    BinaryTree, TestItem,
    CreateTree,
    InsertAsRoot,
    CreateLeftChain,
    PreOrderTraverse<dslab::BinaryTreePreOrderTraverse, 
                     dslab::BinaryTreePreOrderTraverseSemilinear,
                     dslab::BinaryTreePreOrderTraverseLinear,
                     dslab::BinaryTreePreOrderTraverseLinearRight,
                     dslab::BinaryTreePreOrderTraverseLinearRecursive,
                     dslab::BinaryTreePreOrderIterativeTraverse>,
    InOrderTraverse<dslab::BinaryTreeInOrderTraverse,
                    dslab::BinaryTreeInOrderTraverseLinear,
                    dslab::BinaryTreeInOrderIterativeTraverse>,
    PostOrderTraverse<dslab::BinaryTreePostOrderTraverse,
                      dslab::BinaryTreePostOrderTraverseSemilinear,
                      dslab::BinaryTreePostOrderTraverseLinear,
                      dslab::BinaryTreePostOrderIterativeTraverse>,
    CreateSubTree,
    FindTree,
    AttachSubTree,
    LevelOrderTraverse,
    RemoveLeftChild,
    CopyTree,
    MoveTree
> test;

int main() {
    srand(time(nullptr));
    BinaryTree<TestItem> T {};
    test.initialize();
    test(T);
    return 0;
}