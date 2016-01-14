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
    static constexpr T dot(const Vector<Size, T>& lhs, const Vector<Size, T>& rhs) {
        return ((lhs.get(Idxs) * rhs.get(Idxs)) + ...);
    };
};

}

template<std::size_t N, typename T = int>
class Vector {
public:
    static const std::size_t Size = N;

private:
    using idx_impl = util::vec_idx_impl<Vector<Size, T>, std::make_index_sequence<Size>>;

    T data[Size];

public:
    Vector() = delete;

    template<typename... U>
    constexpr Vector(U... args) noexcept : data{args...} {}

    constexpr T get(std::size_t n) const {
        return n < Size ? data[n] : throw std::invalid_argument("Index must be less than Vector::Size");
    }

    constexpr T dot(const Vector<Size, T>& other) const noexcept {
        return idx_impl::dot(*this, other);
    }
};

}

#endif //MATHS_VECTOR_HPP
