#include <vector>
#include <any>
#include <iostream>

int main() {
    std::vector<std::any> v;
    v.push_back(1);
    v.push_back(3.14);
    v.push_back("Hello, World!");
    std::cout << std::any_cast<int>(v[0]) << std::endl; // 1
    std::cout << std::any_cast<double>(v[1]) << std::endl; // 3.14
    std::cout << std::any_cast<const char*>(v[2]) << std::endl; // "Hello, World!"
    return 0;
}