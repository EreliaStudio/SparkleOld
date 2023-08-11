#pragma once

#include "spk_basic_functions.hpp"

namespace spk
{
	template <typename TType, size_t TLenght>
	struct ContentArray
	{
	public:
		static const size_t Lenght = TLenght;

	protected:
		TType _content[Lenght];

	public:
		ContentArray()
		{
			for (size_t i = 0; i < Lenght; i++)
			{
				_content[i] = TType();
			}
		}

		TType* content()
		{
			return (_content);
		}

		TType& operator[](size_t p_index)
		{
			if (p_index >= Lenght)
				spk::throwException(L"Can't access index [" + std::to_wstring(p_index) + L"] on a ContentArray of size [" + std::to_wstring(Lenght) + L"]");
			return (_content[p_index]);
		}

		const TType& operator[](size_t p_index) const
		{
			if (p_index >= Lenght)
				spk::throwException(L"Can't access index [" + std::to_wstring(p_index) + L"] on a ContentArray of size [" + std::to_wstring(Lenght) + L"]");
			return (_content[p_index]);
		}
	};
}