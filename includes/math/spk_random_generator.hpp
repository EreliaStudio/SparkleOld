#pragma once

#include <random>

namespace spk
{
	/**
	 * @brief Random number generator template class.
	 * @tparam IntType The type of integer to generate (default: int).
	 */
	template < typename IntType = int >
	class RandomGenerator
	{
		private:
			std::mt19937 _generator; // Mersenne Twister 19937 generator.
			std::uniform_int_distribution< IntType > _distribution; // Uniform distribution.

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
			RandomGenerator(const uint64_t& p_seed) :
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
			void setDistributionRange(const IntType& p_min, const IntType& p_max)
			{
				_distribution.param(typename std::uniform_int_distribution< IntType >::param_type(p_min, p_max));
			}

			/**
			 * @brief Generate a random number.
			 * @return A random number within the set distribution range.
			 */
			constexpr IntType operator()() { return _distribution(_generator); }

			/**
			 * @brief Get the minimum value of the distribution range.
			 * @return The minimum value of the distribution range.
			 */
			constexpr IntType min() const { return _distribution.min(); }

			/**
			 * @brief Get the maximum value of the distribution range.
			 * @return The maximum value of the distribution range.
			 */
			constexpr IntType max() const { return _distribution.max(); }
	};
}
