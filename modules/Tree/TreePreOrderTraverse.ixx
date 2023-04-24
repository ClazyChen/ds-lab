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
    void operator()(TreeNodeConstPos<T> p, std::function<void(const T&)> visit) override {
        if (!p) {
            return;
        }
        this->call(visit, p);
        for (auto& child : p->children()) {
            operator()(child, visit);
        }
    }
};

template <typename T>
class TreePreOrderTraverseIterative : public AbstractTreeTraverse<T> {
public:
    void operator()(TreeNodeConstPos<T> p, std::function<void(const T&)> visit) override {
        if (!p) {
            return;
        }
        Stack<TreeNodeConstPos<T>> S { p };
        while (!S.empty()) {
            auto node { S.pop() };
            this->call(visit, node);
            for (size_t i { node->children().size() }; i > 0; --i) {
                S.push(node->children()[i - 1]);
            }
        }
    }
};

}