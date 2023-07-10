#pragma once
#include <type_traits>

namespace spk
{
	/**
	 * @struct IsContainer
	 * @brief Metafunction for checking if a type is a container.
	 *
	 * @tparam T Type to be checked.
	 * @tparam Enable Placeholder parameter for SFINAE.
	 *
	 * This structure template will derive from `std::false_type` for non-container types.
	 */
	template <typename T, typename = void>
	struct IsContainer : std::false_type {};


	/**
	 * @struct IsContainer
	 * @brief Metafunction (specialized) for checking if a type is a container.
	 *
	 * @tparam T Type to be checked.
	 *
	 * This structure template will derive from `std::true_type` for container types.
	 * A type is considered a container if it defines `begin()`, `end()`, and has a nested `value_type`.
	 */
	template <typename T>
	struct IsContainer<T, std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end()), typename T::value_type>> : std::true_type {};
}