#include "math/spk_perlin.hpp"

namespace spk
{
	Perlin1D::Perlin1D() :
		IPerlin()
	{

	}

	Perlin1D::Perlin1D(const long long& p_seed) :
		IPerlin(p_seed)
	{

	}
	
	Perlin1D::Perlin1D(const spk::JSON::Object& p_object) :
		IPerlin(p_object)
	{

	}

	float Perlin1D::_computeWaveLength(const float& p_x, const float& p_frequency) const
	{
		float fx = p_x / p_frequency;

		int x0 = std::floor(fx);
		int x1 = x0 + 1;

		float sx = fx - (float)x0;

		float n0, n1, value;
		n0 = _dotGridGradient(x0, 0, 0, fx, 0, 0);
		n1 = _dotGridGradient(x1, 0, 0, fx, 0, 0);
		value = _interpolate(n0, n1, sx);

		return value;
	}

	float Perlin1D::sample(const float& p_x) const
	{
		return (_executeSample([&, p_x](const float& p_frequency){
			return (_computeWaveLength(p_x, p_frequency));
		}));
	}
}
