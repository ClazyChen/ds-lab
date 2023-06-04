import Framework;
import Matrix;
import Matrix.SymmetricMatrix;
import Matrix.TriangularMatrix;
import Matrix.TridiagonalMatrix;
import Matrix.TripleMatrix;
import std;

using namespace std;

using dslab::Algorithm;

// 这个实验用于展示和验证矩阵的基本操作
// 由于本实验中用到的矩阵类型较多，测试流程比较简单
// 使用给定的初始化列表，对矩阵进行初始化，并输出

constexpr size_t N { 5 };
using SymmetricMatrix = dslab::SymmetricMatrix<int, N>;
using LowerTriangularMatrix = dslab::LowerTriangularMatrix<int, N>;
using UpperTriangularMatrix = dslab::UpperTriangularMatrix<int, N>;
using TridiagonalMatrix = dslab::TridiagonalMatrix<int, N>;
using TripleMatrix = dslab::TripleMatrix<int, N>;

void checkSymmetricMatrix() {
    SymmetricMatrix M {
        {  1,  2,  3,  4,  5 },
        {  2,  6,  7,  8,  9 },
        {  3,  7, 10, 11, 12 },
        {  4,  8, 11, 13, 14 },
        {  5,  9, 12, 14, 15 }
    };
    cout << "Symmetric Matrix: " << endl;
    cout << M << endl;
}

void checkLowerTriangularMatrix() {
    LowerTriangularMatrix M {
        {  1                 },
        {  2,  6             },
        {  3,  7, 10         },
        {  4,  8, 11, 13     },
        {  5,  9, 12, 14, 15 }
    };
    cout << "Lower Triangular Matrix: " << endl;
    cout << M << endl;
}

void checkUpperTriangularMatrix() {
    UpperTriangularMatrix M {
        {  1,  2,  3,  4,  5 },
        {      6,  7,  8,  9 },
        {         10, 11, 12 },
        {             13, 14 },
        {                 15 }
    };
    cout << "Upper Triangular Matrix: " << endl;
    cout << M << endl;
}

void checkTridiagonalMatrix() {
    TridiagonalMatrix M {
        {  1,  2             },
        {  2,  6,  7         },
        {      7, 10, 11     },
        {         11, 13, 14 },
        {             14, 15 }
    };
    cout << "Tridiagonal Matrix: " << endl;
    cout << M << endl;
}

void checkTripleMatrix() {
    TripleMatrix M {
        { 0, 0, 1 },
        { 0, 1, 2 },
        { 0, 2, 3 },
        { 0, 4, 5 },
        { 1, 2, 7 },
        { 2, 3, 11 },
        { 3, 3, 13 }
    };
    cout << "Triple Matrix: " << endl;
    cout << M << endl;
}

int main() {
    checkSymmetricMatrix();
    checkLowerTriangularMatrix();
    checkUpperTriangularMatrix();
    checkTridiagonalMatrix();
    checkTripleMatrix();
    return 0;
}
