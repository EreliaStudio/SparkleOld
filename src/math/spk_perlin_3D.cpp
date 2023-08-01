#include "math/perlin/spk_perlin_3D.hpp"

namespace spk
{
	Perlin3D::Perlin3D(const long long& p_seed) :
		Perlin2D(p_seed)
	{

	}
	
	Perlin3D::Perlin3D(const spk::JSON::Object& p_object) :
		Perlin2D(p_object)
	{

	}

	float Perlin3D::_dotGridGradient(const int& ix, const int& iy, const int& iz, const float& x, const float& y, const float& z) const
	{
		size_t firstDim = (ix % PermutationTableSize) % PermutationTableSize;
		size_t secondDim = (_permutationTable[ firstDim ] + iy) % PermutationTableSize;
		size_t thirdDim = (_permutationTable[ secondDim ] + iz) % PermutationTableSize;
		unsigned int hash = _permutationTable[thirdDim ] % 12;

		// Use the hashed index to look up a gradient vector
		const float gradients[12][3] = {
			{1, 1, 0}, {-1, 1, 0}, {1, -1, 0}, {-1, -1, 0},
			{1, 0, 1}, {-1, 0, 1}, {1, 0, -1}, {-1, 0, -1},
			{0, 1, 1}, {0, -1, 1}, {0, 1, -1}, {0, -1, -1}
		};
		const float* gradient = gradients[hash];

		// Compute the distance vector
		float dx = x - (float)ix;
		float dy = y - (float)iy;
		float dz = z - (float)iz;

		// Compute the dot-product
		return (dx * gradient[0] + dy * gradient[1] + dz * gradient[2]);
	}

	float Perlin3D::_computeWaveLength(const float& p_x, const float& p_y, const float& p_z, const float& p_frequency) const
	{
		float fx = p_x / p_frequency;
		float fy = p_y / p_frequency;
		float fz = p_z / p_frequency;

		// Determine grid cell coordinates
		int x0 = std::floor(fx);
		int x1 = x0 + 1;
		int y0 = std::floor(fy);
		int y1 = y0 + 1;
		int z0 = std::floor(fz);
		int z1 = z0 + 1;

		// Determine interpolation weights
		float sx = fx - (float)x0;
		float sy = fy - (float)y0;
		float sz = fz - (float)z0;

		// _Interpolate between grid point gradients
		float n0, n1, ix0, ix1, iy0, iy1, value;
		n0 = _dotGridGradient(x0, y0, z0, fx, fy, fz);
		n1 = _dotGridGradient(x1, y0, z0, fx, fy, fz);
		ix0 = _interpolate(n0, n1, sx);
		n0 = _dotGridGradient(x0, y1, z0, fx, fy, fz);
		n1 = _dotGridGradient(x1, y1, z0, fx, fy, fz);
		ix1 = _interpolate(n0, n1, sx);
		iy0 = _interpolate(ix0, ix1, sy);

		n0 = _dotGridGradient(x0, y0, z1, fx, fy, fz);
		n1 = _dotGridGradient(x1, y0, z1, fx, fy, fz);
		ix0 = _interpolate(n0, n1, sx);
		n0 = _dotGridGradient(x0, y1, z1, fx, fy, fz);
		n1 = _dotGridGradient(x1, y1, z1, fx, fy, fz);
		ix1 = _interpolate(n0, n1, sx);
		iy1 = _interpolate(ix0, ix1, sy);

		value = _interpolate(iy0, iy1, sz);

		return value;
	}

	float Perlin3D::sample(const float& p_x, const float& p_y, const float& p_z) const
	{
		return (_executeSample([&, p_x, p_y, p_z](const float& p_frequency){
			return (_computeWaveLength(p_x, p_y, p_z, p_frequency));
		}));
	}
}
