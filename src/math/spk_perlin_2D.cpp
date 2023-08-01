#include "math/spk_perlin.hpp"

namespace spk
{
	Perlin2D::Perlin2D(const long long& p_seed) :
		Perlin1D(p_seed)
	{

	}
	
	Perlin2D::Perlin2D(const spk::JSON::Object& p_object) :
		Perlin1D(p_object)
	{

	}

	float Perlin2D::_dotGridGradient(const int& ix, const int& iy, const float& x, const float& y) const
	{
		size_t firstDim = (ix % PermutationTableSize) % PermutationTableSize;
		size_t secondDim = (_permutationTable[ firstDim ] + iy) % PermutationTableSize;
		unsigned int hash = _permutationTable[secondDim ] % 12;

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

		// Compute the dot-product
		return (dx * gradient[0] + dy * gradient[1]);
	}

	float Perlin2D::_computeWaveLength(const float& p_x, const float& p_y, const float& p_frequency) const
	{
		return 0;
	}

	float Perlin2D::sample(const float& p_x, const float& p_y) const
	{
		return (_executeSample([&, p_x, p_y](const float& p_frequency){
			return (_computeWaveLength(p_x, p_y, p_frequency));
		}));
	}
}
