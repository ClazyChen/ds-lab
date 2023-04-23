module;
#include <functional>

export module Tree.Traverse.AbstractTreeTraverse;

import Framework.Algorithm;
import Tree.TreeNode;

export namespace dslab {

template <typename T>
class AbstractTreeTraverse : public Algorithm<void(TreeNodeConstPos<T>, std::function<void(const T&)>)> {
protected:
    TreeNodeConstPos<T> m_current { nullptr };
    void call(std::function<void(const T&)> visit, TreeNodeConstPos<T> p) {
        m_current = p;
        visit(p->data());
    }
public:
    TreeNodeConstPos<T> current() const { return m_current; }
};

}