#pragma once

#include "math/perlin/spk_iperlin.hpp"

namespace spk
{
	class Perlin1D : public IPerlin
	{
	private:
		float _computeWaveLength(const float& p_x, const float& p_frequency) const;
		float _dotGridGradient(const int& ix, const float& x) const;
		
	public:
		Perlin1D(const long long& p_seed = 12500);
		Perlin1D(const spk::JSON::Object& p_object);

		float sample(const float& p_x) const;
	};
}