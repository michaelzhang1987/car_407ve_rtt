/*
 *  N dimensional matrix operations
 */
#if defined(__GNUC__)
#pragma GCC optimize("O3")
#endif

#include "matrixN.h"


// multiply two vectors to give a matrix, in-place
template <typename T, uint8_t N>
void MatrixN<T,N>::mult(const VectorN<T,N> &A, const VectorN<T,N> &B)
{
    for (uint8_t i = 0; i < N; i++) {
        for (uint8_t j = 0; j < N; j++) {
            v[i][j] = A[i] * B[j];
        }
    }
}

// subtract B from the matrix
template <typename T, uint8_t N>
MatrixN<T,N> &MatrixN<T,N>::operator -=(const MatrixN<T,N> &B)
{
    for (uint8_t i = 0; i < N; i++) {
        for (uint8_t j = 0; j < N; j++) {
            v[i][j] -= B.v[i][j];
        }
    }
    return *this;
}

// add B to the matrix
template <typename T, uint8_t N>
MatrixN<T,N> &MatrixN<T,N>::operator +=(const MatrixN<T,N> &B)
{
    for (uint8_t i = 0; i < N; i++) {
        for (uint8_t j = 0; j < N; j++) {
            v[i][j] += B.v[i][j];
        }
    }
    return *this;
}

template <typename T, uint8_t N>
VectorN<T,N> &MatrixN<T,N>::operator *(const VectorN<T,N> &Vector)
{
    VectorN<T,N> result;
    for (uint8_t i = 0; i < N; i++) {
        for (uint8_t j = 0; j < N; j++) {
            result[i] += v[i][j] * Vector[j];
        }
    }
    return result;
}

// Matrix symmetry routine
template <typename T, uint8_t N>
void MatrixN<T,N>::force_symmetry(void)
{
    for (uint8_t i = 0; i < N; i++) {
        for (uint8_t j = 0; j < (i - 1); j++) {
            v[i][j] = (v[i][j] + v[j][i]) / 2;
            v[j][i] = v[i][j];
        }
    }
}

template <typename T, uint8_t N>
void MatrixN<T,N>::eye_mult(const float d[N])
{
  for (uint8_t i = 0; i < N; i++) {
    v[i][i] *= d[i];
  }
}

template <typename T, uint8_t N>
void MatrixN<T,N>::eye(void)
{
  memset(v, 0, sizeof(v)); 
  for (uint8_t i = 0; i < N; i++) {
    v[i][i] = 1.0f;
  }
}

template void MatrixN<float,4>::mult(const VectorN<float,4> &A, const VectorN<float,4> &B);
template MatrixN<float,4> &MatrixN<float,4>::operator -=(const MatrixN<float,4> &B);
template MatrixN<float,4> &MatrixN<float,4>::operator +=(const MatrixN<float,4> &B);
template void MatrixN<float,4>::force_symmetry(void);
