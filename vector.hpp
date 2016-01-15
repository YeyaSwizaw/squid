//
// Created by Sam Sleight on 14/01/2016.
//

#ifndef MATHS_VECTOR_HPP
#define MATHS_VECTOR_HPP

namespace squid {

template<std::size_t N, typename T>
class Vector;

namespace util {

template<typename V, typename Idxs>
struct vec_idx_impl;

template<std::size_t Size, typename T, std::size_t... Idxs>
struct vec_idx_impl<Vector<Size, T>, std::index_sequence<Idxs...>> {
    template<typename U>
    static constexpr auto dot(const Vector<Size, T>& lhs, const Vector<Size, U>& rhs) {
        return ((lhs.get(Idxs) * rhs.get(Idxs)) + ...);
    };

    template<typename U>
    static constexpr auto scalar_mul(const U& scalar, const Vector<Size, T>& vector) {
        return Vector<Size, decltype(scalar * T())>((scalar * vector.get(Idxs))...);
    };
};

}

template<std::size_t N, typename T = int>
class Vector {
public:
    static const std::size_t Size = N;

    using idx_impl = util::vec_idx_impl<Vector<Size, T>, std::make_index_sequence<Size>>;

private:
    T data[Size];

public:
    Vector() = delete;

    template<typename... U>
    constexpr Vector(U... args) noexcept : data{args...} {}

    constexpr T get(std::size_t n) const {
        return n < Size ? data[n] : throw std::invalid_argument("Index must be less than Vector::Size");
    }

    template<typename U>
    constexpr auto dot(const Vector<Size, U>& other) const noexcept {
        return idx_impl::dot(*this, other);
    }

    constexpr auto begin() const {
        return std::begin(data);
    }

    constexpr auto end() const {
        return std::end(data);
    }

    constexpr auto cbegin() const {
        return std::cbegin(data);
    }

    constexpr auto cend() const {
        return std::cend(data);
    }
};

template<std::size_t N, typename T, typename U>
constexpr auto operator*(const U& scalar, const Vector<N, T>& vector) {
    return Vector<N, T>::idx_impl::scalar_mul(scalar, vector);
};

}

#endif //MATHS_VECTOR_HPP
