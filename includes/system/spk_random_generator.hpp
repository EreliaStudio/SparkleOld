#pragma once

#include <random>

namespace spk
{
	template < typename IntType = int >
	class RandomGenerator
	{
		private:
			std::mt19937 _generator;
			std::uniform_int_distribution< IntType > _distribution;

		public:
			RandomGenerator() :
				_generator(std::random_device()()),
				_distribution()
			{}

			RandomGenerator(const uint64_t& p_seed) :
				_generator(p_seed),
				_distribution()
			{}

			RandomGenerator(const RandomGenerator& p_other) = delete;

			void setDistributionRange(const IntType& p_min, const IntType& p_max)
			{
				_distribution.param(typename std::uniform_int_distribution< IntType >::param_type(p_min, p_max));
			}

			constexpr IntType operator()() { return _distribution(_generator); }

			constexpr IntType min() const { return _distribution.min(); }
			constexpr IntType max() const { return _distribution.max(); }
	};
}
