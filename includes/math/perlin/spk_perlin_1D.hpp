#pragma once

#include "math/perlin/spk_iperlin.hpp"

namespace spk
{
	class Perlin1D : public IPerlin
	{
	private:
		/**
		 * @brief Compute a wavelenght using a 1D coordinate and a frequency.
		 * @param p_x the coordinate in the X axis to compute wavelenght from.
		 * @param p_frequency the frequency of the wave to compute.
 		 */
		float _computeWaveLength(const float& p_x, const float& p_frequency) const;
		
	public:
		/**
		 * @brief Default constructor for the Perlin1D class.
		 */
		Perlin1D();

		/**
		 * @brief Overloaded constructor for the Perlin1D class that takes a seed value.
		 * @param p_seed Seed value for Perlin noise generation.
		 */
		Perlin1D(const long long& p_seed);

		/**
		 * @brief Overloaded constructor for the Perlin1D class that takes a JSON object.
		 * @param p_object JSON object containing configuration for Perlin noise generation.
		 */
		Perlin1D(const spk::JSON::Object& p_object);

		/**
		 * @brief Launch the computation of a value based of a position on the X axis.
		 * @param p_x the coordinate in the X axis to compute wavelenght from.
		 */
		float sample(const float& p_x) const;
	};
}