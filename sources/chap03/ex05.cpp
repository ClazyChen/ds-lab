#include "list.h"
#include "random.h"
#include "list_sort.h"
using namespace clazy;
using namespace clazy_framework;

int main() {
	auto V = randomVector(10);
	ForwardList<int> L;
	ListMergeSort<int, ListNodePos<int>, ForwardListNode<int>> S;
	for (int i = 0; i < 10; i++) {
		L.push_back(V[i]);
	}
	cout << L << endl;
	S.apply(L);
	cout << L << endl;
	return 0;
} 
