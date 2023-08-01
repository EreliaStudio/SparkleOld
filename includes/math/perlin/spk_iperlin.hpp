#pragma once

#include "miscellaneous/JSON/spk_JSON_object.hpp"
#include <functional>

namespace spk
{
	class IPerlin
	{
	public:
		enum class Interpolation
		{
			Linear,
			SmoothStep
		};

	protected:		
		Interpolation _interpolation = Interpolation::Linear;

		static const size_t PermutationTableSize = 256;
		using PermutationTable = unsigned char[PermutationTableSize * 2];

		PermutationTable _permutationTable;

		const float _gradients[12][3] = {
			{1, 1, 0}, {-1, 1, 0}, {1, -1, 0}, {-1, -1, 0},
			{1, 0, 1}, {-1, 0, 1}, {1, 0, -1}, {-1, 0, -1},
			{0, 1, 1}, {0, -1, 1}, {0, 1, -1}, {0, -1, -1}
		};

		unsigned long _seed = 12500;

		float _min = 0;
		float _max = 100;

		float _range = 100;

		size_t _octaveValue = 3;

		float _frequency = 50.0f;

		float _persistance = 0.5f;

		float _lacunarity = 2.0f;

		void _onSeedEdition();

		float _executeSample(const std::function<float(const float& p_frequency)>& p_lambda) const;

		float _dotGridGradient(const int& ix, const int& iy, const int& iz, const float& x, const float& y, const float& z) const;
		float _interpolate(const float& a0, const float& a1, const float& w) const;
		float _calcRatio(const float& w) const;

	public:
		IPerlin();
		
		IPerlin(unsigned long p_seed);

		IPerlin(const spk::JSON::Object& p_object);

		const unsigned long &seed() const;

		void configureSeed(unsigned long p_seed);

		void configureFrequency(float p_frequency);

		void configurePersistance(float p_persistance);

		void configureLacunarity(float p_lacunarity);

		void configureOctave(size_t p_octaveValue);

		void configureRange(float p_min, float p_max);

		void configureInterpolation(Interpolation p_interpolation);
	};

	std::wstring to_wstring(const spk::IPerlin::Interpolation& p_interpolation);
}
