export module EncodingTree;

import BinaryTree;
import std;

export namespace dslab {

class EncodingTree : public BinaryTree<char> {
public:
    EncodingTree() = default;
    EncodingTree(BinaryTreeNodeInst<char>&& root) : BinaryTree<char>(std::move(root)) {}

    std::string decode(std::vector<bool> bits) const {
        std::string str {};
        auto node { this->root() };
        for (auto bit : bits) {
            if (bit) {
                node = node->right();
            } else {
                node = node->left();
            }
            if (node->isLeaf()) {
                str += node->data();
                node = this->root();
            }
        }
        return str;
    }
};

}