#include "matrix.h"
#include "matrix_list.h"
#include "matrix_triple.h"
#include "matrix_compressed.h"
#include "test_framework.h"
using namespace clazy_framework;

// 这个例子展示了如何使用矩阵的基本操作
// 因为矩阵也没什么性能分析上的必要
// 所以这里仅仅是做一个功能上的演示

constexpr const int N = 5; // 矩阵的边长

class MatrixFillProblem : public Algorithm<> {
public:
    virtual void set() = 0;     // 设置矩阵的值
    virtual void unset() = 0;   // 清零矩阵的值
    virtual void print() = 0;   // 展示矩阵的值
    void apply() override {}    // 没有实际意义的调用函数
};

template <typename Matrix>
requires (is_base_of_v<AbstractMatrix<N, N, int>, Matrix>)
class MatrixFill : public MatrixFillProblem {
protected:
    Matrix m;
public:
    void set() override {
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                m.set(r, c, r * N + c + 1);
            }
        }
    }
    void unset() override {
        m.clear();
    }
    void print() override {
        cout << getTypeName() << endl;
        cout << m << endl;
    }
    string getTypeName() const override {
        return typeid(Matrix).name();
    }
};

int main() {
    TestFramework<
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
    > tf;
    cout << "Test Set-Matrix" << endl;
    tf.invoke(bind(&MatrixFillProblem::set, placeholders::_1));
    tf.invoke(bind(&MatrixFillProblem::print, placeholders::_1));
    cout << "Test Unset-Matrix" << endl;
    tf.invoke(bind(&MatrixFillProblem::unset, placeholders::_1));
    tf.invoke(bind(&MatrixFillProblem::print, placeholders::_1));
}
