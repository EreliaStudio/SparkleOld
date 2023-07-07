#pragma once

#include "math/spk_random_generator.hpp"

namespace spk
{
	class Perlin
	{
	private:
		spk::RandomGenerator<int> _rng;
	
		unsigned long _seed;
		float _min;
		float _max;
		float _range;
		size_t _octaveValue;
		float _frequency;
		float _persistance;
		float _lacunarity;

		float _computeWaveLength(float p_x, float p_y, float p_frequency);

	public:
		Perlin(unsigned long p_seed = 12500);

		const unsigned long& seed() const { return (_seed); }
		void configureSeed(unsigned long p_seed);
		void configureFrequency(float p_frequency);
		void configurePersistance(float p_persistance);
		void configureLacunarity(float p_lacunarity);
		void configureOctave(size_t p_octaveValue);
		void configureRange(float p_min, float p_max);

		float sample(float p_x, float p_y, float p_z);
	};
}