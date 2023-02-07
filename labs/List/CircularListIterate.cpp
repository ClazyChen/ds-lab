#include <vector>
#include <format>
#include <ranges>
#include <iostream>
import Framework;
import List;
using namespace dslab;
using namespace std;

// 这个小实验展示了循环列表无限轮询的特点
// 首先生成一个0到N的列表
// 然后从列表的第一个元素开始，访问M个元素
template <template <typename> typename L>
void printList(size_t N, size_t M) {
    L<size_t> list {};
    ranges::copy(views::iota(0) | views::take(N), back_inserter(list));
    cout << format("Test {} : ", list.type_name()) << endl;
    cout << "\t" << list << endl << "\t";
    for (auto& item : views::counted(begin(list), M)) {
        cout << format("{}, ", item);
    }
    cout << endl << "\t";
    for (auto& item : views::counted(rbegin(list), M)) {
        cout << format("{}, ", item);
    }
    cout << endl;
}

// 对于普通的List来说，所取的M不能大于N，因为遍历到尾节点就为止了
// 但对于循环列表来说，M可以取任意大的值，因为它是无限循环的

int main() {
    printList<List>(3, 3);
    // printList<List>(3, 10); // error
    printList<CircularList>(3, 10);
    return 0;
}