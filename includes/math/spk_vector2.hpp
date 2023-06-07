#pragma once

namespace spk
{
	template <typename TType>
	struct IVector2
	{
		TType x;
		TType y;

		IVector2()
		{}

		IVector2(TType p_value) : x(p_value)
		{}

		IVector2(TType x, TType y) : x(x), y(y)
		{}

		IVector2<TType> operator+(const IVector2<TType> &other) const
		{
			return IVector2<TType>(x + other.x, y + other.y);
		}

		IVector2<TType> operator-(const IVector2<TType> &other) const
		{
			return IVector2<TType>(x - other.x, y - other.y);
		}

		operator IVector2<int>() const
		{
			return IVector2<int>(static_cast<int>(x), static_cast<int>(y));
		}

		operator IVector2<float>() const
		{
			return IVector2<float>(static_cast<float>(x), static_cast<float>(y));
		}
	};

	using Vector2 = IVector2<float>;
	using Vector2Int = IVector2<int>;

}