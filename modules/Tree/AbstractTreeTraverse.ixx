module;
#include <functional>

export module Tree.Traverse.AbstractTreeTraverse;

import Framework.Algorithm;
import Tree.TreeNode;

export namespace dslab {

template <typename T>
class AbstractTreeTraverse : public Algorithm<void(TreeNodeConstPos<T>, std::function<void(const T&)>)> {};

}