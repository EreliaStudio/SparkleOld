#pragma once

#include <random>
#include "math/spk_vector2.hpp"
#include "math/spk_vector3.hpp"
#include "iostream/spk_iostream.hpp"
#include "spk_basic_functions.hpp"
#include "miscellaneous/JSON/spk_JSON_object.hpp"

namespace spk
{
	/**
	 * @brief The PseudoRandomGenerator class template for generating pseudo random numbers.
	 * 
	 * This class generates pseudo random numbers of a certain type.
	 * It provides control over the seed and the range of generated numbers.
	 * The type of generated numbers is determined by the template parameter, which must be a numeric type.
	 * 
	 * @tparam TGeneratedType The type of the generated numbers. Must be a numeric type.
	 */
	template <typename TGeneratedType = int>
	class PseudoRandomGenerator
	{
		// The type TGeneratedType must be a numeric type.
		static_assert(std::is_arithmetic<TGeneratedType>::value, "TGeneratedType must be numeric inside a PseudoRandomGenerator.");

	private:
		/**
		 * @brief The seed used to initialize the pseudo random number generator.
		 */
		uintmax_t _seed;

		/**
		 * @brief The precision of generated values when the type is a floating point type.
		 */
		uint32_t _precision = 2;

		/**
		 * @brief The modulo value for the precision calculation.
		 */
		uint32_t _precisionModulo = 100;

		/**
		 * @brief The minimum value of the generated numbers.
		 */
		TGeneratedType _min = 0;

		/**
		 * @brief The maximum value of the generated numbers.
		 */
		TGeneratedType _max = 1;

		/**
		 * @brief The range of the generated numbers.
		 */
		TGeneratedType _range = 1;

		/**
		 * @brief Generates a value using the seed and coordinates.
		 * @param p_seed The seed value.
		 * @param p_x The x coordinate.
		 * @param p_y The y coordinate.
		 * @param p_z The z coordinate.
		 * @return The generated value.
		 */
		inline uintmax_t _generateValue(const uintmax_t& p_seed, const TGeneratedType& p_x, const TGeneratedType& p_y, const TGeneratedType& p_z) const;

		/**
		 * @brief Generates a random number.
		 * @param p_x The x coordinate.
		 * @param p_y The y coordinate.
		 * @param p_z The z coordinate.
		 * @return The generated random number.
		 */
		inline TGeneratedType _generate(const TGeneratedType& p_x, const TGeneratedType& p_y, const TGeneratedType& p_z) const;

	public:
		/**
		 * @brief Default constructor that uses a random device to initialize the seed.
		 */
		PseudoRandomGenerator();

		/**
		 * @brief Constructor that takes a seed value.
		 * @param p_seed The seed value.
		 */
		PseudoRandomGenerator(long long p_seed);

		/**
		 * @brief Constructor that takes a JSON object containing a seed value.
		 * @param p_object The JSON object.
		 */
		PseudoRandomGenerator(const spk::JSON::Object& p_object);

		/**
		 * @brief Configures the seed for the pseudo random number generator.
		 * @param p_seed The seed value.
		 */
		void configureSeed(long long p_seed);

		/**
		 * @brief Configures the range of the generated numbers.
		 * @param p_min The minimum value.
		 * @param p_max The maximum value.
		 */
		void configureRange(TGeneratedType p_min, TGeneratedType p_max);

		/**
		 * @brief Configures the precision of the generated numbers, valid only when TGeneratedType is a floating point type.
		 * @param p_precision The precision value.
		 */
		template<typename U = TGeneratedType, std::enable_if_t<std::is_floating_point<U>::value, int> = 0>
		void configurePrecision(uint32_t p_precision);

		/**
		 * @brief Samples a random number using a 3D vector as input.
		 * @param p_vector3 The 3D vector.
		 * @return The sampled random number.
		 */
		TGeneratedType sample(const spk::IVector3<TGeneratedType>& p_vector3) const;

		/**
		 * @brief Samples a random number using a 2D vector as input.
		 * @param p_vector2 The 2D vector.
		 * @return The sampled random number.
		 */
		TGeneratedType sample(const spk::IVector2<TGeneratedType>& p_vector2) const;

		/**
		 * @brief Samples a random number using up to three coordinates as input.
		 * @param p_x The x coordinate.
		 * @param p_y The y coordinate.
		 * @param p_z The z coordinate.
		 * @return The sampled random number.
		 */
		TGeneratedType sample(const TGeneratedType& p_x, const TGeneratedType& p_y = 0, const TGeneratedType& p_z = 0) const;
	};

}
