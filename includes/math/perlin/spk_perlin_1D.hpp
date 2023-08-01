#pragma once

#include "math/perlin/spk_iperlin.hpp"

namespace spk
{
	class Perlin1D : public IPerlin
	{
	private:
		float _computeWaveLength(const float& p_x, const float& p_frequency) const;
		
	public:
		Perlin1D();
		Perlin1D(const long long& p_seed);
		Perlin1D(const spk::JSON::Object& p_object);

		float sample(const float& p_x) const;
	};
}