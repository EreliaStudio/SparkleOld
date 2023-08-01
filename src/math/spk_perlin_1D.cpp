#include "math/spk_perlin.hpp"

namespace spk
{
	Perlin1D::Perlin1D(const long long& p_seed) :
		IPerlin(p_seed)
	{

	}
	
	Perlin1D::Perlin1D(const spk::JSON::Object& p_object) :
		IPerlin(p_object)
	{

	}

	float Perlin1D::_dotGridGradient(const int& ix, const float& x) const
	{
		size_t firstDim = (ix % PermutationTableSize) % PermutationTableSize;
		unsigned int hash = _permutationTable[firstDim ] % 12;

		// Use the hashed index to look up a gradient vector
		const float gradients[12][3] = {
			{1, 1, 0}, {-1, 1, 0}, {1, -1, 0}, {-1, -1, 0},
			{1, 0, 1}, {-1, 0, 1}, {1, 0, -1}, {-1, 0, -1},
			{0, 1, 1}, {0, -1, 1}, {0, 1, -1}, {0, -1, -1}
		};
		const float* gradient = gradients[hash];

		// Compute the distance vector
		float dx = x - (float)ix;

		// Compute the dot-product
		return (dx * gradient[0]);
	}

	float Perlin1D::_computeWaveLength(const float& p_x, const float& p_frequency) const
	{
		return 0;
	}

	float Perlin1D::sample(const float& p_x) const
	{
		return (_executeSample([&, p_x](const float& p_frequency){
			return (_computeWaveLength(p_x, p_frequency));
		}));
	}
}
