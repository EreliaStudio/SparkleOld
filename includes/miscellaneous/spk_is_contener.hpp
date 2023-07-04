#pragma once
#include <type_traits>

namespace spk
{
	// Définition de base pour tester si un type est un conteneur
	template <typename T, typename = void>
	struct IsContainer : std::false_type {};

	// Spécialisation pour les types qui sont des conteneurs
	template <typename T>
	struct IsContainer<T, std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end()), typename T::value_type>> : std::true_type {};
}