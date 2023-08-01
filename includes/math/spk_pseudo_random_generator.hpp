#pragma once

#include <random>
#include "math/spk_vector2.hpp"
#include "math/spk_vector3.hpp"
#include "iostream/spk_iostream.hpp"
#include "spk_basic_functions.hpp"
#include "miscellaneous/JSON/spk_JSON_object.hpp"

namespace spk
{
	template < typename TGeneratedType = int >
	class PseudoRandomGenerator
	{
		static_assert(std::is_arithmetic<TGeneratedType>::value, "TGeneratedType must be numeric inside a PseudoRandomGenerator.");
	private:
		uintmax_t _seed;
		uint32_t _precision = 2;
		uint32_t _precisionModulo = 100;
		TGeneratedType _min = 0;
		TGeneratedType _max = 1;
		TGeneratedType _range = 1;

		inline uintmax_t _generateValue(const uintmax_t& p_seed, const TGeneratedType& p_x, const TGeneratedType& p_y, const TGeneratedType& p_z) const
		{
			uintmax_t result = p_seed + p_x * 374761393 + p_y * 668265263 + p_z * 982451653; //all constants are prime
			result = (result ^ (result >> 13)) * 1274126177;
			return (result);
		}

		inline TGeneratedType _generate(const TGeneratedType& p_x, const TGeneratedType& p_y, const TGeneratedType& p_z) const
		{
			uint32_t value = _generateValue(_seed, p_x, p_y, p_z);

			if constexpr (std::is_floating_point<TGeneratedType>::value)
			{
				return (
					static_cast<TGeneratedType>(spk::positiveModulo(value, _range)) + //before ,
					static_cast<TGeneratedType>(spk::positiveModulo(_generateValue(_seed + 11, p_x, p_y, p_z), _precisionModulo)) / _precisionModulo //After ,
				);
			}
			else
			{
				return (spk::positiveModulo(value, _range) + _min);
			}
		}

	public:
		PseudoRandomGenerator() 
			: _seed(std::random_device()())
		{

		}
		PseudoRandomGenerator(long long p_seed) 
			: _seed(p_seed)
		{

		}

		PseudoRandomGenerator(const spk::JSON::Object& p_object) 
			: _seed(p_object[L"Seed"].as<long>())
		{

		}

		void configureSeed(long long p_seed)
		{
			_seed = p_seed;
		}

		void configureRange(TGeneratedType p_min, TGeneratedType p_max)
		{
			if (p_min >= p_max)
				spk::throwException(L"Can set a range with min higher or equal to max");

			_min = p_min;
			_max = p_max;
			_range = _max - _min;
		}

		template<typename U = TGeneratedType, std::enable_if_t<std::is_floating_point<U>::value, int> = 0>
		void configurePrecision(uint32_t p_precision)
		{
			_precision = p_precision;
			_precisionModulo = std::pow(10, p_precision);
		}

		TGeneratedType sample(const spk::IVector3<TGeneratedType>& p_vector3) const {
			return _generate(p_vector3.x, p_vector3.y, p_vector3.z);
		}

		TGeneratedType sample(const spk::IVector2<TGeneratedType>& p_vector2) const {
			return _generate(p_vector2.x, p_vector2.y, 0.0f);
		}

		TGeneratedType sample(const TGeneratedType& p_x, const TGeneratedType& p_y = 0, const TGeneratedType& p_z = 0) const {
			return _generate(p_x, p_y, p_z);
		}
	};
}
