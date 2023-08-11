#pragma once

#include "math/spk_vector2.hpp"
#include <stdexcept>
#include <string>

namespace spk
{
	/**
	 * @brief A generic 3D vector class template.
	 *
	 * @tparam TType The type of the vector components.
	 */
	template <typename TType>
	struct IVector3
	{
		TType x; /**< The x-coordinate value. */
		TType y; /**< The y-coordinate value. */
		TType z; /**< The z-coordinate value. */

		/**
		 * @brief Default constructor. Initializes the vector with zero values.
		 */
		IVector3() : x(0), y(0), z(0) {}

		/**
		 * @brief Constructs a new IVector3 object with the same value for x, y, and z.
		 *
		 * @tparam TOtherType The type of the value.
		 * @param p_value The value to assign to x, y, and z.
		 */
		template <typename TOtherType>
		IVector3(const TOtherType &p_value) : x(static_cast<TType>(p_value)), y(static_cast<TType>(p_value)), z(static_cast<TType>(p_value)) {}

		/**
		 * @brief Constructs a new IVector3 object with the x and y values from another IVector2 object and a specified z value.
		 *
		 * @tparam TOtherType The type of the x and y values.
		 * @tparam TOtherValueType The type of the z value.
		 * @param p_other The IVector2 object to get the x and y values from.
		 * @param p_value The value to assign to z.
		 */
		template <typename TOtherType, typename TOtherValueType>
		IVector3(const IVector2<TOtherType> &p_other, const TOtherValueType &p_value) : x(static_cast<TType>(p_other.x)), y(static_cast<TType>(p_other.y)), z(static_cast<TType>(p_value)) {}

		/**
		 * @brief Constructs a new IVector3 object from another IVector3 object with potentially different value types.
		 *
		 * @tparam TOtherType The type of the other IVector3 object's values.
		 * @param p_other The other IVector3 object.
		 */
		template <typename TOtherType>
		IVector3(const IVector3<TOtherType> &p_other) : x(p_other.x), y(p_other.y), z(p_other.z) {}

		/**
		 * @brief Constructs a new IVector3 object with specified x, y, and z values.
		 *
		 * @tparam TOtherType1 The type of the x value.
		 * @tparam TOtherType2 The type of the y value.
		 * @tparam TOtherType3 The type of the z value.
		 * @param p_x The value to assign to x.
		 * @param p_y The value to assign to y.
		 * @param p_z The value to assign to z.
		 */
		template <typename TOtherType1, typename TOtherType2, typename TOtherType3>
		IVector3(const TOtherType1 &p_x, const TOtherType2 &p_y, const TOtherType3 &p_z) : x(static_cast<TType>(p_x)), y(static_cast<TType>(p_y)), z(static_cast<TType>(p_z)) {}

		IVector3(const spk::JSON::Object& p_object)
		{
			if constexpr (std::is_floating_point<TType>::value)
			{
				x = p_object[L"x"].as<double>();
				y = p_object[L"y"].as<double>();
				z = p_object[L"z"].as<double>();
			}
			else
			{
				x = p_object[L"x"].as<int>();
				y = p_object[L"y"].as<int>();
				z = p_object[L"z"].as<int>();
			}
		}

		/**
		 * @brief Conversion operator to convert the vector to another vector type.
		 * @tparam TOtherType The type to convert the vector elements to.
		 * @return An IVector3 object with the elements converted to TOtherType.
		 */
		template <typename TOtherType>
		operator IVector3<TOtherType>()
		{
			return (IVector3<TOtherType>(static_cast<TOtherType>(x), static_cast<TOtherType>(y), static_cast<TOtherType>(z)));
		}

		/**
		 * @brief Overloaded insertion operator for printing the vector to an output stream.
		 * @param p_os The output stream.
		 * @param p_self The IVector3 object to be printed.
		 * @return The modified output stream.
		 */
		friend std::ostream &operator<<(std::ostream &p_os, const IVector3 &p_self)
		{
			p_os << p_self.x << " / " << p_self.y << " / " << p_self.z;
			return (p_os);
		}

		/**
		 * @brief Convert the vector to a wstring.
		 * @return The resulting string.
		*/
		std::wstring to_wstring() const
		{
			return (std::to_wstring(x) + L" / " + std::to_wstring(y) + L" / " + std::to_wstring(z));
		}

		/**
		 * @brief Overloaded addition operator for adding two IVector3 objects.
		 * @param p_other The IVector3 object to be added.
		 * @return The result of the addition as an IVector3 object.
		 */
		IVector3<TType> operator+(const IVector3 &p_other) const
		{
			return (IVector3<TType>(x + p_other.x, y + p_other.y, z + p_other.z));
		}

		/**
		 * @brief Overloaded subtraction operator for subtracting two IVector3 objects.
		 * @param p_other The IVector3 object to be subtracted.
		 * @return The result of the subtraction as an IVector3 object.
		 */
		IVector3<TType> operator-(const IVector3 &p_other) const
		{
			return (IVector3<TType>(x - p_other.x, y - p_other.y, z - p_other.z));
		}

		/**
		 * @brief Overloaded multiplication operator for multiplying two IVector3 objects.
		 * @param p_other The IVector3 object to be multiplied.
		 * @return The result of the multiplication as an IVector3 object.
		 */
		IVector3<TType> operator*(const IVector3 &p_other) const
		{
			return (IVector3<TType>(x * p_other.x, y * p_other.y, z * p_other.z));
		}

		/**
		 * @brief Overloaded division operator for dividing two IVector3 objects.
		 * @param p_other The IVector3 object to be divided by.
		 * @return The result of the division as an IVector3 object.
		 * @throws std::runtime_error if any component of p_other is zero.
		 */
		IVector3<TType> operator/(const IVector3 &p_other) const
		{
			if (p_other.x == 0 || p_other.y == 0 || p_other.z == 0)
				throw std::runtime_error("Trying to divide by 0");
			return (IVector3<TType>(x / p_other.x, y / p_other.y, z / p_other.z));
		}

		/**
		 * @brief Overloaded compound addition operator for adding another IVector3 object to this vector.
		 * @param p_other The IVector3 object to be added.
		 */
		void operator+=(const IVector3 &p_other)
		{
			x += p_other.x;
			y += p_other.y;
			z += p_other.z;
		}

		/**
		 * @brief Overloaded compound subtraction operator for subtracting another IVector3 object from this vector.
		 * @param p_other The IVector3 object to be subtracted.
		 */
		void operator-=(const IVector3 &p_other)
		{
			x -= p_other.x;
			y -= p_other.y;
			z -= p_other.z;
		}

		/**
		 * @brief Overloaded compound multiplication operator for multiplying this vector with another IVector3 object.
		 * @param p_other The IVector3 object to be multiplied.
		 */
		void operator*=(const IVector3 &p_other)
		{
			x *= p_other.x;
			y *= p_other.y;
			z *= p_other.z;
		}

		/**
		 * @brief Overloaded compound division operator for dividing this vector by another IVector3 object.
		 * @param p_other The IVector3 object to be divided by.
		 * @throws std::runtime_error if any component of p_other is zero.
		 */
		void operator/=(const IVector3 &p_other)
		{
			if (p_other.x == 0 || p_other.y == 0 || p_other.z == 0)
				throw std::runtime_error("Trying to divide by 0");
			x /= p_other.x;
			y /= p_other.y;
			z /= p_other.z;
		}

		/**
		 * @brief Overloaded equality operator to compare the vector with another value of a different type.
		 * @tparam TOtherType The type of the value to compare with.
		 * @param p_other The value to compare with.
		 * @return True if all components of the vector are equal to p_other, false otherwise.
		 */
		template <typename TOtherType>
		bool operator==(const TOtherType &p_other) const
		{
			return (x == static_cast<TType>(p_other) && y == static_cast<TType>(p_other) && z == static_cast<TType>(p_other));
		}

		/**
		 * @brief Overloaded inequality operator to compare the vector with another value of a different type.
		 * @tparam TOtherType The type of the value to compare with.
		 * @param p_other The value to compare with.
		 * @return True if any component of the vector is not equal to p_other, false otherwise.
		 */
		template <typename TOtherType>
		bool operator!=(const TOtherType &p_other) const
		{
			return (x != static_cast<TType>(p_other) || y != static_cast<TType>(p_other) || z != static_cast<TType>(p_other));
		}

		/**
		 * @brief Overloaded less than operator to compare the vector with another value of a different type.
		 * @tparam TOtherType The type of the value to compare with.
		 * @param p_other The value to compare with.
		 * @return True if the vector is less than p_other based on lexicographic comparison of components, false otherwise.
		 */
		template <typename TOtherType>
		bool operator<(const TOtherType &p_other) const
		{
			if (z < static_cast<TType>(p_other))
				return (true);
			if (z == static_cast<TType>(p_other) && y < static_cast<TType>(p_other))
				return (true);
			if (z == static_cast<TType>(p_other) && y == static_cast<TType>(p_other) && x < static_cast<TType>(p_other))
				return (true);
			return (false);
		}

		/**
		 * @brief Overloaded greater than operator to compare the vector with another value of a different type.
		 * @tparam TOtherType The type of the value to compare with.
		 * @param p_other The value to compare with.
		 * @return True if the vector is greater than p_other based on lexicographic comparison of components, false otherwise.
		 */
		template <typename TOtherType>
		bool operator>(const TOtherType &p_other) const
		{
			if (z > static_cast<TType>(p_other))
				return (true);
			if (z == static_cast<TType>(p_other) && y > static_cast<TType>(p_other))
				return (true);
			if (z == static_cast<TType>(p_other) && y == static_cast<TType>(p_other) && x > static_cast<TType>(p_other))
				return (true);
			return (false);
		}

		/**
		 * @brief Overloaded less than or equal to operator to compare the vector with another value of a different type.
		 * @tparam TOtherType The type of the value to compare with.
		 * @param p_other The value to compare with.
		 * @return True if the vector is less than or equal to p_other based on lexicographic comparison of components, false otherwise.
		 */
		template <typename TOtherType>
		bool operator<=(const TOtherType &p_other) const
		{
			if (this->operator==(p_other) == true || this->operator<(p_other) == true)
				return (true);
			return (false);
		}

		/**
		 * @brief Overloaded greater than or equal to operator to compare the vector with another value of a different type.
		 * @tparam TOtherType The type of the value to compare with.
		 * @param p_other The value to compare with.
		 * @return True if the vector is greater than or equal to p_other based on lexicographic comparison of components, false otherwise.
		 */
		template <typename TOtherType>
		bool operator>=(const TOtherType &p_other) const
		{
			if (this->operator==(p_other) == true || this->operator>(p_other) == true)
				return (true);
			return (false);
		}

		/**
		 * @brief Overloaded equality operator to compare the vector with another IVector3 object of a different type.
		 * @tparam TOtherType The type of the other IVector3 object.
		 * @param p_other The IVector3 object to compare with.
		 * @return True if all components of the vectors are equal, false otherwise.
		 */
		template <typename TOtherType>
		bool operator==(const IVector3<TOtherType> &p_other) const
		{
			return ((x == static_cast<TType>(p_other.x) && y == static_cast<TType>(p_other.y) && z == static_cast<TType>(p_other.z) ? true : false));
		}

		/**
		 * @brief Overloaded inequality operator to compare the vector with another IVector3 object of a different type.
		 * @tparam TOtherType The type of the other IVector3 object.
		 * @param p_other The IVector3 object to compare with.
		 * @return True if any component of the vectors is not equal, false otherwise.
		 */
		template <typename TOtherType>
		bool operator!=(const IVector3<TOtherType> &p_other) const
		{
			return ((x == static_cast<TType>(p_other.x) && y == static_cast<TType>(p_other.y) && z == static_cast<TType>(p_other.z) ? false : true));
		}

		/**
		 * @brief Overloaded less than operator to compare the vector with another IVector3 object of a different type.
		 * @tparam TOtherType The type of the other IVector3 object.
		 * @param p_other The IVector3 object to compare with.
		 * @return True if the vector is less than the other IVector3 object based on lexicographic comparison of components, false otherwise.
		 */
		template <typename TOtherType>
		bool operator<(const IVector3<TOtherType> &p_other) const
		{
			if (z < static_cast<TType>(p_other.z))
				return (true);
			if (z == static_cast<TType>(p_other.z) && y < static_cast<TType>(p_other.y))
				return (true);
			if (z == static_cast<TType>(p_other.z) && y == static_cast<TType>(p_other.y) && x < static_cast<TType>(p_other.x))
				return (true);
			return (false);
		}

		/**
		 * @brief Overloaded greater than operator to compare the vector with another IVector3 object of a different type.
		 * @tparam TOtherType The type of the other IVector3 object.
		 * @param p_other The IVector3 object to compare with.
		 * @return True if the vector is greater than the other IVector3 object based on lexicographic comparison of components, false otherwise.
		 */
		template <typename TOtherType>
		bool operator>(const IVector3<TOtherType> &p_other) const
		{
			if (z > static_cast<TType>(p_other.z))
				return (true);
			if (z == static_cast<TType>(p_other.z) && y > static_cast<TType>(p_other.y))
				return (true);
			if (z == static_cast<TType>(p_other.z) && y == static_cast<TType>(p_other.y) && x > static_cast<TType>(p_other.x))
				return (true);
			return (false);
		}

		/**
		 * @brief Overloaded less than or equal to operator to compare the vector with another IVector3 object of a different type.
		 * @tparam TOtherType The type of the other IVector3 object.
		 * @param p_other The IVector3 object to compare with.
		 * @return True if the vector is less than or equal to the other IVector3 object based on lexicographic comparison of components, false otherwise.
		 */
		template <typename TOtherType>
		bool operator<=(const IVector3<TOtherType> &p_other) const
		{
			if (this->operator==(p_other) == true || this->operator<(p_other) == true)
				return (true);
			return (false);
		}

		/**
		 * @brief Overloaded greater than or equal to operator to compare the vector with another IVector3 object of a different type.
		 * @tparam TOtherType The type of the other IVector3 object.
		 * @param p_other The IVector3 object to compare with.
		 * @return True if the vector is greater than or equal to the other IVector3 object based on lexicographic comparison of components, false otherwise.
		 */
		template <typename TOtherType>
		bool operator>=(const IVector3<TOtherType> &p_other) const
		{
			if (this->operator==(p_other) == true || this->operator>(p_other) == true)
				return (true);
			return (false);
		}

		/**
		 * @brief Performs linear interpolation between two vectors.
		 * @param a The starting vector.
		 * @param b The ending vector.
		 * @param t The interpolation parameter.
		 * @return The interpolated vector between a and b based on t.
		 */
		static IVector3 lerp(const IVector3 &a, const IVector3 &b, float t)
		{
			return IVector3(
				a.x + (b.x - a.x) * t,
				a.y + (b.y - a.y) * t,
				a.z + (b.z - a.z) * t);
		}

		/**
		 * @brief Calculates the distance between this vector and another vector.
		 * @tparam TOtherType The type of the other vector.
		 * @param p_other The other vector to calculate the distance to.
		 * @return The distance between this vector and p_other.
		 */
		template <typename TOtherType>
		float distance(const IVector3<TOtherType> &p_other) const
		{
			return (static_cast<float>(::sqrt(pow(p_other.x - x, 2) + pow(p_other.y - y, 2) + pow(p_other.z - z, 2))));
		}

		/**
		 * @brief Calculates the norm (length) of the vector.
		 * @return The norm of the vector.
		 */
		float norm() const
		{
			return (sqrt(pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f)));
		}

		/**
		 * @brief Returns a normalized copy of the vector.
		 * @return A normalized copy of the vector.
		 * @throws std::runtime_error if the norm of the vector is zero.
		 */
		IVector3<float> normalize() const
		{
			IVector3<float> result;

			float tmpnorm = norm();

			if (tmpnorm == 0)
				throw std::runtime_error("Try to divide by 0");

			result.x = x / tmpnorm;
			result.y = y / tmpnorm;
			result.z = z / tmpnorm;

			return (result);
		}

		/**
		 * @brief Calculates the cross product between this vector and another vector.
		 * @tparam TType The type of the other vector.
		 * @param p_other The other vector to calculate the cross product with.
		 * @return The cross product of this vector and p_other.
		 */
		IVector3<TType> cross(const IVector3<TType> &p_other) const
		{
			return IVector3<TType>(
				y * p_other.z - z * p_other.y,
				z * p_other.x - x * p_other.z,
				x * p_other.y - y * p_other.x);
		}

		/**
		 * @brief Rotates the vector based on rotation values in degrees.
		 * @tparam TType The type of the rotation values.
		 * @param p_rotationValues The rotation values in degrees.
		 * @return The rotated vector.
		 */
		IVector3<TType> rotate(const IVector3<TType> &p_rotationValues) const
		{
			TType x_rad = p_rotationValues.x * static_cast<TType>(M_PI) / 180;
			TType y_rad = p_rotationValues.y * static_cast<TType>(M_PI) / 180;
			TType z_rad = p_rotationValues.z * static_cast<TType>(M_PI) / 180;

			TType cos_x = cos(x_rad);
			TType sin_x = sin(x_rad);
			TType cos_y = cos(y_rad);
			TType sin_y = sin(y_rad);
			TType cos_z = cos(z_rad);
			TType sin_z = sin(z_rad);

			IVector3<TType> result;
			result.x = cos_y * cos_z * x + (sin_x * sin_y * cos_z - cos_x * sin_z) * y + (cos_x * sin_y * cos_z + sin_x * sin_z) * z;
			result.y = cos_y * sin_z * x + (sin_x * sin_y * sin_z + cos_x * cos_z) * y + (cos_x * sin_y * sin_z - sin_x * cos_z) * z;
			result.z = -sin_y * x + sin_x * cos_y * y + cos_x * cos_y * z;

			return result;
		}

		/**
		 * @brief Calculates the dot product between this vector and another vector.
		 * @tparam TType The type of the other vector.
		 * @param p_other The other vector to calculate the dot product with.
		 * @return The dot product of this vector and p_other.
		 */
		TType dot(const IVector3<TType> &p_other) const
		{
			return x * p_other.x + y * p_other.y + z * p_other.z;
		}

		/**
		 * @brief Projects the vector onto a plane defined by its normal and a point on the plane.
		 * @param p_planeNormal The normal vector of the plane.
		 * @param p_planePoint A point on the plane.
		 * @return The projected vector onto the plane.
		 */
		IVector3 projectOnPlane(const IVector3 &p_planeNormal, const IVector3 &p_planePoint) const
		{
			IVector3 pointToPlane = *this - p_planePoint;
			float distance = pointToPlane.dot(p_planeNormal);
			return (*this - p_planeNormal * distance);
		}

		/**
		 * @brief Removes the y component of the vector and returns a 2D vector.
		 * @return A 2D vector with the x and z components of the original vector.
		 */
		IVector2<TType> removeY()
		{
			return (IVector2<TType>(x, z));
		}

		/**
		 * @brief Returns the inverse (negation) of the vector.
		 * @return The inverse of the vector.
		 */
		IVector3 inverse()
		{
			return (IVector3(-x, -y, -z));
		}
	};

	/**
	 * @brief Overloaded addition operator to add a value to a vector.
	 * @tparam TType The type of the vector.
	 * @tparam TValueType The type of the value.
	 * @param p_value The value to be added.
	 * @param p_point The vector to add the value to.
	 * @return The result of adding the value to the vector.
	 */
	template <typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector3<TType> operator+(TValueType p_value, const IVector3<TType> &p_point)
	{
		return (IVector3<TType>(p_value) + p_point);
	};

	/**
	 * @brief Overloaded subtraction operator to subtract a value from a vector.
	 * @tparam TType The type of the vector.
	 * @tparam TValueType The type of the value.
	 * @param p_value The value to be subtracted.
	 * @param p_point The vector to subtract the value from.
	 * @return The result of subtracting the value from the vector.
	 */
	template <typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector3<TType> operator-(TValueType p_value, const IVector3<TType> &p_point)
	{
		return (IVector3<TType>(p_value) - p_point);
	};

	/**
	 * @brief Overloaded multiplication operator to multiply a value with a vector.
	 * @tparam TType The type of the vector.
	 * @tparam TValueType The type of the value.
	 * @param p_value The value to be multiplied.
	 * @param p_point The vector to multiply the value with.
	 * @return The result of multiplying the value with the vector.
	 */
	template <typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector3<TType> operator*(TValueType p_value, const IVector3<TType> &p_point)
	{
		return (IVector3<TType>(p_value) * p_point);
	};

	/**
	 * @brief Overloaded division operator to divide a value by a vector.
	 * @tparam TType The type of the vector.
	 * @tparam TValueType The type of the value.
	 * @param p_value The value to be divided.
	 * @param p_point The vector to divide the value by.
	 * @return The result of dividing the value by the vector.
	 */
	template <typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector3<TType> operator/(TValueType p_value, const IVector3<TType> &p_point)
	{
		return (IVector3<TType>(p_value) / p_point);
	};

	/**
	 * @brief Convert a vector to a wstring.
	 * @param p_point The vector to convert.
	 * @tparam TType The type of the vector.
	 * @return The resulting string.
	*/
	template <typename TType>
	std::wstring to_wstring(const IVector3<TType> &p_point)
	{
		return (p_point.to_wstring());
	}

	/**
	 * @brief Alias for IVector3 with float type.
	 */
	using Vector3 = IVector3<float>;

	/**
	 * @brief Alias for IVector3 with int type.
	 */
	using Vector3Int = IVector3<int>;

	/**
	 * @brief Alias for IVector3 with unsigned int type.
	 */
	using Vector3UInt = IVector3<unsigned int>;

};
