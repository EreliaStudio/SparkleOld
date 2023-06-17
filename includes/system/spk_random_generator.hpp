#pragma once

#include <random>

namespace spk
{
	template < typename IntType = int >
	class RandomGenerator
	{
		private:
			std::seed_seq _seed;
			std::mt19937 _generator;
			std::uniform_int_distribution< IntType > _distribution;

		public:
			RandomGenerator() :
				_generator(std::random_device()()),
				_distribution()
			{}

			template < typename... Args >
			RandomGenerator(Args&&... p_args) :
				_seed({std::forward< Args >(p_args)...}),
				_generator(_seed),
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
