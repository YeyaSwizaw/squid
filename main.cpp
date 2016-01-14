#include <iostream>

#include "vector.hpp"

int main(int argc, char* argv[]) {
    // Vector and dot product
    constexpr squid::Vector<4> vec1(1, 3, 5, 6);
    constexpr squid::Vector<4> vec2(2, 1, 3, 9);
    constexpr int dot = vec1.dot(vec2);
    std::cout << dot << std::endl;
}