import Framework;
import Matrix;
import std;

using namespace dslab;
using namespace std;

template <typename T, size_t N, template<typename, size_t, size_t> typename Matrix>
    requires (has_single_bit(N)) && std::is_base_of_v<AbstractMatrix<T, N, N>, Matrix<T, N, N>>
class MatrixMultiply : public Algorithm<void(const Matrix<T, N, N>&, const Matrix<T, N, N>&, Matrix<T, N, N>&)> {
};

// 用于对比，IJK版本的朴素矩阵乘法
template <typename T, size_t N, template <typename, size_t, size_t> typename Matrix>
class MatrixMultiply_IJK : public MatrixMultiply<T, N, Matrix> {
public:
    void operator()(const Matrix<T, N, N>& A, const Matrix<T, N, N>& B, Matrix<T, N, N>& C) override {
        C.clear();
        for (int i { 0 }; i < N; ++i) {
            for (int j { 0 }; j < N; ++j) {
                for (size_t k { 0 }; k < N; ++k) {
                    C[{i, j}] += A[{i, k}] * B[{k, j}];
                }
            }
        }
    }
    string type_name() const override {
        return "Naive(IJK)";
    }
};

template <typename T, size_t N, template <typename, size_t, size_t> typename Matrix>
class MatrixMultiplyParallel : public MatrixMultiply<T, N, Matrix> {
public:
    void operator()(const Matrix<T, N, N>& A, const Matrix<T, N, N>& B, Matrix<T, N, N>& C) override {
        C.clear();
        #pragma omp parallel for
        for (int i { 0 }; i < N; ++i) {
            #pragma omp parallel for
            for (int j { 0 }; j < N; ++j) {
                for (size_t k { 0 }; k < N; ++k) {
                    C[{i, j}] += A[{i, k}] * B[{k, j}];
                }
            }
        }
    }
    string type_name() const override {
        return "Parallel";
    }
};

template <typename T, size_t N, template <typename, size_t, size_t> typename Matrix>
    requires (has_single_bit(N)) && std::is_base_of_v<AbstractMatrix<T, N, N>, Matrix<T, N, N>>
class MatrixSpan {
    Matrix<T, N, N>& m;
public:
    pair<size_t, size_t> p;
    size_t sz;
    MatrixSpan(Matrix<T, N, N>& m) : m(m), p({ 0, 0 }), sz(N) {}
    MatrixSpan(const Matrix<T, N, N>& m) : m(const_cast<Matrix<T, N, N>&>(m)), p({ 0, 0 }), sz(N) {}
    MatrixSpan(Matrix<T, N, N>& m, pair<size_t, size_t> p, size_t sz) : m(m), p(p), sz(sz) {}
    T& at(pair<size_t, size_t> q) {
        return m[{ p.first + q.first, p.second + q.second }];
    }
    const T& at(pair<size_t, size_t> q) const {
        return m[{ p.first + q.first, p.second + q.second }];
    }
    MatrixSpan operator[](pair<size_t, size_t> q) {
        return MatrixSpan<T, N, Matrix>(m, { p.first + q.first * (sz >> 1), p.second + q.second * (sz >> 1) }, sz >> 1);
    }
};

// 分治矩阵乘法
template <typename T, size_t N, template <typename, size_t, size_t> typename Matrix>
class MatrixMultiplyDivideAndConquer : public MatrixMultiply<T, N, Matrix> {
    using Span = MatrixSpan<T, N, Matrix>;
    void add(Span A, Span B) {
        for (int i { 0 }; i < A.sz; ++i) {
            for (int j { 0 }; j < A.sz; ++j) {
                A.at({ i, j }) += B.at({ i, j });
            }
        }
    }

    void multiply(Span A, Span B, Span C) {
        if (A.sz == 1) {
            C.at({0, 0}) += A.at({0, 0}) * B.at({0, 0});
        } else {
            #pragma omp parallel sections
            {
                #pragma omp section
                {
                    multiply(A[{0, 0}], B[{0, 0}], C[{0, 0}]);
                    multiply(A[{0, 1}], B[{1, 0}], C[{0, 0}]);
                }
                #pragma omp section
                {
                    multiply(A[{0, 0}], B[{0, 1}], C[{0, 1}]);
                    multiply(A[{0, 1}], B[{1, 1}], C[{0, 1}]);
                }
                #pragma omp section
                {
                    multiply(A[{1, 0}], B[{0, 0}], C[{1, 0}]);
                    multiply(A[{1, 1}], B[{1, 0}], C[{1, 0}]);
                }
                #pragma omp section
                {
                    multiply(A[{1, 0}], B[{0, 1}], C[{1, 1}]);
                    multiply(A[{1, 1}], B[{1, 1}], C[{1, 1}]);
                }
            }
        }
    }
public:
    void operator()(const Matrix<T, N, N>& A, const Matrix<T, N, N>& B, Matrix<T, N, N>& C) override {
        C.clear();
        multiply(Span { A }, Span { B }, Span { C });
    }
    string type_name() const override {
        return "Divide and Conquer";
    }
};

// Strassen算法
template <typename T, size_t N, template <typename, size_t, size_t> typename Matrix>
class MatrixMultiplyStrassen : public MatrixMultiply<T, N, Matrix> {
    using Span = MatrixSpan<T, N, Matrix>;

    // 加法和减法
    void add(Span A, Span B, Span C) {
        for (size_t i { 0 }; i < A.sz; ++i) {
            for (size_t j { 0 }; j < A.sz; ++j) {
                C.at({i, j}) = A.at({i, j}) + B.at({i, j});
            }
        }
    }
    void subtract(Span A, Span B, Span C) {
        for (size_t i { 0 }; i < A.sz; ++i) {
            for (size_t j { 0 }; j < A.sz; ++j) {
                C.at({i, j}) = A.at({i, j}) - B.at({i, j});
            }
        }
    }
    
    // 中间结果的存储矩阵及相关的视图
    std::array<Matrix<T, N, N>, 10> MatS;
    std::array<Matrix<T, N, N>, 7> MatP;
    std::vector<Span> S;
    std::vector<Span> P;

    // 初始化S和P
    void initialize() {
        transform(begin(MatS), end(MatS), back_inserter(S), [](auto& m) { return Span { m }; });
        transform(begin(MatP), end(MatP), back_inserter(P), [](auto& m) { return Span { m }; });
    }

    // 设置S和P的规模，等待C++23的views::concat
    void resize(size_t sz) {
        for (auto& s : S) {
            s.p = { sz, sz };
            s.sz = sz;
        }
        for (auto& p : P) {
            p.p = { sz, sz };
            p.sz = sz;
        }
    }

    // Strassen算法
    void multiply(Span A, Span B, Span C) {
        if (A.sz == 1) {
            C.at({0, 0}) = A.at({0, 0}) * B.at({0, 0});
        } else {
            resize(A.sz / 2);
            subtract(B[{0, 1}], B[{1, 1}], S[0]);
            add(A[{0, 0}], A[{0, 1}], S[1]);
            add(A[{1, 0}], A[{1, 1}], S[2]);
            subtract(B[{1, 0}], B[{0, 0}], S[3]);
            add(A[{0, 0}], A[{1, 1}], S[4]);
            add(B[{0, 0}], B[{1, 1}], S[5]);
            subtract(A[{0, 1}], A[{1, 1}], S[6]);
            add(B[{1, 0}], B[{1, 1}], S[7]);
            subtract(A[{0, 0}], A[{1, 0}], S[8]);
            add(B[{0, 0}], B[{0, 1}], S[9]);
            multiply(A[{0, 0}], S[0], P[0]);
            multiply(S[1], B[{1, 1}], P[1]);
            multiply(S[2], B[{0, 0}], P[2]);
            multiply(A[{1, 1}], S[3], P[3]);
            multiply(S[4], S[5], P[4]);
            multiply(S[6], S[7], P[5]);
            multiply(S[8], S[9], P[6]);
            add(P[4], P[3], S[0]);
            subtract(S[0], P[1], S[0]);
            add(S[0], P[5], C[{0, 0}]);
            add(P[0], P[1], C[{0, 1}]);
            add(P[2], P[3], C[{1, 0}]);
            add(P[4], P[0], S[0]);
            subtract(S[0], P[2], S[0]);
            subtract(S[0], P[6], C[{1, 1}]);
            resize(A.sz);
        }
    }

public:
    void operator()(const Matrix<T, N, N>& A, const Matrix<T, N, N>& B, Matrix<T, N, N>& C) override {
        initialize();
        multiply(Span { A }, Span { B }, Span { C });
    }
    string type_name() const override {
        return "Strassen";
    }
};

// 矩阵乘法的测试模板，对两个矩阵进行乘法，返回矩阵乘积的迹
// 以下和NaiveMatrixMultiply.cpp相同

template <size_t N, template <typename, size_t, size_t> typename Matrix>
    requires std::is_base_of_v<AbstractMatrix<int, N, N>, Matrix<int, N, N>>
class MatrixMultiplyTest : public Algorithm<int(shared_ptr<MatrixMultiply<int, N, Matrix>> multiply) > {
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
    int operator()(shared_ptr<MatrixMultiply<int, N, Matrix>> multiply) override {
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
    vector<shared_ptr<MatrixMultiply<int, N, RowMajorMatrix>>> rowMajorMultiply {
        make_shared<MatrixMultiply_IJK<int, N, RowMajorMatrix>>(),
        make_shared<MatrixMultiplyParallel<int, N, RowMajorMatrix>>(),
        make_shared<MatrixMultiplyDivideAndConquer<int, N, RowMajorMatrix>>(),
        make_shared<MatrixMultiplyStrassen<int, N, RowMajorMatrix>>()
    };
    vector<shared_ptr<MatrixMultiply<int, N, ColumnMajorMatrix>>> columnMajorMultiply {
        make_shared<MatrixMultiply_IJK<int, N, ColumnMajorMatrix>>(),
        make_shared<MatrixMultiplyParallel<int, N, ColumnMajorMatrix>>(),
        make_shared<MatrixMultiplyDivideAndConquer<int, N, ColumnMajorMatrix>>(),
        make_shared<MatrixMultiplyStrassen<int, N, ColumnMajorMatrix>>()
    };
    TestFramework<MatrixMultiplyTest<N, RowMajorMatrix>, MatrixMultiplyTestImpl<N, RowMajorMatrix>> rowMajorTest;
    TestFramework<MatrixMultiplyTest<N, ColumnMajorMatrix>, MatrixMultiplyTestImpl<N, ColumnMajorMatrix>> columnMajorTest;

public:
    void operator()() override {
        cout << format("Row Major Matrix Multiply Test (N = {})", N) << endl;
        rowMajorTest.run(&MatrixMultiplyTest<N, RowMajorMatrix>::initialize);
        for (auto& multiply : rowMajorMultiply) {
            cout << format("{:20} ... ", multiply->type_name());
            rowMajorTest(multiply);
        }
        cout << format("Col Major Matrix Multiply Test (N = {})", N) << endl;
        columnMajorTest.run(&MatrixMultiplyTest<N, ColumnMajorMatrix>::initialize);
        for (auto& multiply : columnMajorMultiply) {
            cout << format("{:20} ... ", multiply->type_name());
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

MatrixMultiplyNTest<64, 128, 256, 512> test;

int main() {
    test();
    return 0;
}