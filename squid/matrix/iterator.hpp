//
// Created by Sam Sleight on 15/01/2016.
//

#ifndef SQUID_MATRIX_ITERATOR_HPP
#define SQUID_MATRIX_ITERATOR_HPP

#include "squid/vector/vector.hpp"

namespace squid {

template<std::size_t N, std::size_t M, typename T>
class Matrix;

enum class MatrixIteratorMode {
    Rows,
    Cols
};

template<std::size_t N, std::size_t M, typename T, MatrixIteratorMode mode, typename I>
class MatrixIteratorBase {
protected:
    const Matrix <N, M, T> &mat;

    std::size_t idx;

    constexpr MatrixIteratorBase(const Matrix <N, M, T> &mat, std::size_t idx = 0) : mat(mat), idx(idx) {}

public:
    constexpr I begin() const {
        return I(mat, 0);
    }

    constexpr void operator++() {
        idx++;
    }

    constexpr void operator--() {
        idx--;
    }

    constexpr bool operator==(const I &other) const {
        return (this->idx == other.idx);
    }

    constexpr bool operator!=(const I &other) const {
        return !this->operator==(other);
    }
};

template<std::size_t N, std::size_t M, typename T, MatrixIteratorMode mode>
class MatrixIterator;

template<std::size_t N, std::size_t M, typename T>
class MatrixIterator<N, M, T, MatrixIteratorMode::Rows>
    : public MatrixIteratorBase<N, M, T, MatrixIteratorMode::Rows, MatrixIterator<N, M, T, MatrixIteratorMode::Rows>> {
private:
    friend class Matrix<N, M, T>;

    friend class MatrixIteratorBase<N, M, T, MatrixIteratorMode::Rows, MatrixIterator<N, M, T, MatrixIteratorMode::Rows>>;

    constexpr MatrixIterator(const Matrix<N, M, T> &mat, std::size_t idx = 0)
        : MatrixIteratorBase<N, M, T, MatrixIteratorMode::Rows, MatrixIterator<N, M, T, MatrixIteratorMode::Rows>>(mat, idx) {}

public:
    constexpr MatrixIterator<N, M, T, MatrixIteratorMode::Rows> end() const {
        return MatrixIterator(this->mat, N);
    }

    constexpr Vector<M, T> operator*() const {
        return this->mat.row(this->idx);
    };

    template<typename F>
    constexpr auto map(F f) {
        return Matrix<N, M, T>::row_idx_impl::row_map(this->mat, f);
    }
};

template<std::size_t N, std::size_t M, typename T>
class MatrixIterator<N, M, T, MatrixIteratorMode::Cols>
    : public MatrixIteratorBase<N, M, T, MatrixIteratorMode::Cols, MatrixIterator<N, M, T, MatrixIteratorMode::Cols>> {
private:
    friend class Matrix<N, M, T>;

    friend class MatrixIteratorBase<N, M, T, MatrixIteratorMode::Cols, MatrixIterator<N, M, T, MatrixIteratorMode::Cols>>;

    constexpr MatrixIterator(const Matrix<N, M, T> &mat, std::size_t idx = 0)
        : MatrixIteratorBase<N, M, T, MatrixIteratorMode::Cols, MatrixIterator<N, M, T, MatrixIteratorMode::Cols>>(mat, idx) {}

public:
    constexpr MatrixIterator<N, M, T, MatrixIteratorMode::Cols> end() const {
        return MatrixIterator(this->mat, N);
    }

    constexpr Vector <M, T> operator*() const {
        return this->mat.col(this->idx);
    };

    template<typename F>
    constexpr auto map(F f) {
        return Matrix<N, M, T>::col_idx_impl::col_map(this->mat, f);
    }
};

}

#endif //SQUID_MATRIX_ITERATOR_HPP
