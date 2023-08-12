export module HuffmanTree;

import BinaryTree;
import PriorityQueue;
import Framework.Algorithm;
import std;
import EncodingTree;

export namespace dslab {

class HuffmanTree : public Algorithm<EncodingTree(const std::map<char, double>&)> {
protected:
    struct SubTree {
        BinaryTreeNodeInst<char> node;
        double frequency;
        bool operator==(const SubTree& t) const {
            return frequency == t.frequency;
        }
        auto operator<=>(const SubTree& t) const {
            return frequency <=> t.frequency;
        }
    };

public:
    EncodingTree operator()(const std::map<char, double>& frequencies) override {
        Heap<SubTree> H;
        for (const auto& [c, f] : frequencies) {
            H.push(SubTree { BinaryTreeNodeInst<char>::make(c), f });
        }
        while (H.size() > 1) {
            auto t0 { H.pop() }, t1 { H.pop() };
            auto node { BinaryTreeNodeInst<char>::make(0) };
            (node->left() = std::move(t0.node))->parent() = node;
            (node->right() = std::move(t1.node))->parent() = node;
            H.push(SubTree { std::move(node), t0.frequency + t1.frequency });
        }
        return EncodingTree { std::move(H.pop().node) };
    }
};

}