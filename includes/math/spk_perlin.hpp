#pragma once

#include "math/spk_random_generator.hpp"
#include "iostream/spk_iostream.hpp"

namespace spk
{
    /**
     * @class Perlin
     * @brief A class to generate Perlin Noise.
     * 
     * The Perlin class generates Perlin noise, which is a type of gradient noise often used in procedural texture and terrain generation. It uses a pseudo-random number generator to create a smooth but non-repeating pattern in three dimensions.
     */
	class Perlin
	{
	private:
        /// Pseudo-random number generator.
		spk::RandomGenerator<int> _rng;

        /// Size of the gradient vector.
		static const size_t GradiantSize = 10;

        /// 3D gradient vector.
		float _gradiants[GradiantSize][GradiantSize][GradiantSize][3];

        /// Seed value for the random number generator.
		unsigned long _seed = 12500;

        /// Minimum and maximum values for the range of noise.
		float _min = 0;
		float _max = 100;

        /// Range of the noise.
		float _range = 100;

        /// Number of octaves in the noise function.
		size_t _octaveValue = 3;

        /// Frequency of the first octave.
		float _frequency = 50.0f;

        /// Persistance value of the noise function.
		float _persistance = 0.5f;

        /// Lacunarity value of the noise function.
		float _lacunarity = 2.0f;

		/**
		 * @brief Perform a smooth step interpolation.
		 * 
		 * @param w The value to be interpolated.
		 * @return float Interpolated value.
		 */
        float _smoothstep(float w);

		/**
		 * @brief Interpolate between two values using a weight.
		 * 
		 * @param a0 The first value.
		 * @param a1 The second value.
		 * @param w The weight.
		 * @return float Interpolated result.
		 */
        float _interpolate(float a0, float a1, float w);
		
		/**
		 * @brief Compute the dot product of the gradient vector and the vector from the input coordinate to the 8 surrounding points in the grid.
		 * 
		 * @param ix Integer x coordinate.
		 * @param iy Integer y coordinate.
		 * @param iz Integer z coordinate.
		 * @param x Float x coordinate.
		 * @param y Float y coordinate.
		 * @param z Float z coordinate.
		 * @return float The computed dot product.
		 */
        float _dotGridGradient(int ix, int iy, int iz, float x, float y, float z);
		
		/**
		 * @brief Compute the wavelength of the Perlin noise at a given point.
		 * 
		 * @param p_x x-coordinate of the point.
		 * @param p_y y-coordinate of the point.
		 * @param p_z z-coordinate of the point.
		 * @param p_frequency Frequency of the Perlin noise.
		 * @return float The wavelength at the given point.
		 */
        float _computeWaveLength(float p_x, float p_y, float p_z, float p_frequency);

		/**
		 * @brief Calculate the gradient vectors for the grid points in the Perlin noise function.
		 * 
		 * The function populates the 3D array of gradient vectors, _gradiants, with pseudorandom values generated using the random number generator _rng. Each gradient vector has three components (x, y, z).
		 */
        void _calcGradiant();

	public:
        /**
         * @brief Construct a new Perlin object.
         * 
         * @param p_seed Seed value for the random number generator. Default value is 12500.
         */
		Perlin(unsigned long p_seed = 12500);

        /**
         * @brief Get the seed value.
         * 
         * @return const unsigned long& Reference to the seed value.
         */
		const unsigned long &seed() const;

        /**
         * @brief Set the seed value.
         * 
         * @param p_seed New seed value.
         */
		void configureSeed(unsigned long p_seed);

        /**
         * @brief Set the frequency of the first octave.
         * 
         * @param p_frequency New frequency value.
         */
		void configureFrequency(float p_frequency);

        /**
         * @brief Set the persistence value.
         * 
         * @param p_persistance New persistence value.
         */
		void configurePersistance(float p_persistance);

        /**
         * @brief Set the lacunarity value.
         * 
         * @param p_lacunarity New lacunarity value.
         */
		void configureLacunarity(float p_lacunarity);

        /**
         * @brief Set the number of octaves.
         * 
         * @param p_octaveValue New number of octaves.
         */
		void configureOctave(size_t p_octaveValue);

        /**
         * @brief Set the range of the noise.
         * 
         * @param p_min Minimum value of the range.
         * @param p_max Maximum value of the range.
         */
		void configureRange(float p_min, float p_max);

        /**
         * @brief Get a sample from the noise.
         * 
         * @param p_x X-coordinate.
         * @param p_y Y-coordinate. Default is 0.
         * @param p_z Z-coordinate. Default is 0.
         * 
         * @return float The sample from the noise.
         */
		float sample(float p_x, float p_y = 0, float p_z = 0);
	};
}
