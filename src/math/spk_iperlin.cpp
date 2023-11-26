#include "math/perlin/spk_iperlin.hpp"
#include <random>
#include <cfloat>

namespace spk
{
	IPerlin::IPerlin() :
		_seed(std::random_device()()),
		_min(0),
		_max(100),
		_range(100),
		_octaveValue(3),
		_frequency(50),
		_persistance(0.5f),
		_lacunarity(2.0f)
	{
		_onSeedEdition();
	}

	IPerlin::IPerlin(unsigned long p_seed) :
		_seed(p_seed),
		_min(0),
		_max(100),
		_range(100),
		_octaveValue(3),
		_frequency(50),
		_persistance(0.5f),
		_lacunarity(2.0f)
	{
		_onSeedEdition();
	}
	
	IPerlin::IPerlin(const spk::JSON::Object& p_object)
	{
		if (p_object.isObject() == true)
		{
			_seed = (p_object.contains(L"Seed") == true ? p_object[L"Seed"].as<long>() : 12500);
			_min = static_cast<float>(p_object.contains(L"MininalRange") == true ? p_object[L"MininalRange"].as<double>() : 0.0);
			_max = static_cast<float>(p_object.contains(L"MaximalRange") == true ? p_object[L"MaximalRange"].as<double>() : 100.0);
			_range = _max - _min;
			_octaveValue = (p_object.contains(L"Octave") == true ? p_object[L"Octave"].as<long>() : 3);
			_frequency = static_cast<float>(p_object.contains(L"Frequency") == true ? p_object[L"Frequency"].as<double>() : 50.0);
			_persistance = static_cast<float>(p_object.contains(L"Persistance") == true ? p_object[L"Persistance"].as<double>() : 0.5);
			_lacunarity = static_cast<float>(p_object.contains(L"Lacunarity") == true ? p_object[L"Lacunarity"].as<double>() : 2.0);
		}
		else if (p_object.isArray() == true)
		{
			_seed = (p_object.size() >= 1 ? p_object[0].as<long>() : 12500);
			_octaveValue = (p_object.size() >= 2 ? p_object[3].as<long>() : 3);
			_frequency = static_cast<float>(p_object.size() >= 3 ? p_object[4].as<double>() : 50.0f);
			_persistance = static_cast<float>(p_object.size() >= 4 ? p_object[5].as<double>() : 0.5f);
			_lacunarity = static_cast<float>(p_object.size() >= 5 ? p_object[6].as<double>() : 2.0f);
			_min = static_cast<float>(p_object.size() >= 6 ? p_object[1].as<double>() : 0);
			_max = static_cast<float>(p_object.size() >= 7 ? p_object[2].as<double>() : 100);
			_range = _max - _min;
		}
		else
		{
			spk::throwException(L"Unexpected JSON type while creating a perlin");
		}
		_onSeedEdition();
	}

	float IPerlin::_calcRatio(const float& w) const
	{
		if (w <= 0.0f)
			return 0.0f;
		if (w >= 1.0f)
			return 1.0f;
			
		switch (_interpolation)
		{
			case Interpolation::Linear:
				return w;
			case Interpolation::SmoothStep:
				return w * w * (3.0f - 2.0f * w);				
			default:
				return w;
		}
	}

	float IPerlin::_interpolate(const float& a0, const float& a1, const float& w) const
	{
		return a0 + (a1 - a0) * _calcRatio(w);
	}

	float IPerlin::_dotGridGradient(const int& ix, const int& iy, const int& iz, const float& x, const float& y, const float& z) const
	{
		size_t hashValue = (ix % PermutationTableSize) % PermutationTableSize;
		hashValue = (_permutationTable[ hashValue ] + iy) % PermutationTableSize;
		hashValue = (_permutationTable[ hashValue ] + iz) % PermutationTableSize;
		unsigned int hash = _permutationTable[hashValue ] % 12;

		float result = (x - (float)ix) * _gradients[hash][0];
		if (y != 0)
			result += (y - (float)iy) * _gradients[hash][1];
		if (z != 0)
			result += (z - (float)iz) * _gradients[hash][2];
		return (result);
	}

	float IPerlin::_dotGridGradient(const int& ix, const int& iy, const float& x, const float& y) const
	{
		size_t hashValue = (ix % PermutationTableSize) % PermutationTableSize;
		hashValue = (_permutationTable[ hashValue ] + iy) % PermutationTableSize;
		unsigned int hash = _permutationTable[hashValue ] % 12;

		float result = (x - (float)ix) * _gradients[hash][0];
		result += (y - (float)iy) * _gradients[hash][1];
		return (result);
	}

	float IPerlin::_dotGridGradient(const int& ix, const float& x) const
	{
		size_t hashValue = (ix % PermutationTableSize) % PermutationTableSize;
		unsigned int hash = _permutationTable[hashValue ] % 12;

		float result = (x - (float)ix) * _gradients[hash][0];
		return (result);
	}

	const unsigned long &IPerlin::seed() const
	{
		return (_seed);
	}

	void IPerlin::configureSeed(unsigned long p_seed)
	{
		_seed = p_seed;
		_onSeedEdition();
	}

	void IPerlin::_onSeedEdition()
	{
		for (int i = 0; i < PermutationTableSize; ++i)
		{
			_permutationTable[i] = i;
		}

		std::shuffle(_permutationTable, _permutationTable + PermutationTableSize, std::default_random_engine(_seed));

		std::copy_n(_permutationTable, PermutationTableSize, _permutationTable + PermutationTableSize);
	}

	float IPerlin::_executeSample(const std::function<float(const float& p_frequency)>& p_lambda) const
	{
		float result = 0;
		float amplitude = 1.0f;
		float frequency = _frequency;

		for (size_t i = 0; i < _octaveValue; i++)
		{
			result += p_lambda(frequency) * amplitude;
			amplitude *= _persistance;
			frequency /= _lacunarity;
		}

		result += 1;
		result /= 2;

		result = std::clamp(result, 0.0f, 1.0f - FLT_EPSILON);

		result *= _range;
		result += _min;

		return (result);
	}

	void IPerlin::configureFrequency(float p_frequency)
	{
		_frequency = p_frequency;
	}
	void IPerlin::configurePersistance(float p_persistance)
	{
		if (p_persistance > 1.0f)
			spk::throwException(L"Persistance can't be higher than 1");
		_persistance = p_persistance;
	}
	void IPerlin::configureLacunarity(float p_lacunarity)
	{
		if (p_lacunarity <= 1.0f)
			spk::throwException(L"Lacunarity can't be lower or equal to 1");
		_lacunarity = p_lacunarity;
	}
	void IPerlin::configureOctave(size_t p_octaveValue)
	{
		_octaveValue = p_octaveValue;
	}
	void IPerlin::configureRange(float p_min, float p_max)
	{
		_min = p_min;
		_max = p_max;
		_range = _max - _min;
	}

	void IPerlin::configureInterpolation(Interpolation p_interpolation)
	{
		_interpolation = p_interpolation;
	}
	
	std::wstring to_wstring(const spk::IPerlin::Interpolation& p_interpolation)
	{
		switch (p_interpolation)
		{
			case spk::IPerlin::Interpolation::Linear:
				return (L"Linear");
			case spk::IPerlin::Interpolation::SmoothStep:
				return (L"SmoothStep");
			default:
				return (L"Unknow");
		}
	}
}
