#include "matrix.h"
#include "matrix_list.h"
#include "matrix_triple.h"
#include "matrix_compressed.h"
using namespace clazy_framework;

// 这个例子展示了如何使用矩阵的基本操作
// 因为矩阵也没什么性能分析上的必要
// 所以这里仅仅是做一个功能上的演示

constexpr const int N = 5; // 矩阵的边长

class MatrixFillProblem : public Algorithm {
public:
    virtual void set(); // 设置矩阵的值
    virtual void unset(); // 清零矩阵的值
    virtual void display(); // 展示矩阵的值
};

template <typename Matrix>
requires (is_base_of_v<clazy_framework::AbstractMatrix<N, N, int>, Matrix>)
class MatrixFill : public MatrixFillProblem {
protected:
    Matrix m;
public:
    virtual void set() override {
        for (int i : views::iota(0, N)) {
            for (int j : views::iota(0, N)) {
                m.set(i, j, i * N + j + 1);
            }
        }
    }
    virtual void unset() override {
        for (int i : views::iota(0, N)) {
            for (int j : views::iota(0, N)) {
                m.unset(i, j);
            }
        }
    }
    virtual void display() override {
        cout << m << endl;
    }
    virtual string getTypename() const override {
        return typeid(Matrix).name();
    }
};

int main() {
    auto algorithms = generateInstances<
        MatrixFillProblem,
        MatrixFill<clazy::Matrix<N>>,
        MatrixFill<clazy::MatrixList<N>>, 
        MatrixFill<clazy::MatrixTriple<N>>,
        MatrixFill<clazy::RowMajorMatrix<N>>,
        MatrixFill<clazy::ColMajorMatrix<N>>,
        MatrixFill<clazy::SymmetricMatrix<N>>,
        MatrixFill<clazy::LowerTriangularMatrix<N>>,
        MatrixFill<clazy::UpperTriangularMatrix<N>>,
        MatrixFill<clazy::TridiagonalMatrix<N>>
    >();
    cout << "Test Set-Matrix" << endl;
    for (auto algorithm : algorithms) {
        cout << algorithm->getTypename() << endl;
        algorithm->set();
        algorithm->display();
    }
    cout << "Test Unset-Matrix" << endl;
    for (auto algorithm : algorithms) {
        cout << algorithm->getTypename() << endl;
        algorithm->unset();
        algorithm->display();
    }
}
