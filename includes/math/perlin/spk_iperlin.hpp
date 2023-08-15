#pragma once

#include "miscellaneous/JSON/spk_JSON_object.hpp"
#include <functional>

/**
 * @namespace spk
 * @brief Contains the definitions and implementations related to Perlin noise generation.
 */
namespace spk
{
    /**
     * @class IPerlin
     * @brief Provides functionality related to Perlin noise generation.
     *
     * This class contains methods and data structures needed for Perlin noise generation.
     * Allows to generate Perlin noise with different seed, frequency, persistance, lacunarity, octave value, and interpolation.
     */
	class IPerlin
	{
	public:
        /**
         * @enum Interpolation
         * @brief Enum class representing different types of interpolation methods.
         */
		enum class Interpolation
		{
			Linear, ///< Linear interpolation method.
			SmoothStep ///< SmoothStep interpolation method.
		};

	protected:
        /**
         * @brief The interpolation method used, default is Linear.
         */
		Interpolation _interpolation = Interpolation::Linear;

        /**
         * @brief Size of the permutation table.
         */
		static const size_t PermutationTableSize = 256;

        /**
         * @brief Type alias for the PermutationTable.
         */
		using PermutationTable = unsigned char[PermutationTableSize * 2];

        /**
         * @brief The permutation table used in Perlin noise generation.
         */
		PermutationTable _permutationTable;

        /**
         * @brief Gradient vectors for Perlin noise generation.
         */
		const float _gradients[12][3] = {
			{1, 1, 0}, {-1, 1, 0}, {1, -1, 0}, {-1, -1, 0},
			{1, 0, 1}, {-1, 0, 1}, {1, 0, -1}, {-1, 0, -1},
			{0, 1, 1}, {0, -1, 1}, {0, 1, -1}, {0, -1, -1}
		};

		/**
		 * @brief Seed value used in Perlin noise generation, default is 12500.
		 */
		unsigned long _seed = 12500;

		/**
		 * @brief The minimum limit for the Perlin noise output, default is 0.
		 */
		float _min = 0;

		/**
		 * @brief The maximum limit for the Perlin noise output, default is 100.
		 */
		float _max = 100;

		/**
		 * @brief The range of the Perlin noise output, default is 100.
		 */
		float _range = 100;

		/**
		 * @brief The number of octaves used in Perlin noise generation, default is 3.
		 */
		size_t _octaveValue = 3;

		/**
		 * @brief The frequency of the Perlin noise output, default is 50.0f.
		 */
		float _frequency = 50.0f;

		/**
		 * @brief The persistence of the Perlin noise output, default is 0.5f.
		 */
		float _persistance = 0.5f;

		/**
		 * @brief The lacunarity of the Perlin noise output, default is 2.0f.
		 */
		float _lacunarity = 2.0f;

		/**
     * @brief Updates the Perlin noise parameters when the seed value is edited.
     */
    void _onSeedEdition();

    /**
     * @brief Executes the Perlin noise sampling using a lambda function.
     * @param p_lambda The lambda function to execute the sampling.
     * @return The result of the Perlin noise sampling.
     */
    float _executeSample(const std::function<float(const float& p_frequency)>& p_lambda) const;

    /**
     * @brief Computes the dot product of the gradient and the vector from the input coordinate to the 8 surrounding points in the unit cube.
     * @param ix The x-coordinate of the input point.
     * @param iy The y-coordinate of the input point.
     * @param iz The z-coordinate of the input point.
     * @param x The x-coordinate of the vector from the input coordinate.
     * @param y The y-coordinate of the vector from the input coordinate.
     * @param z The z-coordinate of the vector from the input coordinate.
     * @return The result of the dot product operation.
     */
    float _dotGridGradient(const int& ix, const int& iy, const int& iz, const float& x, const float& y, const float& z) const;
	
    /**
     * @brief Computes the dot product of the gradient and the vector from the input coordinate to the 8 surrounding points in the unit cube.
     * @param ix The x-coordinate of the input point.
     * @param iy The y-coordinate of the input point.
     * @param x The x-coordinate of the vector from the input coordinate.
     * @param y The y-coordinate of the vector from the input coordinate.
     * @return The result of the dot product operation.
     */
	float _dotGridGradient(const int& ix, const int& iy, const float& x, const float& y) const;
	
    /**
     * @brief Computes the dot product of the gradient and the vector from the input coordinate to the 8 surrounding points in the unit cube.
     * @param ix The x-coordinate of the input point.
     * @param x The x-coordinate of the vector from the input coordinate.
     * @return The result of the dot product operation.
     */
    float _dotGridGradient(const int& ix, const float& x) const;

    /**
     * @brief Performs an interpolation operation between two points.
     * @param a0 The first point.
     * @param a1 The second point.
     * @param w The weighting factor.
     * @return The result of the interpolation operation.
     */
    float _interpolate(const float& a0, const float& a1, const float& w) const;

    /**
     * @brief Calculates the ratio for the Perlin noise generation.
     * @param w The weighting factor.
     * @return The calculated ratio.
     */
    float _calcRatio(const float& w) const;

	public:
		/**
		 * @brief Default constructor for the IPerlin class.
		 */
		IPerlin();

		/**
		 * @brief Overloaded constructor for the IPerlin class that takes a seed value.
		 * @param p_seed Seed value for Perlin noise generation.
		 */
		IPerlin(unsigned long p_seed);

		/**
		 * @brief Overloaded constructor for the IPerlin class that takes a JSON object.
		 * @param p_object JSON object containing configuration for Perlin noise generation.
		 */
		IPerlin(const spk::JSON::Object& p_object);

		/**
		 * @brief Gets the seed value used for Perlin noise generation.
		 * @return The current seed value.
		 */
		const unsigned long &seed() const;

		/**
		 * @brief Configures the seed value for Perlin noise generation.
		 * @param p_seed The seed value to be set.
		 */
		void configureSeed(unsigned long p_seed);

		/**
		 * @brief Configures the frequency for Perlin noise generation.
		 * @param p_frequency The frequency to be set.
		 */
		void configureFrequency(float p_frequency);

		/**
		 * @brief Configures the persistence for Perlin noise generation.
		 * @param p_persistance The persistence to be set.
		 */
		void configurePersistance(float p_persistance);

		/**
		 * @brief Configures the lacunarity for Perlin noise generation.
		 * @param p_lacunarity The lacunarity to be set.
		 */
		void configureLacunarity(float p_lacunarity);

		/**
		 * @brief Configures the octave value for Perlin noise generation.
		 * @param p_octaveValue The octave value to be set.
		 */
		void configureOctave(size_t p_octaveValue);

		/**
		 * @brief Configures the range for Perlin noise generation.
		 * @param p_min The minimum limit to be set.
		 * @param p_max The maximum limit to be set.
		 */
		void configureRange(float p_min, float p_max);

		/**
		 * @brief Configures the interpolation method for Perlin noise generation.
		 * @param p_interpolation The interpolation method to be set.
		 */
		void configureInterpolation(Interpolation p_interpolation);

	};

    /**
     * @brief Converts an Interpolation type to a string representation.
     * @param p_interpolation Interpolation type to be converted.
     * @return A string representation of the Interpolation type.
     */
	std::wstring to_wstring(const spk::IPerlin::Interpolation& p_interpolation);
}
