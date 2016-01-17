//
// Created by Sam Sleight on 14/01/2016.
//

#ifndef MATHS_MATRIX_HPP
#define MATHS_MATRIX_HPP

#include "impl.hpp"
#include "iterator.hpp"

namespace squid {

template<std::size_t N, std::size_t M, typename T = int>
class Matrix {
public:
    static const std::size_t Rows = N;
    static const std::size_t Cols = M;

    template<MatrixIteratorMode mode>
    using Iterator = MatrixIterator<Rows, Cols, T, mode>;

    using row_idx_impl = impl::mat_idx_impl<Matrix<Rows, Cols, T>, std::make_index_sequence<Rows>>;
    using col_idx_impl = impl::mat_idx_impl<Matrix<Rows, Cols, T>, std::make_index_sequence<Cols>>;

private:
    T data[Rows * Cols];

public:
    Matrix() = delete;

    template<typename... U>
    constexpr Matrix(U... args) noexcept : data{args...} {};

    template<typename... V>
    static constexpr Matrix<Rows, Cols, T> from_row_vectors(V... vecs) {
        return col_idx_impl::from_row_vectors(vecs...);
    };

    template<typename... V>
    static constexpr Matrix<Rows, Cols, T> from_col_vectors(V... vecs) {
        return row_idx_impl::from_col_vectors(vecs...);
    };

    constexpr Matrix<Rows + 1, Cols, T> append_row(const Vector<Cols, T>& row) const {
        using Idxs = std::pair<std::make_index_sequence<Rows * Cols>, std::make_index_sequence<Cols>>;
        return impl::mat_pair_impl<Rows, Cols, 0, T, Idxs>::append_row(*this, row);
    };

    constexpr Matrix<Rows + 1, Cols, T> push_row(const Vector<Cols, T>& row) const {
        using Idxs = std::pair<std::make_index_sequence<Rows * Cols>, std::make_index_sequence<Cols>>;
        return impl::mat_pair_impl<Rows, Cols, 0, T, Idxs>::push_row(*this, row);
    };

    constexpr Matrix<Rows, Cols + 1, T> push_col(const Vector<Rows, T>& col) const {
        using Idxs = std::pair<std::make_index_sequence<Rows>, std::make_index_sequence<Cols>>;
        return impl::mat_pair_impl<Rows, Cols, 0, T, Idxs>::push_col(*this, col);
    };

    constexpr T get(std::size_t n) const {
        return n < (Rows * Cols) ? data[n] : throw std::invalid_argument("Index must be less than Matrix::(Rows * Cols)");
    };

    constexpr Vector<Cols, T> row(std::size_t n) const {
        return n < Rows ? col_idx_impl::row(*this, n) : throw std::invalid_argument("Row index must be less than Matrix::Rows");
    };

    constexpr Vector<Rows, T> col(std::size_t n) const {
        return n < Cols ? row_idx_impl::col(*this, n) : throw std::invalid_argument("Col index must be less than Matrix::Cols");
    };

    template<std::size_t P, typename U>
    constexpr Matrix<Rows, P, T> operator*(const Matrix<Cols, P, U>& other) const {
        using Idxs = impl::make_mul_idxs<P>;
        return impl::mat_pair_impl<Rows, Cols, P, T, Idxs>::mul(*this, other);
    };

    constexpr Iterator<MatrixIteratorMode::Rows> rows() const {
        return Iterator<MatrixIteratorMode::Rows>(*this);
    };

    constexpr Iterator<MatrixIteratorMode::Cols> cols() const {
        return Iterator<MatrixIteratorMode::Cols>(*this);
    };
};

template<std::size_t N, std::size_t M, typename T, typename U>
constexpr auto operator*(const U& scalar, const Matrix<N, M, T>& matrix) {
    return impl::mat_idx_impl<Matrix<N, M, T>, std::make_index_sequence<N * M>>::scalar_mul(scalar, matrix);
};

}

#endif //MATHS_MATRIX_HPP
