module;
#include <functional>

export module Tree.Traverse.TreePreOrderTraverse;

import Tree.TreeNode;
import Tree.Traverse.AbstractTreeTraverse;
import Stack;

export namespace dslab {

template <typename T>
class TreePreOrderTraverse : public AbstractTreeTraverse<T> {
public:
    void operator()(TreeNodeConstPos<T> p, std::function<void(const T&)> visit) const override {
        if (!p) {
            return;
        }
        visit(p->data());
        for (auto& child : p->children()) {
            operator()(child, visit);
        }
    }
};

template <typename T>
class TreePreOrderTraverseIterative : public AbstractTreeTraverse<T> {
public:
    void operator()(TreeNodeConstPos<T> p, std::function<void(const T&)> visit) const override {
        if (!p) {
            return;
        }
        Stack<TreeNodeConstPos<T>> S;
        S.push(p);
        while (!S.empty()) {
            auto node { S.pop() };
            visit(node->data());
            for (auto it { node->children().rbegin() }; it != node->children().rend(); ++it) {
                S.push(*it);
            }
        }
    }
};

}