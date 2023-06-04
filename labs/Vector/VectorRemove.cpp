import Framework;
import Vector;
import std;

using namespace dslab;
using namespace std;

class VectorRemove : public Algorithm<size_t(int)> {
protected:
    Vector<int> V {};
    virtual void batchRemove(int e) = 0;
public:
    size_t operator()(int e) override {
        size_t n { V.size() };
        batchRemove(e);
        return n - V.size();
    }
    void initialize(size_t n) {
        V.resize(n);
        for (size_t i { 0 }; i < n; ++i) {
            V[i] = i % 2;
        }
    }
};

class VectorRemoveBasic : public VectorRemove {
protected:
    void batchRemove(int e) override {
        Rank r { 0 };
        while (r = find(begin(V), end(V), e) - begin(V), r < V.size()) {
            V.remove(r);
        }
    }

public:
    string type_name() const override {
        return "Separate Find, Separate Remove";
    }
};

class VectorRemoveImproved : public VectorRemove {
protected:
    void batchRemove(int e) override {
        Rank r { 0 };
        while (r = find(begin(V) + r, end(V), e) - begin(V), r < V.size()) {
            V.remove(r);
        }
    }

public:
    string type_name() const override {
        return "Global   Find, Separate Remove";
    }
};

class VectorRemoveFSP : public VectorRemove {
protected:
    void batchRemove(int e) override {
        Rank k { 0 };
        for (Rank r { 0 }; r < V.size(); ++r) {
            if (V[r] == e) {
                ++k;
            } else {
                V[r - k] = move(V[r]);
            }
        }
        V.resize(V.size() - k);
    }

public:
    string type_name() const override {
        return "Global   Find, Batched  Remove (Fast & Slow Ptr)";
    }
};

// Remove Erase Idiom
class VectorRemoveErase : public VectorRemove {
protected:
    void batchRemove(int e) override {
        V.resize(remove(begin(V), end(V), e) - begin(V));
    }

public:
    string type_name() const override {
        return "Global   Find, Batched  Remove (Remove Erase Idiom)";
    }
};

vector testData { 10, 100, 1000, 10000, 100'000 };

TestFramework<VectorRemove, VectorRemoveBasic, VectorRemoveImproved, VectorRemoveFSP, VectorRemoveErase> test;

int main() {
    for (auto n : testData) {
        cout << format("n = {}", n) << endl;
        test.run([n](auto& algo) { algo.initialize(n); });
        test(0);
    }
}
