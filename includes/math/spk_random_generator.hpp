#pragma once

#include <random>

namespace spk
{
	/**
	 * @brief Random number generator template class.
	 * @tparam TGeneratedType The type of integer to generate (default: int).
	 */
	template < typename TGeneratedType = int >
	class RandomGenerator
	{
		private:
			using GeneratorType = typename std::conditional<
				std::is_same<TGeneratedType, double>::value || std::is_same<TGeneratedType, float>::value,
				std::uniform_real_distribution<TGeneratedType>,
				std::uniform_int_distribution<TGeneratedType>
			>::type;

			std::mt19937 _generator; // Mersenne Twister 19937 generator.
			GeneratorType _distribution; // Uniform distribution.

		public:
			/**
			 * @brief Default constructor. Initializes the generator with a random seed.
			 */
			RandomGenerator() :
				_generator(std::random_device()()),
				_distribution()
			{}

			/**
			 * @brief Constructor that allows to set the seed manually.
			 * @param p_seed Seed for the random number generator.
			 */
			RandomGenerator(const unsigned int& p_seed) :
				_generator(p_seed),
				_distribution()
			{}

			// Disallow copying
			RandomGenerator(const RandomGenerator& p_other) = delete;

			/**
			 * @brief Set the range of the distribution.
			 * @param p_min The lower bound of the distribution range.
			 * @param p_max The upper bound of the distribution range.
			 */
			void setDistributionRange(const TGeneratedType& p_min, const TGeneratedType& p_max)
			{
				_distribution.param(typename GeneratorType::param_type(p_min, p_max));
			}

			/**
			 * @brief Generate a random number.
			 * @return A random number within the set distribution range.
			 */
			constexpr TGeneratedType operator()() { return _distribution(_generator); }

			/**
			 * @brief Get the minimum value of the distribution range.
			 * @return The minimum value of the distribution range.
			 */
			constexpr TGeneratedType min() const { return _distribution.min(); }

			/**
			 * @brief Get the maximum value of the distribution range.
			 * @return The maximum value of the distribution range.
			 */
			constexpr TGeneratedType max() const { return _distribution.max(); }
	};
}
