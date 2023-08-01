#pragma once

#include "math/perlin/spk_perlin_1D.hpp"

#include "math/spk_vector2.hpp"

namespace spk
{
	class Perlin2D : public Perlin1D
	{
	private:
		float _computeWaveLength(const float& p_x, const float& p_y, const float& p_frequency) const;
		float _dotGridGradient(const int& ix, const int& iy, const float& x, const float& y) const;

	public:
		Perlin2D(const long long& p_seed = 12500);
		Perlin2D(const spk::JSON::Object& p_object);

		float sample(const float& p_x, const float& p_y) const;

		template<typename TVectorType>
		float sample(const spk::IVector2<TVectorType>& p_position) const
		{
			return (sample(p_position.x, p_position.y));
		}
	};
}