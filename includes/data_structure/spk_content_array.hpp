#pragma once

#include "spk_basic_functions.hpp"

namespace spk
{
    /**
     * @brief A template class representing a fixed-length array.
     *
     * This class wraps a fixed-length array and provides basic access functions.
     * The length is defined at compile time.
     *
     * @tparam TType The type of elements in the array.
     * @tparam TLenght The compile-time fixed length of the array.
     */
	template <typename TType, size_t TLenght>
	struct ContentArray
	{
	public:
        /// Represents the fixed length of the array.
		static const size_t Lenght = TLenght;

	protected:
		/// The underlying fixed-length array.
		TType _content[Lenght];

	public:

        /**
         * @brief Default constructor. Initializes each element with its default constructor.
         */
		ContentArray()
		{
			for (size_t i = 0; i < Lenght; i++)
			{
				_content[i] = TType();
			}
		}

        /**
         * @brief Gets a pointer to the underlying array.
         *
         * @return Pointer to the underlying array.
         */
		TType* content()
		{
			return (_content);
		}

        /**
         * @brief Overloaded array subscript operator.
         *
         * Provides access to the elements of the array. Throws an exception if the index is out of bounds.
         *
         * @param p_index The index of the element to access.
         * @return Reference to the element at the specified index.
         */
		TType& operator[](size_t p_index)
		{
			if (p_index >= Lenght)
				spk::throwException(L"Can't access index [" + std::to_wstring(p_index) + L"] on a ContentArray of size [" + std::to_wstring(Lenght) + L"]");
			return (_content[p_index]);
		}

        /**
         * @brief Overloaded array subscript operator (const version).
         *
         * Provides access to the elements of the array. Throws an exception if the index is out of bounds.
         *
         * @param p_index The index of the element to access.
         * @return Const reference to the element at the specified index.
         */
		const TType& operator[](size_t p_index) const
		{
			if (p_index >= Lenght)
				spk::throwException(L"Can't access index [" + std::to_wstring(p_index) + L"] on a ContentArray of size [" + std::to_wstring(Lenght) + L"]");
			return (_content[p_index]);
		}
	};
}
