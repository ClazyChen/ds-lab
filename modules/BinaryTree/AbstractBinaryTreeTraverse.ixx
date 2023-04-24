module;
#include <functional>

export module BinaryTree.Traverse.AbstractBinaryTreeTraverse;

import Framework.Algorithm;
import BinaryTree.BinaryTreeNode;

export namespace dslab {

template <typename T>
class AbstractBinaryTreeTraverse : public Algorithm<void(BinaryTreeNodeConstPos<T>, std::function<void(const T&)>)> {
protected:
    BinaryTreeNodeConstPos<T> m_current { nullptr };
    void call(std::function<void(const T&)> visit, BinaryTreeNodeConstPos<T> p) {
        m_current = p;
        visit(p->data());
    }
public:
    BinaryTreeNodeConstPos<T> current() const { return m_current; }
};

}