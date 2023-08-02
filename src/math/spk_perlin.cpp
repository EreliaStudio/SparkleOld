#include "math/spk_perlin.hpp"
#include "math/spk_vector3.hpp"

#include "iostream/spk_iostream.hpp"

namespace spk
{
	float Perlin::_smoothstep(float w)
	{
		if (w <= 0.0f)
			return 0.0f;
		if (w >= 1.0f)
			return 1.0f;
		return w * w * (3.0f - 2.0f * w);
	}

	float Perlin::_interpolate(float a0, float a1, float w)
	{
		return a0 + (a1 - a0) * _smoothstep(w);
	}

	float Perlin::_dotGridGradient(int ix, int iy, int iz, float x, float y, float z)
	{
		// Compute the distance vector
		float dx = x - (float)ix;
		float dy = y - (float)iy;
		float dz = z - (float)iz;

		// Compute the dot-product
		return (dx * _gradiants[iz % GradiantSize][iy % GradiantSize][ix % GradiantSize][0] +
				dy * _gradiants[iz % GradiantSize][iy % GradiantSize][ix % GradiantSize][1] +
				dz * _gradiants[iz % GradiantSize][iy % GradiantSize][ix % GradiantSize][2]);
	}

	float Perlin::_computeWaveLength(float p_x, float p_y, float p_z, float p_frequency)
	{
		p_x /= p_frequency;
		p_y /= p_frequency;
		p_z /= p_frequency;

		// Determine grid cell coordinates
		int x0 = static_cast<int>(std::floor(p_x));
		int x1 = x0 + 1;
		int y0 = static_cast<int>(std::floor(p_y));
		int y1 = y0 + 1;
		int z0 = static_cast<int>(std::floor(p_z));
		int z1 = z0 + 1;

		// Determine interpolation weights
		float sx = p_x - (float)x0;
		float sy = p_y - (float)y0;
		float sz = p_z - (float)z0;

		// _Interpolate between grid point gradients
		float n0, n1, ix0, ix1, iy0, iy1, value;
		n0 = _dotGridGradient(x0, y0, z0, p_x, p_y, p_z);
		n1 = _dotGridGradient(x1, y0, z0, p_x, p_y, p_z);
		ix0 = _interpolate(n0, n1, sx);
		n0 = _dotGridGradient(x0, y1, z0, p_x, p_y, p_z);
		n1 = _dotGridGradient(x1, y1, z0, p_x, p_y, p_z);
		ix1 = _interpolate(n0, n1, sx);
		iy0 = _interpolate(ix0, ix1, sy);

		n0 = _dotGridGradient(x0, y0, z1, p_x, p_y, p_z);
		n1 = _dotGridGradient(x1, y0, z1, p_x, p_y, p_z);
		ix0 = _interpolate(n0, n1, sx);
		n0 = _dotGridGradient(x0, y1, z1, p_x, p_y, p_z);
		n1 = _dotGridGradient(x1, y1, z1, p_x, p_y, p_z);
		ix1 = _interpolate(n0, n1, sx);
		iy1 = _interpolate(ix0, ix1, sy);

		value = _interpolate(iy0, iy1, sz);

		return value;
	}

	void Perlin::_calcGradiant()
	{
		for (int i = 0; i < GradiantSize; ++i)
		{
			for (int j = 0; j < GradiantSize; ++j)
			{
				for (int k = 0; k < GradiantSize; ++k)
				{
					for (int l = 0; l < 3; ++l)
					{
						_gradiants[i][j][k][l] = static_cast<float>(_rng()) / 10000.0f;
					}
				}
			}
		}
	}

	Perlin::Perlin(unsigned long p_seed) : _seed(p_seed),
												   _rng(_seed),
												   _min(),
												   _max(),
												   _range(),
												   _octaveValue(3),
												   _frequency(50),
												   _persistance(0.5f),
												   _lacunarity(2.0f)
	{
		configureSeed(_seed);
		_calcGradiant();
	}

	const unsigned long &Perlin::seed() const
	{
		return (_seed);
	}

	void Perlin::configureSeed(unsigned long p_seed)
	{
		_seed = p_seed;
		_rng = spk::RandomGenerator<int>(_seed);
		_rng.setDistributionRange(0, 10000);
		_calcGradiant();
	}
	void Perlin::configureFrequency(float p_frequency)
	{
		_frequency = p_frequency;
	}
	void Perlin::configurePersistance(float p_persistance)
	{
		_persistance = p_persistance;
	}
	void Perlin::configureLacunarity(float p_lacunarity)
	{
		_lacunarity = p_lacunarity;
	}
	void Perlin::configureOctave(size_t p_octaveValue)
	{
		_octaveValue = p_octaveValue;
	}
	void Perlin::configureRange(float p_min, float p_max)
	{
		_min = p_min;
		_max = p_max;
		_range = _max - _min;
	}

	float Perlin::sample(float p_x, float p_y, float p_z)
	{
		float result = 0;
		float amplitude = 1.0f;
		float frequency = _frequency;

		for (size_t i = 0; i < _octaveValue; i++)
		{
			result += _computeWaveLength(p_x, p_y, p_z, frequency) * amplitude;
			amplitude *= _persistance;
			frequency /= _lacunarity;
		}

		// spk::cout << "Result : " << result << std::endl;

		result += 1;
		result /= 2;
		result *= _range;
		result += _min;

		return (result);
	}
}
