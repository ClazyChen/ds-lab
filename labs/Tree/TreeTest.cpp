#include <format>
#include <iostream>

import Framework;
import Tree;
using namespace std;

using dslab::Algorithm;
using dslab::TestItem;

// 这个实验用于展示和验证树的基本操作
// 您可以将下面的树替换为自己实现的树参与测试
template <typename T>
using Tree = dslab::Tree<T>;

// 测试流程：
// 1. 创建一个空树，要求规模=0
// 2. 将一个元素插入到根节点（移动），要求不触发复制，规模=1
// 3. 将N-1个元素插入为子节点以创建链（复制），要求不触发移动，规模=N
// 4. 提取N/2个元素组成的半条链生成新的树，要求不触发复制或移动，规模=N/2
// 5. 将原树作为子树插入到新树的树根处，要求不触发复制或移动，规模=N
// 6. 将N个元素插入为子节点以创建菊花图（移动），要求不触发复制，规模=2N
// 7. 删除最右侧的链，要求不触发复制，规模=3N/2
// 8. 查找随机的N个元素，要求不触发复制或移动，规模=3N/2
// 9. 调用复制构造函数，要求不触发移动，前后相同
// 10. 调用移动构造函数，要求不触发复制或移动，前后相同

constexpr size_t N { 6 };

class CreateTree : public Algorithm<void(Tree<TestItem>&)> {
public:
    void operator()(Tree<TestItem>& T) override {
        T.clear();
        if (T.size() != 0) {
            throw runtime_error("size != 0");
        }
    }
};

class InsertAsRoot : public Algorithm<void(Tree<TestItem>&)> {
public:
    void operator()(Tree<TestItem>& T) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        T.insertAsRoot(TestItem { 0 });
        cout << "insert as root 0 -> " << T << endl;
        if (T.size() != 1) {
            throw runtime_error("size != 1");
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (TestItem::s_moveCount != 1) {
            throw runtime_error("moveCount != 1");
        }
    }
};

class CreateChain : public Algorithm<void(Tree<TestItem>&)> {
public:
    void operator()(Tree<TestItem>& T) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        auto pos { T.root() };
        TestItem tmp { 0 };
        for (size_t i { 1 }; i < N; ++i) {
            tmp.m_value = i;
            pos = T.insertAsChild(pos, tmp);
            cout << format("insert as child {} -> ", i) << T << endl;
        }
        if (T.size() != N) {
            throw runtime_error("size != N");
        }
        if (TestItem::s_copyCount != N - 1) {
            throw runtime_error("copyCount != N - 1");
        }
        if (TestItem::s_moveCount != 0) {
            throw runtime_error("moveCount != 0");
        }
    }
};

Tree<TestItem> subTree;

class CreateSubTree : public Algorithm<void(Tree<TestItem>&)> {
public:
    void operator()(Tree<TestItem>& T) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        subTree.clear();
        auto pos { T.root() };
        for (size_t i { 1 }; i < N / 2; ++i) {
            pos = pos->child(0);
        }
        subTree = T.detach(pos->child(0));
        cout << "detached tree -> " << T << endl;
        cout << "     sub tree -> " << subTree << endl;
        if (T.size() != N / 2) {
            throw runtime_error("size != N / 2");
        }
        if (subTree.size() != N / 2) {
            throw runtime_error("subTree.size() != N / 2");
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (TestItem::s_moveCount != 0) {
            throw runtime_error("moveCount != 0");
        }
    }
};

class AttachSubTree : public Algorithm<void(Tree<TestItem>&)> {
public:
    void operator()(Tree<TestItem>& T) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        T.attachAsChild(T.root(), subTree);
        cout << "attach sub tree -> " << T << endl;
        if (T.size() != N) {
            throw runtime_error("size != N");
        }
        if (subTree.size() != 0) {
            throw runtime_error("subTree.size() != 0");
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (TestItem::s_moveCount != 0) {
            throw runtime_error("moveCount != 0");
        }
    }
};

class CreateFlower : public Algorithm<void(Tree<TestItem>&)> {
public:
    void operator()(Tree<TestItem>& T) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        for (size_t i { 0 }; i < N; ++i) {
            T.insertAsChild(T.root(), TestItem { i + N }, 0);
            cout << format("insert as child {} -> ", i + N) << T << endl;
        }
        if (T.size() != 2 * N) {
            throw runtime_error("size != 2 * N");
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (TestItem::s_moveCount != N) {
            throw runtime_error("moveCount != N");
        }
    }
};

class RemoveChain : public Algorithm<void(Tree<TestItem>&)> {
public:
    void operator()(Tree<TestItem>& T) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        auto e { T.remove(T.root()->children().back()) };
        cout << format("remove @ {} -> ", e.m_value) << T << endl;
        if (T.size() != 3 * N / 2) {
            throw runtime_error("size != 3 * N / 2");
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (TestItem::s_moveCount != 1) {
            throw runtime_error("moveCount != 1");
        }
    }
};

class FindTree : public Algorithm<void(Tree<TestItem>&)> {
public:
    void operator()(Tree<TestItem>& T) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        TestItem tmp { 0 };
        for (size_t i { 0 }; i < N; ++i) {
            tmp.m_value = rand() % (2 * N);
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
                cout << format("find {} -> ", tmp.m_value) << *pos << endl;
            } else {
                cout << format("find {} -> nullptr", tmp.m_value) << endl;
            }
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (TestItem::s_moveCount != 0) {
            throw runtime_error("moveCount != 0");
        }
        if (T.size() != 3 * N / 2) {
            throw runtime_error("size != 3 * N / 2");
        }
    }
};

class CopyTree : public Algorithm<void(Tree<TestItem>&)> {
public:
    void operator()(Tree<TestItem>& T) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        auto T2 { T };
        cout << "copy tree -> " << T2 << endl;
        if (T2.size() != T.size()) {
            throw runtime_error("size != T.size()");
        }
        if (TestItem::s_copyCount != T.size()) {
            throw runtime_error("copyCount != T.size()");
        }
        if (TestItem::s_moveCount != 0) {
            throw runtime_error("moveCount != 0");
        }
        if (T2.root() == T.root()) {
            throw runtime_error("T2.root() == T.root()");
        }
    }
};

class MoveTree : public Algorithm<void(Tree<TestItem>&)> {
public:
    void operator()(Tree<TestItem>& T) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        auto T2 { move(T) };
        cout << "move tree -> " << T2 << endl;
        if (T2.size() != 3 * N / 2) {
            throw runtime_error("size != 3 * N / 2");
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (TestItem::s_moveCount != 0) {
            throw runtime_error("moveCount != 0");
        }
        if (T.root() != nullptr) {
            throw runtime_error("T.root() != nullptr");
        }
    }
};

dslab::StructureTestFramework<
    Tree, TestItem,
    CreateTree,
    InsertAsRoot,
    CreateChain,
    CreateSubTree,
    AttachSubTree,
    CreateFlower,
    RemoveChain,
    FindTree,
    CopyTree,
    MoveTree
> test;

int main() {
    srand(time(nullptr));
    Tree<TestItem> T {};
    test.initialize();
    test(T);
    return 0;
}