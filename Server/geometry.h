#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cstdint>

/*
 * Module contains types and functions for geometric calculations.
 */

/*
 * Class for handling 2D vector.
 */
template<typename T>
class Vector {
public:
    Vector(T const x, T const y) : x{x}, y{y} {}

    T x, y;

    Vector<T> &operator+=(Vector<T> const &v);

    bool operator==(Vector<T> const &v);
    bool operator!=(Vector<T> const &v);
};


template<typename T>
inline Vector<T> &Vector<T>::operator+=(Vector<T> const &v) {
    x += v.x;
    y += v.y;

    return *this;
}


template<typename T>
inline Vector<T> operator+(Vector<T> const &v1, Vector<T> const &v2) {
    return Vector<T>(v1.x + v2.x, v1.y + v2.y);
}


template<typename T>
inline bool Vector<T>::operator==(Vector<T> const &v) {
    return x == v.x && y == v.y;
}


template<typename T>
inline bool Vector<T>::operator!=(Vector<T> const &v) {
    return !(*this == v);
}

/*
 * Types of possible vector spaces.
 */
using Pixel = Vector<uint32_t>;
using Position = Vector<double>;

/*
 * Function returns unit vector rotated along X axis.
 */
Position rotationVector(uint16_t angle);

/*
 * Function map board position to pixel.
 */
Pixel roundPosition(Position pos);

#endif //GEOMETRY_H
