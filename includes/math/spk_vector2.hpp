#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <iostream>
#include <algorithm>

namespace spk
{
    /**
     * @class IVector2
     * @brief A two-dimensional vector class with variable type.
     *
     * This class represents a two-dimensional vector.
     * It provides a number of methods for vector operations.
     * @tparam TType Type used for vector coordinates.
     */
    template <typename TType>
    struct IVector2
    {
        TType x; ///< The x-coordinate of the vector.
        TType y; ///< The y-coordinate of the vector.

        /**
         * @brief Default constructor.
         *
         * Creates a two-dimensional vector with coordinates (0,0).
         */
        IVector2() : x(0), y(0) {}

        /**
         * @brief Constructor that initializes both coordinates with a single value.
         * @param p_value The value to be used for both coordinates.
         */
        template <typename TOtherType>
        IVector2(const TOtherType &p_value) : x(p_value), y(p_value) {}

        /**
         * @brief Constructor that initializes the vector with two values.
         * @param p_x The value to be used for the x-coordinate.
         * @param p_y The value to be used for the y-coordinate.
         */
        template <typename TOtherType>
        IVector2(const TOtherType &p_x, const TOtherType &p_y) : x(p_x), y(p_y) {}

        /**
         * @brief Type-casting operator.
         *
         * Allows an instance of the class to be type-casted to an instance of
         * IVector2 of another type.
         * @return An instance of IVector2 of the new type.
         */
        template <typename TOtherType>
        operator IVector2<TOtherType>()
        {
            return (IVector2<TOtherType>(static_cast<TOtherType>(x), static_cast<TOtherType>(y)));
        }

        /**
         * @brief Overload of << operator for output stream.
         *
         * This operator is used to output the content of an IVector2 object.
         * @param p_os The output stream.
         * @param p_self The IVector2 object to output.
         * @return The output stream.
         */
        friend std::wostream &operator<<(std::wostream &p_os, const IVector2 &p_self)
        {
            p_os << p_self.x << " / " << p_self.y;
            return p_os;
        }

        /**
         * @brief Overload of + operator.
         *
         * This operator is used to add two IVector2 objects.
         * @param p_other The other IVector2 to add.
         * @return The resulting IVector2.
         */
        template <typename TOtherType>
        IVector2<TType> operator+(const IVector2<TOtherType> &p_other) const
        {
            return IVector2<TType>(x + p_other.x, y + p_other.y);
        }

        /**
         * @brief Overload of - operator.
         *
         * This operator is used to subtract two IVector2 objects.
         * @param p_other The other IVector2 to subtract.
         * @return The resulting IVector2.
         */
        template <typename TOtherType>
        IVector2<TType> operator-(const IVector2<TOtherType> &p_other) const
        {
            return IVector2<TType>(x - p_other.x, y - p_other.y);
        }

        /**
         * @brief Overload of * operator.
         *
         * This operator is used to multiply two IVector2 objects.
         * @param p_other The other IVector2 to multiply with.
         * @return The resulting IVector2.
         */
        template <typename TOtherType>
        IVector2<TType> operator*(const IVector2<TOtherType> &p_other) const
        {
            return (IVector2<TType>(x * p_other.x, y * p_other.y));
        }

        /**
         * @brief Overload of / operator.
         *
         * This operator is used to divide two IVector2 objects.
         * @param p_other The other IVector2 to divide by.
         * @return The resulting IVector2.
         * @throws std::runtime_error if an attempt is made to divide by zero.
         */
        template <typename TOtherType>
        IVector2<TType> operator/(const IVector2<TOtherType> &p_other) const
        {
            if (p_other.x == 0 || p_other.y == 0)
                throw std::runtime_error("Trying to divide by 0");
            return (IVector2<TType>(x / p_other.x, y / p_other.y));
        }

        /**
         * @brief Overload of += operator.
         *
         * This operator is used to add another IVector2 to this object and assign the result to this object.
         * @param p_other The other IVector2 to add.
         */
        template <typename TOtherType>
        void operator+=(const IVector2<TOtherType> &p_other)
        {
            x += p_other.x;
            y += p_other.y;
        }

        /**
         * @brief Overload of -= operator.
         *
         * This operator is used to subtract another IVector2 from this object and assign the result to this object.
         * @param p_other The other IVector2 to subtract.
         */
        template <typename TOtherType>
        void operator-=(const IVector2<TOtherType> &p_other)
        {
            x -= p_other.x;
            y -= p_other.y;
        }

        /**
         * @brief Overload of *= operator.
         *
         * This operator is used to multiply this object by another IVector2 and assign the result to this object.
         * @param p_other The other IVector2 to multiply with.
         */
        template <typename TOtherType>
        void operator*=(const IVector2<TOtherType> &p_other)
        {
            x *= p_other.x;
            y *= p_other.y;
        }

        /**
         * @brief Overload of /= operator.
         *
         * This operator is used to divide this object by another IVector2 and assign the result to this object.
         * @param p_other The other IVector2 to divide by.
         * @throws std::runtime_error if an attempt is made to divide by zero.
         */
        template <typename TOtherType>
        void operator/=(const IVector2<TOtherType> &p_other)
        {
            if (p_other.x == 0 || p_other.y == 0)
                throw std::runtime_error("Trying to divide by 0");
            x /= p_other.x;
            y /= p_other.y;
        }
        /**
         * @brief Overload of == operator.
         *
         * This operator is used to compare if two IVector2 objects are equal.
         * @param delta The other IVector2 to compare with.
         * @return true if both IVector2 objects are equal, false otherwise.
         */
        template <typename TOtherType>
        bool operator==(const IVector2<TOtherType> &delta) const
        {
            return (x == delta.x && y == delta.y);
        }

        /**
         * @brief Overload of != operator.
         *
         * This operator is used to compare if two IVector2 objects are not equal.
         * @param delta The other IVector2 to compare with.
         * @return true if both IVector2 objects are not equal, false otherwise.
         */
        template <typename TOtherType>
        bool operator!=(const IVector2<TOtherType> &delta) const
        {
            return (x != delta.x || y != delta.y);
        }

        /**
         * @brief Overload of < operator.
         *
         * This operator is used to compare if one IVector2 object is less than another.
         * @param p_other The other IVector2 to compare with.
         * @return true if this IVector2 object is less than the other, false otherwise.
         */
        template <typename TOtherType>
        bool operator<(const IVector2<TOtherType> &p_other) const
        {
            if (y < p_other.y)
                return (true);
            if (y == p_other.y && x < p_other.x)
                return (true);
            return (false);
        }

        /**
         * @brief Overload of > operator.
         *
         * This operator is used to compare if one IVector2 object is greater than another.
         * @param p_other The other IVector2 to compare with.
         * @return true if this IVector2 object is greater than the other, false otherwise.
         */
        template <typename TOtherType>
        bool operator>(const IVector2<TOtherType> &p_other) const
        {
            if (y > p_other.y)
                return (true);
            if (y == p_other.y && x > p_other.x)
                return (true);
            return (false);
        }

        /**
         * @brief Overload of <= operator.
         *
         * This operator is used to compare if one IVector2 object is less than or equal to another.
         * @param p_other The other IVector2 to compare with.
         * @return true if this IVector2 object is less than or equal to the other, false otherwise.
         */
        template <typename TOtherType>
        bool operator<=(const IVector2<TOtherType> &p_other) const
        {
            if (y < p_other.y)
                return (true);
            if (y == p_other.y && x <= p_other.x)
                return (true);
            return (false);
        }

        /**
         * @brief Overload of >= operator.
         *
         * This operator is used to compare if one IVector2 object is greater than or equal to another.
         * @param p_other The other IVector2 to compare with.
         * @return true if this IVector2 object is greater than or equal to the other, false otherwise.
         */
        template <typename TOtherType>
        bool operator>=(const IVector2<TOtherType> &p_other) const
        {
            if (y > p_other.y)
                return (true);
            if (y == p_other.y && x >= p_other.x)
                return (true);
            return (false);
        }

        /**
         * @brief Calculates the norm (or length) of this vector.
         *
         * This method calculates the Euclidean norm (also known as the L2 norm) of this vector, which is the square root of the sum of the squares of the coordinates.
         * @return The norm of the vector.
         */
        float norm() const
        {
            return (sqrt(pow(x, 2.0f) + pow(y, 2.0f)));
        }

        /**
         * @brief Calculates the Euclidean distance between this vector and another vector.
         *
         * This method calculates the Euclidean distance (or L2 distance) between this vector and another vector. The Euclidean distance is the square root of the sum of the squares of differences in each coordinate.
         * @tparam TOtherType The type of the other vector.
         * @param p_other The other IVector2 object to compute the distance to.
         * @return The Euclidean distance between this vector and the other vector.
         */
        template <typename TOtherType>
        float distance(const IVector2<TOtherType> &p_other) const
        {
            return (static_cast<float>(std::sqrt(pow(p_other.x - x, 2) + pow(p_other.y - y, 2))));
        }

        /**
         * @brief Normalizes this vector.
         *
         * This method normalizes this vector by dividing each coordinate by the norm of the vector. The resulting vector has a norm of 1. If the norm of the vector is zero, a runtime error is thrown.
         * @return The normalized vector.
         * @throws std::runtime_error if the norm of the vector is zero.
         */
        IVector2<float> normalize() const
        {
            float tmp_norm;

            tmp_norm = this->norm();

            if (tmp_norm == 0)
                std::runtime_error("Trying to divide by 0");
            return (IVector2<float>(static_cast<float>(x) / tmp_norm, static_cast<float>(y) / tmp_norm));
        }
        /**
         * @brief Performs the 2D cross product with another vector.
         *
         * This method calculates the 2D cross product of this vector with another vector. The result is a new vector.
         * @tparam TOtherType The type of the other vector.
         * @param p_other The other IVector2 object to compute the cross product with. Defaults to a zero vector.
         * @return The cross product of this vector and the other vector.
         */
        template <typename TOtherType>
        IVector2 cross(const IVector2<TOtherType> &p_other = IVector2(0, 0)) const
        {
            IVector2 result;

            result = IVector2(-(p_other.y - y), p_other.x - x);

            return (result);
        }

        /**
         * @brief Calculates the z-component of the cross product with another vector.
         *
         * This method calculates the z-component of the 2D cross product of this vector with another vector. The result is a scalar.
         * @tparam TOtherType The type of the other vector.
         * @param p_other The other IVector2 object to compute the cross product with.
         * @return The z-component of the cross product of this vector and the other vector.
         */
        template <typename TOtherType>
        float crossZ(const IVector2<TOtherType> &p_other) const
        {
            return (x * p_other.y - y * p_other.x);
        }

        /**
         * @brief Performs the dot product with another vector, with respect to a center vector.
         *
         * This method calculates the dot product of this vector with another vector, with respect to a center vector. The result is a scalar.
         * @tparam TOtherType The type of the other vector.
         * @param p_other The other IVector2 object to compute the dot product with.
         * @param p_center The center IVector2 object. Defaults to a zero vector.
         * @return The dot product of this vector and the other vector, with respect to the center vector.
         */
        template <typename TOtherType>
        float dot(const IVector2<TOtherType> &p_other, const IVector2<TOtherType> &p_center = IVector2(0, 0)) const
        {
            float result;

            result = (x - p_center.x) * (p_other.x - p_center.x) + (y - p_center.y) * (p_other.y - p_center.y);

            return (result);
        }

        /**
         * @brief Calculates the angle between this vector and another vector, with respect to a center vector.
         *
         * This method calculates the angle in degrees between this vector and another vector, with respect to a center vector.
         * @tparam TOtherType The type of the other vector.
         * @param p_other The other IVector2 object to compute the angle to.
         * @param p_center The center IVector2 object. Defaults to a zero vector.
         * @return The angle in degrees between this vector and the other vector, with respect to the center vector.
         */
        template <typename TOtherType>
        float angle(const IVector2<TOtherType> &p_other, const IVector2<TOtherType> &p_center = IVector2(0, 0)) const
        {
            float rdot = dot(p_other, p_center);

            rdot = (rdot < -1.0f ? -1.0f : (rdot > 1.0f ? 1.0f : rdot));

            float angle = std::acos(rdot);

            return (angle * (180 / M_PI));
        }

        /**
         * @brief Inverts this vector.
         *
         * This method inverts this vector by multiplying each coordinate by -1.
         * The result is a new vector pointing in the opposite direction.
         * @return The inverted vector.
         */
        IVector2 invert() const
        {
            return (IVector2(x * -1, y * -1));
        }

        /**
         * @brief Rotates this vector around a given center point.
         *
         * This method rotates this vector around a given center point by a specified angle.
         * @tparam TOtherType The type of the center vector.
         * @param p_center The IVector2 object that serves as the rotation center point.
         * @param p_angle The angle in degrees by which to rotate the vector.
         * @return The rotated vector.
         */
        template <typename TOtherType>
        IVector2 rotate(const IVector2<TOtherType> &p_center, const float &p_angle) const
        {
            float theta = p_angle * (M_PI / 180);

            float pcos = cos(theta);
            float psin = sin(theta);

            IVector2 result;
            result.x = (x - p_center.x) * pcos - (y - p_center.y) * psin;
            result.y = (x - p_center.x) * psin + (y - p_center.y) * pcos;

            return (result + p_center);
        }

        /**
         * @brief Rounds down the coordinates of a vector.
         *
         * This method rounds down the coordinates of a given IVector2 object.
         * @param p_vector The vector to be rounded down.
         * @return The vector with rounded down coordinates.
         */
        static IVector2 floor(const IVector2 &p_vector)
        {
            IVector2 result;

            result.x = ::floorf(p_vector.x);
            result.y = ::floorf(p_vector.y);

            return (result);
        }

        /**
         * @brief Rounds up the coordinates of a vector.
         *
         * This method rounds up the coordinates of a given IVector2 object.
         * @param p_vector The vector to be rounded up.
         * @return The vector with rounded up coordinates.
         */
        static IVector2 ceiling(const IVector2 &p_vector)
        {
            IVector2 result;

            result.x = ::ceilf(p_vector.x);
            result.y = ::ceilf(p_vector.y);

            return (result);
        }

        /**
         * @brief Rounds the coordinates of a vector to the nearest integer.
         *
         * This method rounds the coordinates of a given IVector2 object to the nearest integer.
         * @param p_vector The vector to be rounded.
         * @return The vector with rounded coordinates.
         */
        static IVector2 round(const IVector2 &p_vector)
        {
            IVector2 result;

            result.x = ::roundf(p_vector.x);
            result.y = ::roundf(p_vector.y);

            return (result);
        }

        /**
         * @brief Calculates the vector with the minimum coordinates.
         *
         * This method calculates the IVector2 object with the minimum coordinates from two given vectors.
         * @param p_min The first vector.
         * @param p_max The second vector.
         * @return The vector with the minimum coordinates.
         */
        static IVector2 min(const IVector2 &p_min, const IVector2 &p_max)
        {
            return (IVector2(std::min(p_min.x, p_max.x), std::min(p_min.y, p_max.y)));
        }

        /**
         * @brief Calculates the vector with the maximum coordinates.
         *
         * This method calculates the IVector2 object with the maximum coordinates from two given vectors.
         * @param p_min The first vector.
         * @param p_max The second vector.
         * @return The vector with the maximum coordinates.
         */
        static IVector2 max(const IVector2 &p_min, const IVector2 &p_max)
        {
            return (IVector2(std::max(p_min.x, p_max.x), std::max(p_min.y, p_max.y)));
        }

        /**
         * @brief Checks if a point is inside a rectangle defined by two corner points.
         *
         * This method checks if a given point (as an IVector2 object) is inside a rectangle defined by two corner points (as IVector2 objects).
         * @param p_point The point to be checked.
         * @param p_cornerA The first corner point of the rectangle.
         * @param p_cornerB The second corner point of the rectangle.
         * @return True if the point is inside the rectangle, false otherwise.
         */
        static bool isInsideRectangle(const IVector2 &p_point, const IVector2 &p_cornerA, const IVector2 &p_cornerB)
        {
            IVector2 tmpMin = IVector2::min(p_cornerA, p_cornerB);
            IVector2 tmpMax = IVector2::max(p_cornerA, p_cornerB);

            if (p_point.x < tmpMin.x || p_point.y < tmpMin.y ||
                p_point.x > tmpMax.x || p_point.y > tmpMax.y)
                return (false);
            return (true);
        }

        /**
         * @brief Interpolates linearly between two points.
         *
         * This method performs a linear interpolation between two points, creating a smooth transition from the starting point to the ending point.
         * @param p_startingPoint The starting point for the interpolation.
         * @param p_endingPoint The ending point for the interpolation.
         * @param t The interpolation parameter, ranging from 0 (start point) to 1 (end point). For values outside this range, the function extrapolates.
         * @return The interpolated vector.
         */
        static IVector2 lerp(const IVector2 &p_startingPoint, const IVector2 &p_endingPoint, float t)
        {
            return IVector2(
                p_startingPoint.x + (p_endingPoint.x - p_startingPoint.x) * t,
                p_startingPoint.y + (p_endingPoint.y - p_startingPoint.y) * t);
        }
    };

    /**
     * @brief Overload of the addition operator for an arithmetic value and a vector.
     *
     * @tparam TType The type of the elements of the vector.
     * @tparam TValueType The type of the arithmetic value.
     * @param p_value The arithmetic value to be added.
     * @param p_point The vector to which the arithmetic value is added.
     * @return A new vector resulting from the addition.
     */
    template <typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
    IVector2<TType> operator+(TValueType p_value, const IVector2<TType> &p_point)
    {
        return (IVector2<TType>(p_value, p_value) + p_point);
    };

    /**
     * @brief Overload of the substraction operator for an arithmetic value and a vector.
     *
     * @tparam TType The type of the elements of the vector.
     * @tparam TValueType The type of the arithmetic value.
     * @param p_value The arithmetic value to be added.
     * @param p_point The vector to which the arithmetic value is added.
     * @return A new vector resulting from the addition.
     */
    template <typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
    IVector2<TType> operator-(TValueType p_value, const IVector2<TType> &p_point)
    {
        return (IVector2<TType>(p_value, p_value) - p_point);
    };

    /**
     * @brief Overload of the multiplication operator for an arithmetic value and a vector.
     *
     * @tparam TType The type of the elements of the vector.
     * @tparam TValueType The type of the arithmetic value.
     * @param p_value The arithmetic value to be added.
     * @param p_point The vector to which the arithmetic value is added.
     * @return A new vector resulting from the addition.
     */
    template <typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
    IVector2<TType> operator*(TValueType p_value, const IVector2<TType> &p_point)
    {
        return (IVector2<TType>(p_value, p_value) * p_point);
    };

    /**
     * @brief Overload of the division operator for an arithmetic value and a vector.
     *
     * @tparam TType The type of the elements of the vector.
     * @tparam TValueType The type of the arithmetic value.
     * @param p_value The arithmetic value to be added.
     * @param p_point The vector to which the arithmetic value is added.
     * @return A new vector resulting from the addition.
     */
    template <typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
    IVector2<TType> operator/(TValueType p_value, const IVector2<TType> &p_point)
    {
        return (IVector2<TType>(p_value, p_value) / p_point);
    };

    using Vector2 = IVector2<float>;
    using Vector2Int = IVector2<int>;
    using Vector2UInt = IVector2<unsigned int>;
}
