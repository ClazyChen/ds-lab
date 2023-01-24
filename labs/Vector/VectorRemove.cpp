#include <vector>
#include <format>
#include <iostream>
import Framework;
import Vector;
using namespace dslab;
using namespace std;

class VectorRemove : public Algorithm<size_t, int> {
protected:
    Vector<int> V {};
    virtual void batchRemove(int e) = 0;
public:
    size_t operator()(int e) override {
        size_t n { V.size() };
        batchRemove(e);
        return n - V.size();
    }
    void initialize(int n) {
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
};

class VectorRemoveImproved : public VectorRemove {
protected:
    void batchRemove(int e) override {
        Rank r { 0 };
        while (r = find(begin(V) + r, end(V), e) - begin(V), r < V.size()) {
            V.remove(r);
        }
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
                V[r - k] = V[r];
            }
        }
        V.resize(V.size() - k);
    }
};

// Remove Erase Idiom
class VectorRemoveErase : public VectorRemove {
protected:
    void batchRemove(int e) override {
        V.resize(remove(begin(V), end(V), e) - begin(V));
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
