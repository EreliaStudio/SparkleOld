#pragma once

#include "math/spk_vector2.hpp"
#include <stdexcept>

namespace spk
{
	template <typename TType>
	struct IVector3
	{
		TType x;
		TType y;
		TType z;

		IVector3() : x(0), y(0), z(0) { }

		template <typename TOtherType>
		IVector3(const TOtherType& p_value) : x(static_cast<TType>(p_value)), y(static_cast<TType>(p_value)), z(static_cast<TType>(p_value)) {}

		template <typename TOtherType, typename TOtherValueType>
		IVector3(const IVector2<TOtherType>& p_other, const TOtherValueType& p_value) : x(static_cast<TType>(p_other.x)), y(static_cast<TType>(p_other.y)), z(static_cast<TType>(p_value)) {}

		template <typename TOtherType>
		IVector3(const IVector3<TOtherType>& p_other) : x(p_other.x), y(p_other.y), z(p_other.z) {}

		template <typename TOtherType1, typename TOtherType2, typename TOtherType3>
		IVector3(const TOtherType1& p_x, const TOtherType2& p_y, const TOtherType3& p_z) : x(static_cast<TType>(p_x)), y(static_cast<TType>(p_y)), z(static_cast<TType>(p_z)) {}

		template <typename TOtherType>
		operator IVector3<TOtherType>()
		{
			return (IVector3<TOtherType>(static_cast<TOtherType>(x), static_cast<TOtherType>(y), static_cast<TOtherType>(z)));
		}

		friend std::ostream& operator << (std::ostream& p_os, const IVector3& p_self)
		{
			p_os << p_self.x << " / " << p_self.y << " / " << p_self.z;
			return (p_os);
		}

		IVector3<TType> operator + (const IVector3& p_other) const
		{
			return (IVector3<TType>(x + p_other.x, y + p_other.y, z + p_other.z));
		}
		IVector3<TType> operator - (const IVector3& p_other) const
		{
			return (IVector3<TType>(x - p_other.x, y - p_other.y, z - p_other.z));
		}
		IVector3<TType> operator * (const IVector3& p_other) const
		{
			return (IVector3<TType>(x * p_other.x, y * p_other.y, z * p_other.z));
		}
		IVector3<TType> operator / (const IVector3& p_other) const
		{
			if (p_other.x == 0 || p_other.y == 0 || p_other.z == 0)
				throw std::runtime_error("Trying to divide by 0");
			return (IVector3<TType>(x / p_other.x, y / p_other.y, z / p_other.z));
		}
		void operator += (const IVector3& p_other)
		{
			x += p_other.x;
			y += p_other.y;
			z += p_other.z;
		}
		void operator -= (const IVector3& p_other)
		{
			x -= p_other.x;
			y -= p_other.y;
			z -= p_other.z;
		}
		void operator *= (const IVector3& p_other)
		{
			x *= p_other.x;
			y *= p_other.y;
			z *= p_other.z;
		}
		void operator /= (const IVector3& p_other)
		{
			if (p_other.x == 0 || p_other.y == 0 || p_other.z == 0)
				throw std::runtime_error("Trying to divide by 0");
			x /= p_other.x;
			y /= p_other.y;
			z /= p_other.z;
		}



		template <typename TOtherType>
		bool operator == (const TOtherType& p_other) const
		{
			return (x == static_cast<TType>(p_other) && y == static_cast<TType>(p_other) && z == static_cast<TType>(p_other));
		}
		template <typename TOtherType>
		bool operator != (const TOtherType& p_other) const
		{
			return (x != static_cast<TType>(p_other) || y != static_cast<TType>(p_other) || z != static_cast<TType>(p_other));
		}
		template <typename TOtherType>
		bool operator < (const TOtherType& p_other) const
		{
			if (z < static_cast<TType>(p_other))
				return (true);
			if (z == static_cast<TType>(p_other) && y < static_cast<TType>(p_other))
				return (true);
			if (z == static_cast<TType>(p_other) && y == static_cast<TType>(p_other) && x < static_cast<TType>(p_other))
				return (true);
			return (false);
		}
		template <typename TOtherType>
		bool operator > (const TOtherType& p_other) const
		{
			if (z > static_cast<TType>(p_other))
				return (true);
			if (z == static_cast<TType>(p_other) && y > static_cast<TType>(p_other))
				return (true);
			if (z == static_cast<TType>(p_other) && y == static_cast<TType>(p_other) && x > static_cast<TType>(p_other))
				return (true);
			return (false);
		}
		template <typename TOtherType>
		bool operator <= (const TOtherType& p_other) const
		{
			if (this->operator == (p_other) == true || this->operator < (p_other) == true)
				return (true);
			return (false);
		}
		template <typename TOtherType>
		bool operator >= (const TOtherType& p_other) const
		{
			if (this->operator == (p_other) == true || this->operator > (p_other) == true)
				return (true);
			return (false);
		}


		template <typename TOtherType>
		bool operator == (const IVector3<TOtherType>& p_other) const
		{
			return ((x == static_cast<TType>(p_other.x) && y == static_cast<TType>(p_other.y) && z == static_cast<TType>(p_other.z) ? true : false));
		}
		template <typename TOtherType>
		bool operator != (const IVector3<TOtherType>& p_other) const
		{
			return ((x == static_cast<TType>(p_other.x) && y == static_cast<TType>(p_other.y) && z == static_cast<TType>(p_other.z) ? false : true));
		}
		template <typename TOtherType>
		bool operator < (const IVector3<TOtherType>& p_other) const
		{
			if (z < static_cast<TType>(p_other.z))
				return (true);
			if (z == static_cast<TType>(p_other.z) && y < static_cast<TType>(p_other.y))
				return (true);
			if (z == static_cast<TType>(p_other.z) && y == static_cast<TType>(p_other.y) && x < static_cast<TType>(p_other.x))
				return (true);
			return (false);
		}
		template <typename TOtherType>
		bool operator > (const IVector3<TOtherType>& p_other) const
		{
			if (z > static_cast<TType>(p_other.z))
				return (true);
			if (z == static_cast<TType>(p_other.z) && y > static_cast<TType>(p_other.y))
				return (true);
			if (z == static_cast<TType>(p_other.z) && y == static_cast<TType>(p_other.y) && x > static_cast<TType>(p_other.x))
				return (true);
			return (false);
		}
		template <typename TOtherType>
		bool operator <= (const IVector3<TOtherType>& p_other) const
		{
			if (this->operator == (p_other) == true || this->operator < (p_other) == true)
				return (true);
			return (false);
		}
		template <typename TOtherType>
		bool operator >= (const IVector3<TOtherType>& p_other) const
		{
			if (this->operator == (p_other) == true || this->operator > (p_other) == true)
				return (true);
			return (false);
		}

		static IVector3 lerp(const IVector3& a, const IVector3& b, float t) {
			return IVector3(
				a.x + (b.x - a.x) * t,
				a.y + (b.y - a.y) * t,
				a.z + (b.z - a.z) * t
			);
		}

		template <typename TOtherType>
		float distance(const IVector3<TOtherType>& p_other) const
		{
			return (static_cast<float>(::sqrt(pow(p_other.x - x, 2) + pow(p_other.y - y, 2) + pow(p_other.z - z, 2))));
		}

		float norm() const
		{
			return (sqrt(pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f)));
		}

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

		IVector3<TType> cross(const IVector3<TType>& p_other) const
		{
			return IVector3<TType>(
				y * p_other.z - z * p_other.y,
				z * p_other.x - x * p_other.z,
				x * p_other.y - y * p_other.x
			);
		}

		IVector3<TType> rotate(const IVector3<TType>& p_rotationValues) const
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

		TType dot(const IVector3<TType>& p_other) const
		{
			return x * p_other.x + y * p_other.y + z * p_other.z;
		}

		IVector3 projectOnPlane(const IVector3& p_planeNormal, const IVector3& p_planePoint) const
		{
			IVector3 pointToPlane = *this - p_planePoint;
			float distance = pointToPlane.dot(p_planeNormal);
			return (*this - p_planeNormal * distance);
		}

		IVector2<TType> removeY()
		{
			return (IVector2<TType>(x, z));
		}

		IVector3 inverse()
		{
			return (IVector3(-x, -y, -z));
		}
	};

	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector3<TType> operator+(TValueType p_value, const IVector3<TType>& p_point)
	{
		return (IVector3<TType>(p_value) + p_point);
	};
	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector3<TType> operator-(TValueType p_value, const IVector3<TType>& p_point)
	{
		return (IVector3<TType>(p_value) - p_point);
	};
	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector3<TType> operator*(TValueType p_value, const IVector3<TType>& p_point)
	{
		return (IVector3<TType>(p_value) * p_point);
	};
	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector3<TType> operator/(TValueType p_value, const IVector3<TType>& p_point)
	{
		return (IVector3<TType>(p_value) / p_point);
	};

	using Vector3 = IVector3<float>;
	using Vector3Int = IVector3<int>;
	using Vector3UInt = IVector3<unsigned int>;
};

