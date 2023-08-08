export module Sort.TournamentSort;

import Vector;
import Sort.AbstractSort;
import PriorityQueue.WinnerTree;
import std;

export namespace dslab {

template <typename T, template<typename> typename Linear>
    requires std::is_base_of_v<AbstractVector<T>, Linear<T>>
class TournamentSort : public AbstractSort<T, Linear> {
protected:
    void sort(Linear<T>& V) override {
        if (auto n { V.size() };  n > 0) {
            WinnerTree<T> W { std::move(V) };
            for (Rank i { 0 }; i < n; ++i) {
                V[i] = W.pop();
            }
        }
    }
public:
    std::string type_name() const override {
        return "Tournament Sort";
    }
};

}