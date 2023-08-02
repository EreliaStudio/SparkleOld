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
	template < typename TGeneratedType = int >
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
		template <typename TOtherType>
		inline uintmax_t _generateValue(const uintmax_t& p_seed, const TOtherType& p_x, const TOtherType& p_y, const TOtherType& p_z) const
		{
			uintmax_t result = p_seed + p_x * 374761393 + p_y * 668265263 + p_z * 982451653; //all constants are prime
			result = (result ^ (result >> 13)) * 1274126177;
			return (result);
		}

		/**
		 * @brief Generates a random number.
		 * @param p_x The x coordinate.
		 * @param p_y The y coordinate.
		 * @param p_z The z coordinate.
		 * @return The generated random number.
		 */
		template <typename TOtherType>
		inline TGeneratedType _generate(const TOtherType& p_x, const TOtherType& p_y, const TOtherType& p_z) const
		{
			uintmax_t value = _generateValue(_seed, p_x, p_y, p_z);

			if constexpr (std::is_floating_point<TGeneratedType>::value)
			{
				return (
					static_cast<TGeneratedType>(spk::positiveModulo(value, _range)) + //before ,
					static_cast<TGeneratedType>(spk::positiveModulo(_generateValue(_seed + 11, p_x, p_y, p_z), _precisionModulo)) / _precisionModulo //After ,
				);
			}
			else
			{
				return (static_cast<TGeneratedType>(spk::positiveModulo(value, _range) + _min));
			}
		}

	public:
		/**
		 * @brief Default constructor that uses a random device to initialize the seed.
		 */
		PseudoRandomGenerator() 
			: _seed(std::random_device()())
		{

		}

		/**
		 * @brief Constructor that takes a seed value.
		 * @param p_seed The seed value.
		 */
		PseudoRandomGenerator(long long p_seed) 
			: _seed(p_seed)
		{

		}

		/**
		 * @brief Constructor that takes a JSON object containing a seed value.
		 * @param p_object The JSON object.
		 */
		PseudoRandomGenerator(const spk::JSON::Object& p_object) 
			: _seed(p_object[L"Seed"].as<long>())
		{

		}

		/**
		 * @brief Configures the seed for the pseudo random number generator.
		 * @param p_seed The seed value.
		 */
		void configureSeed(long long p_seed)
		{
			_seed = p_seed;
		}

		/**
		 * @brief Configures the range of the generated numbers.
		 * @param p_min The minimum value.
		 * @param p_max The maximum value.
		 */
		void configureRange(TGeneratedType p_min, TGeneratedType p_max)
		{
			if (p_min >= p_max)
				spk::throwException(L"Can set a range with min higher or equal to max");

			_min = p_min;
			_max = p_max;
			_range = _max - _min;
		}

		/**
		 * @brief Configures the precision of the generated numbers, valid only when TGeneratedType is a floating point type.
		 * @param p_precision The precision value.
		 */
		template<typename U = TGeneratedType, std::enable_if_t<std::is_floating_point<U>::value, int> = 0>
		void configurePrecision(uint32_t p_precision)
		{
			_precision = p_precision;
			_precisionModulo = std::pow(10, p_precision);
		}

		/**
		 * @brief Samples a random number using a 3D vector as input.
		 * @param p_vector3 The 3D vector.
		 * @return The sampled random number.
		 */
		template <typename TOtherType>
		TGeneratedType sample(const spk::IVector3<TOtherType>& p_vector3) const {
			return _generate(p_vector3.x, p_vector3.y, p_vector3.z);
		}

		/**
		 * @brief Samples a random number using a 2D vector as input.
		 * @param p_vector2 The 2D vector.
		 * @return The sampled random number.
		 */
		template <typename TOtherType>
		TGeneratedType sample(const spk::IVector2<TOtherType>& p_vector2) const {
			return _generate(p_vector2.x, p_vector2.y, 0.0f);
		}

		/**
		 * @brief Samples a random number using up to three coordinates as input.
		 * @param p_x The x coordinate.
		 * @param p_y The y coordinate.
		 * @param p_z The z coordinate.
		 * @return The sampled random number.
		 */
		template <typename TOtherType>
		TGeneratedType sample(const TOtherType& p_x, const TOtherType& p_y = 0, const TOtherType& p_z = 0) const {
			return _generate(p_x, p_y, p_z);
		}
	};
}