#include "math/spk_perlin.hpp"

namespace spk
{
	Perlin2D::Perlin2D() :
		Perlin1D()
	{

	}
	
	Perlin2D::Perlin2D(const long long& p_seed) :
		Perlin1D(p_seed)
	{

	}
	
	Perlin2D::Perlin2D(const spk::JSON::Object& p_object) :
		Perlin1D(p_object)
	{

	}

	float Perlin2D::_computeWaveLength(const float& p_x, const float& p_y, const float& p_frequency) const
	{
		float fx = p_x / p_frequency;
		float fy = p_y / p_frequency;

		int x0 = std::floor(fx);
		int x1 = x0 + 1;
		int y0 = std::floor(fy);
		int y1 = y0 + 1;

		float sx = fx - (float)x0;
		float sy = fy - (float)y0;

		float n0, n1, ix0, ix1, value;
		n0 = _dotGridGradient(x0, y0, 0, fx, fy, 0);
		n1 = _dotGridGradient(x1, y0, 0, fx, fy, 0);
		ix0 = _interpolate(n0, n1, sx);
		n0 = _dotGridGradient(x0, y1, 0, fx, fy, 0);
		n1 = _dotGridGradient(x1, y1, 0, fx, fy, 0);
		ix1 = _interpolate(n0, n1, sx);
		value = _interpolate(ix0, ix1, sy);

		return value;
	}

	float Perlin2D::sample(const float& p_x, const float& p_y) const
	{
		return (_executeSample([&, p_x, p_y](const float& p_frequency){
			return (_computeWaveLength(p_x, p_y, p_frequency));
		}));
	}
}
