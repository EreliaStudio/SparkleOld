#pragma once

#include "math/spk_random_generator.hpp"

#include "iostream/spk_iostream.hpp"

namespace spk
{
	class Perlin
	{
	private:
		spk::RandomGenerator<int> _rng;
		static const size_t GradiantSize = 10;
		float _gradiants[GradiantSize][GradiantSize][GradiantSize][3];
		unsigned long _seed = 12500;
		float _min = 0;
		float _max = 100;
		float _range = 100;
		size_t _octaveValue = 3;
		float _frequency = 50.0f;
		float _persistance = 0.5f;
		float _lacunarity = 2.0f;

		float _smoothstep(float w);
		float _interpolate(float a0, float a1, float w);
		float _dotGridGradient(int ix, int iy, int iz, float x, float y, float z);
		float _computeWaveLength(float p_x, float p_y, float p_z, float p_frequency);
		void _calcGradiant();

	public:
		Perlin(unsigned long p_seed = 12500);

		const unsigned long &seed() const;

		void configureSeed(unsigned long p_seed);
		void configureFrequency(float p_frequency);
		void configurePersistance(float p_persistance);
		void configureLacunarity(float p_lacunarity);
		void configureOctave(size_t p_octaveValue);
		void configureRange(float p_min, float p_max);

		float sample(float p_x, float p_y = 0, float p_z = 0);
	};
}