#include <iostream>
using namespace std;

long double x;

struct A {
    char c1;
    long double y;
    char c2;
};

int main()
{
    cout << sizeof(x) << endl;
    cout << sizeof(struct A) << endl;
    return 0;
}