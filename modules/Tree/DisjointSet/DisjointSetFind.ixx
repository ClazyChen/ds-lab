export module DisjointSet.DisjointSetFind;

import Vector;
import Framework.Algorithm;

export namespace dslab {

class AbstractDisjointSetFind : public Algorithm<Rank(Rank, Vector<Rank>&)> {
};

class DisjointSetFindBasic : public AbstractDisjointSetFind {
public:
    Rank operator()(Rank x, Vector<Rank>& parent) override {
        while (x != parent[x]) {
            x = parent[x];
        }
        return x;
    }
};

class DisjointSetFindPathCompression : public DisjointSetFindBasic {
public:
    Rank operator()(Rank x, Vector<Rank>& parent) override {
        auto root { DisjointSetFindBasic::operator()(x, parent) };
        while (x != root) {
            auto parentX { parent[x] };
            parent[x] = root;
            x = parentX;
        }
        return root;
    }
};

class DisjointSetFindPathHalving : public DisjointSetFindBasic {
public:
    Rank operator()(Rank x, Vector<Rank>& parent) override {
        while (parent[x] != parent[parent[x]]) {
            x = parent[x] = parent[parent[x]];
        }
        return parent[x];
    }
};

class DisjointSetFindPathSplitting : public DisjointSetFindBasic {
public:
    Rank operator()(Rank x, Vector<Rank>& parent) override {
        while (parent[x] != parent[parent[x]]) {
            auto parentX { parent[x] };
            parent[x] = parent[parent[x]];
            x = parentX;
        }
        return parent[x];
    }
};

}