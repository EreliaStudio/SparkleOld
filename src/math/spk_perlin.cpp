#include "math/spk_perlin.hpp"

namespace spk
{
	float Perlin::_computeWaveLength(float p_x, float p_y, float p_z, float p_frequency)
	{
		p_x /= p_frequency;
		p_y /= p_frequency;
		p_z /= p_frequency;

		Vector3 directions[12] = {
			Vector3(1, 1, 0), Vector3(-1, 1, 0), Vector3(1, -1, 0), Vector3(-1, -1, 0),
			Vector3(1, 0, 1), Vector3(-1, 0, 1), Vector3(1, 0, -1), Vector3(-1, 0, -1),
			Vector3(0, 1, 1), Vector3(0, -1, 1), Vector3(0, 1, -1), Vector3(0, -1, -1)};

		Vector3 points[8] = {
			Vector3((float)(p_x), (float)(p_y), (float)(p_z)),
			Vector3((float)(p_x + 1), (float)(p_y), (float)(p_z)),
			Vector3((float)(p_x), (float)(p_y + 1), (float)(p_z)),
			Vector3((float)(p_x + 1), (float)(p_y + 1), (float)(p_z)),
			Vector3((float)(p_x), (float)(p_y), (float)(p_z + 1)),
			Vector3((float)(p_x + 1), (float)(p_y), (float)(p_z + 1)),
			Vector3((float)(p_x), (float)(p_y + 1), (float)(p_z + 1)),
			Vector3((float)(p_x + 1), (float)(p_y + 1), (float)(p_z + 1))};

		Vector3 targetPoint = Vector3(p_x, p_y, p_z);
		Vector3 targetDelta = targetPoint - points[0];

		int indices[8];
		_rng.setDistributionRange(0, 11);

		for (int i = 0; i < 8; i++)
		{
			int ii = static_cast<int>(points[i].x) & 255;
			int jj = static_cast<int>(points[i].y) & 255;
			int kk = static_cast<int>(points[i].z) & 255;

			indices[i] = _rng() % 12; // Get random index
		}

		float dotProducts[8];
		for (int i = 0; i < 8; i++)
		{
			dotProducts[i] = directions[indices[i]].dot(targetPoint - points[i]);
		}

		// Compute smooth factors for interpolation
		float smoothX = 3 * targetDelta.x * targetDelta.x - 2 * targetDelta.x * targetDelta.x * targetDelta.x;
		float smoothY = 3 * targetDelta.y * targetDelta.y - 2 * targetDelta.y * targetDelta.y * targetDelta.y;
		float smoothZ = 3 * targetDelta.z * targetDelta.z - 2 * targetDelta.z * targetDelta.z * targetDelta.z;

		// Perform trilinear interpolation
		float x1 = dotProducts[0] + smoothX * (dotProducts[1] - dotProducts[0]);
		float x2 = dotProducts[2] + smoothX * (dotProducts[3] - dotProducts[2]);
		float y1 = x1 + smoothY * (x2 - x1);
		x1 = dotProducts[4] + smoothX * (dotProducts[5] - dotProducts[4]);
		x2 = dotProducts[6] + smoothX * (dotProducts[7] - dotProducts[6]);
		float y2 = x1 + smoothY * (x2 - x1);

		return y1 + smoothZ * (y2 - y1);
	}

	Perlin::Perlin(unsigned long p_seed) : _seed(p_seed),
										   _rng(p_seed),
										   _min(),
										   _max(),
										   _range(),
										   _octaveValue(3),
										   _frequency(50),
										   _persistance(0.5f),
										   _lacunarity(2.0f)
	{
	}

	void Perlin::configureSeed(unsigned long p_seed)
	{
		_seed = p_seed;
		_rng = spk::RandomGenerator<int>(p_seed);
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
		p_x = std::fabs(p_x + _seed);
		p_y = std::fabs(p_y + _seed);
		p_z = std::fabs(p_z + _seed);

		float result = 0;
		float amplitude = 1.0f;
		float frequency = _frequency;

		for (size_t i = 0; i < _octaveValue; i++)
		{
			result += _computeWaveLength(p_x, p_y, p_z, frequency) * amplitude;
			amplitude *= _persistance;
			frequency /= _lacunarity;
		}

		result += 1;
		result /= 2;
		result *= _range;
		result += _min;

		return (result);
	}
}
