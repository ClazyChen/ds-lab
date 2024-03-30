#include "circular_list.hpp"

using namespace dslab;

// this experiment shows the feature of circular list that it can iterate infinitely
// first generate a list from 0 to N
// then iterate M elements from the first element of the list
// then iterate M elements from the last element of the list 
template <template <typename> typename L>
void printList(std::size_t N, std::size_t M) {
    L<std::size_t> list {};
    std::ranges::copy(std::views::iota(0) | std::views::take(N), std::back_inserter(list));
    std::cout << std::format("Test {} : {}", list.type_name(), list) << std::endl;
    for (auto & item : std::views::counted(list.begin(), M)) {
        std::cout << std::format("{}, ", item);
    }
    std::cout << std::endl;
    for (auto & item : std::views::counted(list.rbegin(), M)) {
        std::cout << std::format("{}, ", item);
    }
    std::cout << std::endl;
}

// for normal list, M cannot be greater than N, because it will stop at the tail node
// but for circular list, M can be any value, because it is infinitely circular

int main() {
    printList<List>(3, 3);
    // printList<List>(3, 10); // error
    printList<CircularList>(3, 10);
    return 0;
}