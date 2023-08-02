import Framework;
import Matrix;
import std;

using namespace dslab;
using namespace std;

template <typename T, size_t N, template<typename, size_t, size_t> typename Matrix>
    requires std::is_base_of_v<AbstractMatrix<T, N, N>, Matrix<T, N, N>>
class NaiveMatrixMultiply : public Algorithm<void(const Matrix<T, N, N>&, const Matrix<T, N, N>&, Matrix<T, N, N>&)> {
};

template <typename T, size_t N, template <typename, size_t, size_t> typename Matrix>
class MatrixMultiply_IJK : public NaiveMatrixMultiply<T, N, Matrix> {
public:
    void operator()(const Matrix<T, N, N>& A, const Matrix<T, N, N>& B, Matrix<T, N, N>& C) override {
        C.clear();
        for (size_t i { 0 }; i < N; ++i) {
            for (size_t j { 0 }; j < N; ++j) {
                for (size_t k { 0 }; k < N; ++k) {
                    C[{i, j}] += A[{i, k}] * B[{k, j}];
                }
            }
        }
    }
    string type_name() const override {
        return "IJK";
    }
};

template <typename T, size_t N, template <typename, size_t, size_t> typename Matrix>
class MatrixMultiply_IKJ : public NaiveMatrixMultiply<T, N, Matrix> {
public:
    void operator()(const Matrix<T, N, N>& A, const Matrix<T, N, N>& B, Matrix<T, N, N>& C) override {
        C.clear();
        for (size_t i { 0 }; i < N; ++i) {
            for (size_t k { 0 }; k < N; ++k) {
                for (size_t j { 0 }; j < N; ++j) { 
                    C[{i, j}] += A[{i, k}] * B[{k, j}];
                }
            }
        }
    }
    string type_name() const override {
        return "IKJ";
    }
};

template <typename T, size_t N, template <typename, size_t, size_t> typename Matrix>
class MatrixMultiply_JIK : public NaiveMatrixMultiply<T, N, Matrix> {
public:
    void operator()(const Matrix<T, N, N>& A, const Matrix<T, N, N>& B, Matrix<T, N, N>& C) override {
        C.clear();
        for (size_t j { 0 }; j < N; ++j) {
            for (size_t i { 0 }; i < N; ++i) {
                for (size_t k { 0 }; k < N; ++k) { 
                    C[{i, j}] += A[{i, k}] * B[{k, j}];
                }
            }
        }
    }
    string type_name() const override {
        return "JIK";
    }
};

template <typename T, size_t N, template <typename, size_t, size_t> typename Matrix>
class MatrixMultiply_JKI : public NaiveMatrixMultiply<T, N, Matrix> {
public:
    void operator()(const Matrix<T, N, N>& A, const Matrix<T, N, N>& B, Matrix<T, N, N>& C) override {
        C.clear();
        for (size_t j { 0 }; j < N; ++j) {
            for (size_t k { 0 }; k < N; ++k) {
                for (size_t i { 0 }; i < N; ++i) {
                    C[{i, j}] += A[{i, k}] * B[{k, j}];
                }
            }
        }
    }
    string type_name() const override {
        return "JKI";
    }
};

template <typename T, size_t N, template <typename, size_t, size_t> typename Matrix>
class MatrixMultiply_KIJ : public NaiveMatrixMultiply<T, N, Matrix> {
public:
    void operator()(const Matrix<T, N, N>& A, const Matrix<T, N, N>& B, Matrix<T, N, N>& C) override {
        C.clear();
        for (size_t k { 0 }; k < N; ++k) {
            for (size_t i { 0 }; i < N; ++i) {
                for (size_t j { 0 }; j < N; ++j) {
                    C[{i, j}] += A[{i, k}] * B[{k, j}];
                }
            }
        }
    }
    string type_name() const override {
        return "KIJ";
    }
};

template <typename T, size_t N, template <typename, size_t, size_t> typename Matrix>
class MatrixMultiply_KJI : public NaiveMatrixMultiply<T, N, Matrix> {
public:
    void operator()(const Matrix<T, N, N>& A, const Matrix<T, N, N>& B, Matrix<T, N, N>& C) override {
        C.clear();
        for (size_t k { 0 }; k < N; ++k) {
            for (size_t j { 0 }; j < N; ++j) {
                for (size_t i { 0 }; i < N; ++i) {
                    C[{i, j}] += A[{i, k}] * B[{k, j}];
                }
            }
        }
    }
    string type_name() const override {
        return "KJI";
    }
};

// 矩阵乘法的测试模板，对两个矩阵进行乘法，返回矩阵乘积的迹

template <size_t N, template <typename, size_t, size_t> typename Matrix>
    requires std::is_base_of_v<AbstractMatrix<int, N, N>, Matrix<int, N, N>>
class MatrixMultiplyTest : public Algorithm<int(shared_ptr<NaiveMatrixMultiply<int, N, Matrix>> multiply) > {
public:
    virtual void initialize() = 0;
};

template <size_t N, template <typename, size_t, size_t> typename Matrix>
class MatrixMultiplyTestImpl : public MatrixMultiplyTest<N, Matrix> {
    Matrix<int, N, N> A, B, C;
public:
    void initialize() override {
        for (size_t i { 0 }; i < N; ++i) {
            for (size_t j { 0 }; j < N; ++j) {
                A[{i, j}] = B[{i, j}] = i + j;
            }
        }
    }
    int operator()(shared_ptr<NaiveMatrixMultiply<int, N, Matrix>> multiply) override {
        multiply->operator()(A, B, C);
        int tr { 0 };
        for (size_t i { 0 }; i < N; ++i) {
            tr += C[{i, i}];
        }
        return tr;
    }
    string type_name() const override {
        return A.type_name();
    }
};

template <size_t N>
class MatrixMultiplyN : public Algorithm<void()> {
    vector<shared_ptr<NaiveMatrixMultiply<int, N, RowMajorMatrix>>> rowMajorMultiply {
        make_shared<MatrixMultiply_IJK<int, N, RowMajorMatrix>>(),
        make_shared<MatrixMultiply_IKJ<int, N, RowMajorMatrix>>(),
        make_shared<MatrixMultiply_JIK<int, N, RowMajorMatrix>>(),
        make_shared<MatrixMultiply_JKI<int, N, RowMajorMatrix>>(),
        make_shared<MatrixMultiply_KIJ<int, N, RowMajorMatrix>>(),
        make_shared<MatrixMultiply_KJI<int, N, RowMajorMatrix>>()
    };
    vector<shared_ptr<NaiveMatrixMultiply<int, N, ColumnMajorMatrix>>> columnMajorMultiply {
        make_shared<MatrixMultiply_IJK<int, N, ColumnMajorMatrix>>(),
        make_shared<MatrixMultiply_IKJ<int, N, ColumnMajorMatrix>>(),
        make_shared<MatrixMultiply_JIK<int, N, ColumnMajorMatrix>>(),
        make_shared<MatrixMultiply_JKI<int, N, ColumnMajorMatrix>>(),
        make_shared<MatrixMultiply_KIJ<int, N, ColumnMajorMatrix>>(),
        make_shared<MatrixMultiply_KJI<int, N, ColumnMajorMatrix>>()
    };
    TestFramework<MatrixMultiplyTest<N, RowMajorMatrix>, MatrixMultiplyTestImpl<N, RowMajorMatrix>> rowMajorTest;
    TestFramework<MatrixMultiplyTest<N, ColumnMajorMatrix>, MatrixMultiplyTestImpl<N, ColumnMajorMatrix>> columnMajorTest;

public:
    void operator()() override {
        cout << format("Row Major Matrix Multiply Test (N = {:>5})", N) << endl;
        rowMajorTest.run(&MatrixMultiplyTest<N, RowMajorMatrix>::initialize);
        for (auto& multiply : rowMajorMultiply) {
            cout << format("{} ... ", multiply->type_name());
            rowMajorTest(multiply);
        }
        cout << format("Col Major Matrix Multiply Test (N = {:>5})", N) << endl;
        columnMajorTest.run(&MatrixMultiplyTest<N, ColumnMajorMatrix>::initialize);
        for (auto& multiply : columnMajorMultiply) {
            cout << format("{} ... ", multiply->type_name());
            columnMajorTest(multiply);
        }
    }
};

template <size_t... Sizes>
class MatrixMultiplyNTest : public Algorithm<void()> {
public:
    void operator()() override {
        (MatrixMultiplyN<Sizes>()(), ...);
    }
};

// MatrixMultiplyNTest<20, 100, 200, 300, 400> test;

MatrixMultiplyNTest<128, 128, 128, 128, 128, 128, 128, 128, 128, 128> test;

int main() {
    test();
    return 0;
}