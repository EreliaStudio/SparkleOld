#pragma once
#include <type_traits>
#include <vector>

namespace spk
{
	template <typename T>
	struct isVectorType : std::false_type
	{
	};

	template <typename T, typename Alloc>
	struct isVectorType<std::vector<T, Alloc>> : std::true_type
	{
	};
}