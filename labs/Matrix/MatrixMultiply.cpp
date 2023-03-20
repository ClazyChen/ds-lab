#include <vector>
#include <format>
#include <iostream>
import Framework;
import Matrix;
using namespace dslab;
using namespace std;

template <size_t N>
using Matrix = RowMajorMatrix<int, N, N>;

template <size_t N>
class MatrixMultiply : public Algorithm<void(const Matrix<N>&, const Matrix<N>&, Matrix<N>&)> {};

template <size_t N>
class MatrixMultiplyNaive : public MatrixMultiply<N> {
public:
    void operator()(const Matrix<N>& A, const Matrix<N>& B, Matrix<N>& C) override {
        for (auto i { 0 }; i < N; ++i) {
            for (auto j { 0 }; j < N; ++j) {
                C[{i, j}] = 0;
                for (auto k { 0 }; k < N; ++k) {
                    C[{i, j}] += A[{i, k}] * B[{k, j}];
                }
            }
        }
    }
};