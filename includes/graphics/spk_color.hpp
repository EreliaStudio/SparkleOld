#pragma once

namespace spk
{
	struct Color
	{
		float r;
		float g;
		float b;
		float a;

		Color() :
			r(0),
			g(0),
			b(0),
			a(1)
		{

		}
		Color(int p_value) : Color(p_value, p_value, p_value)
		{

		}

		Color(int p_r, int p_g, int p_b, int p_a = 255) :
			r(static_cast<float>(p_r) / 255.0f),
			g(static_cast<float>(p_g) / 255.0f),
			b(static_cast<float>(p_b) / 255.0f),
			a(static_cast<float>(p_a) / 255.0f)
		{

		}
		Color(float p_r, float p_g, float p_b, float p_a = 1.0f) :
			r(p_r),
			g(p_g),
			b(p_b),
			a(p_a)
		{

		}

		Color operator + (const Color& p_color) const
		{
			Color result;

			result.r = std::clamp(r + p_color.r, 0.0f, 1.0f);
			result.g = std::clamp(g + p_color.g, 0.0f, 1.0f);
			result.b = std::clamp(b + p_color.b, 0.0f, 1.0f);

			return (result);
		}

		Color operator - (const Color& p_color) const
		{
			Color result;

			result.r = std::clamp(r - p_color.r, 0.0f, 1.0f);
			result.g = std::clamp(g - p_color.g, 0.0f, 1.0f);
			result.b = std::clamp(b - p_color.b, 0.0f, 1.0f);

			return (result);
		}

		friend std::wostream& operator << (std::wostream& p_os, const Color& p_values)
		{
			p_os << p_values.r << " / " << p_values.g << " / " << p_values.b << " / " << p_values.a;
			return (p_os);
		}
	};
}