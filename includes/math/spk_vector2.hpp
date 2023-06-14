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
	template <typename TType>
	struct IVector2
	{
		TType x;
		TType y;

		IVector2() : x(0), y(0) { }

		template <typename TOtherType>
		IVector2(const TOtherType& p_value) : x(p_value), y(p_value) {}

		template <typename TOtherType>
		IVector2(const TOtherType& p_x, const TOtherType& p_y) : x(p_x), y(p_y) {}

		template <typename TOtherType>
		operator IVector2<TOtherType>()
		{
			return (IVector2<TOtherType>(static_cast<TOtherType>(x), static_cast<TOtherType>(y)));
		}

		friend std::wostream& operator << (std::wostream& p_os, const IVector2& p_self)
		{
			p_os << p_self.x << " / " << p_self.y;
			return (p_os);
		}

		template <typename TOtherType>
		IVector2<TType> operator + (const IVector2<TOtherType>& p_other) const
		{
			return (IVector2<TType>(x + p_other.x, y + p_other.y));
		}

		template <typename TOtherType>
		IVector2<TType> operator - (const IVector2<TOtherType>& p_other) const
		{
			return (IVector2<TType>(x - p_other.x, y - p_other.y));
		}

		template <typename TOtherType>
		IVector2<TType> operator * (const IVector2<TOtherType>& p_other) const
		{
			return (IVector2<TType>(x * p_other.x, y * p_other.y));
		}

		template <typename TOtherType>
		IVector2<TType> operator / (const IVector2<TOtherType>& p_other) const
		{
			if (p_other.x == 0 || p_other.y == 0)
				throw std::runtime_error("Trying to divide by 0");
			return (IVector2<TType>(x / p_other.x, y / p_other.y));
		}

		template <typename TOtherType>
		void operator += (const IVector2<TOtherType>& p_other)
		{
			x += p_other.x;
			y += p_other.y;
		}

		template <typename TOtherType>
		void operator -= (const IVector2<TOtherType>& p_other)
		{
			x -= p_other.x;
			y -= p_other.y;
		}

		template <typename TOtherType>
		void operator *= (const IVector2<TOtherType>& p_other)
		{
			x *= p_other.x;
			y *= p_other.y;
		}

		template <typename TOtherType>
		void operator /= (const IVector2<TOtherType>& p_other)
		{
			if (p_other.x == 0 || p_other.y == 0)
				throw std::runtime_error("Trying to divide by 0");
			x /= p_other.x;
			y /= p_other.y;
		}

		template <typename TOtherType>
		bool operator == (const IVector2<TOtherType>& delta) const
		{
			return (x == delta.x && y == delta.y);
		}

		template <typename TOtherType>
		bool operator != (const IVector2<TOtherType>& delta) const
		{
			return (x != delta.x || y != delta.y);
		}

		template <typename TOtherType>
		bool operator < (const IVector2<TOtherType>& p_other) const
		{
			if (y < p_other.y)
				return (true);
			if (y == p_other.y && x < p_other.x)
				return (true);
			return (false);
		}

		template <typename TOtherType>
		bool operator > (const IVector2<TOtherType>& p_other) const
		{
			if (y > p_other.y)
				return (true);
			if (y == p_other.y && x > p_other.x)
				return (true);
			return (false);
		}

		template <typename TOtherType>
		bool operator <= (const IVector2<TOtherType>& p_other) const
		{
			if (x <= p_other.x && y <= p_other.y)
				return (true);
			return (false);
		}

		template <typename TOtherType>
		bool operator >= (const IVector2<TOtherType>& p_other) const
		{
			if (x >= p_other.x && y >= p_other.y)
				return (true);
			return (false);
		}

		float norm() const
		{
			return (sqrt(pow(x, 2.0f)
				+ pow(y, 2.0f)));
		}

		template <typename TOtherType>
		float distance(const IVector2<TOtherType>& p_other) const
		{
			return (static_cast<float>(std::sqrt(pow(p_other.x - x, 2) + pow(p_other.y - y, 2))));
		}

		IVector2<float> normalize() const
		{
			float tmp_norm;

			tmp_norm = this->norm();

			if (tmp_norm == 0)
				std::runtime_error("Trying to divide by 0");
			return (IVector2<float>(static_cast<float>(x) / tmp_norm, static_cast<float>(y) / tmp_norm));
		}

		template <typename TOtherType>
		IVector2 cross(const IVector2<TOtherType>& p_other = IVector2(0, 0)) const
		{
			IVector2 result;

			result = IVector2(-(p_other.y - y), p_other.x - x);

			return (result);
		}

		template <typename TOtherType>
		float crossZ(const IVector2<TOtherType>& p_other) const
		{
			return (x * p_other.y - y * p_other.x);
		}

		template <typename TOtherType>
		float dot(const IVector2<TOtherType>& p_other, const IVector2<TOtherType>& p_center = IVector2(0, 0)) const
		{
			float result;

			result = (x - p_center.x) * (p_other.x - p_center.x) + (y - p_center.y) * (p_other.y - p_center.y);

			return (result);
		}

		template <typename TOtherType>
		float angle(const IVector2<TOtherType>& p_other, const IVector2<TOtherType>& p_center = IVector2(0, 0)) const
		{
			float rdot = dot(p_other, p_center);

			rdot = (rdot < -1.0f ? -1.0f : (rdot > 1.0f ? 1.0f : rdot));

			float angle = std::acos(rdot);

			return (angle * (180 / M_PI));
		}

		IVector2 invert() const
		{
			return (IVector2(x * -1, y * -1));
		}

		template <typename TOtherType>
		IVector2 rotate(const IVector2<TOtherType>& p_center, const float& p_angle) const
		{
			float theta = p_angle * (M_PI / 180);

			float pcos = cos(theta);
			float psin = sin(theta);

			IVector2 result;
			result.x = (x - p_center.x) * pcos - (y - p_center.y) * psin;
			result.y = (x - p_center.x) * psin + (y - p_center.y) * pcos;

			return (result + p_center);
		}

		static IVector2 floor(const IVector2& p_vector)
		{
			IVector2 result;

			result.x = ::floorf(p_vector.x);
			result.y = ::floorf(p_vector.y);

			return (result);
		}

		static IVector2 ceiling(const IVector2& p_vector)
		{
			IVector2 result;

			result.x = ::ceilf(p_vector.x);
			result.y = ::ceilf(p_vector.y);

			return (result);
		}

		static IVector2 round(const IVector2& p_vector)
		{
			IVector2 result;

			result.x = ::roundf(p_vector.x);
			result.y = ::roundf(p_vector.y);

			return (result);
		}

		static IVector2 min(const IVector2& p_min, const IVector2& p_max)
		{
			return (IVector2(std::min(p_min.x, p_max.x), std::min(p_min.y, p_max.y)));
		}

		static IVector2 max(const IVector2& p_min, const IVector2& p_max)
		{
			return (IVector2(std::max(p_min.x, p_max.x), std::max(p_min.y, p_max.y)));
		}

		static bool isInsideRectangle(const IVector2& p_point, const IVector2& p_cornerA, const IVector2& p_cornerB)
		{
			IVector2 tmpMin = IVector2::min(p_cornerA, p_cornerB);
			IVector2 tmpMax = IVector2::max(p_cornerA, p_cornerB);

			if (p_point.x < tmpMin.x || p_point.y < tmpMin.y ||
				p_point.x > tmpMax.x || p_point.y > tmpMax.y)
				return (false);
			return (true);
		}

		static IVector2 lerp(const IVector2& p_startingPoint, const IVector2& p_endingPoint, float t) {
			return IVector2(
				p_startingPoint.x + (p_endingPoint.x - p_startingPoint.x) * t,
				p_startingPoint.y + (p_endingPoint.y - p_startingPoint.y) * t
			);
		}
	};

	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector2<TType> operator+(TValueType p_value, const IVector2<TType>& p_point)
	{
		return (IVector2<TType>(p_value, p_value) + p_point);
	};
	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector2<TType> operator-(TValueType p_value, const IVector2<TType>& p_point)
	{
		return (IVector2<TType>(p_value, p_value) - p_point);
	};
	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector2<TType> operator*(TValueType p_value, const IVector2<TType>& p_point)
	{
		return (IVector2<TType>(p_value, p_value) * p_point);
	};
	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector2<TType> operator/(TValueType p_value, const IVector2<TType>& p_point)
	{
		return (IVector2<TType>(p_value, p_value) / p_point);
	};


	using Vector2 = IVector2<float>;
	using Vector2Int = IVector2<int>;
	using Vector2UInt = IVector2<unsigned int>;
}

