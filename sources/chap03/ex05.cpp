#include "list.h"
#include "random.h"
#include "list_sort.h"
using namespace clazy;
using namespace clazy_framework;

int main() {
	auto V = randomVector(10);
	List<int> L;
	ListMergeSort<int> S;
	for (int i = 9; i >= 0; i--) {
		L.push_back(V[i]);
	}
	cout << L << endl;
	S.apply(L);
	cout << L << endl;
	return 0;
} 
