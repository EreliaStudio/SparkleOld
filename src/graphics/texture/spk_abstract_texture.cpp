#include "graphics/texture/spk_abstract_texture.hpp"

namespace spk
{
	/**
	 * @brief Overloads the insertion operator for AbstractTexture::Format.
	 * This allows printing of Format enum values to a wide output stream in a human-readable format.
	 * 
	 * @param p_os Wide output stream to insert into.
	 * @param p_format Format enum value to be converted to a string.
	 * @return Reference to the modified output stream.
	 */
	std::wostream &operator<<(std::wostream &p_os, const AbstractTexture::Format &p_format)
	{
		switch (p_format)
		{
		case AbstractTexture::Format::BGR:
			p_os << L"BGR";
			break;
		case AbstractTexture::Format::BGRA:
			p_os << L"BGRA";
			break;
		case AbstractTexture::Format::RGB:
			p_os << L"RGB";
			break;
		case AbstractTexture::Format::RGBA:
			p_os << L"RGBA";
			break;
		default:
			p_os << L"R";
			break;
		}
		return (p_os);
	}

	/**
	 * @brief Overloads the insertion operator for AbstractTexture::Filtering.
	 * This allows printing of Filtering enum values to a wide output stream in a human-readable format.
	 * 
	 * @param p_os Wide output stream to insert into.
	 * @param p_filtering Filtering enum value to be converted to a string.
	 * @return Reference to the modified output stream.
	 */
	std::wostream &operator<<(std::wostream &p_os, const AbstractTexture::Filtering &p_filtering)
	{
		switch (p_filtering)
		{
		case AbstractTexture::Filtering::Linear:
			p_os << L"BGR";
			break;
		case AbstractTexture::Filtering::LinearMipmapLinear:
			p_os << L"BGRA";
			break;
		case AbstractTexture::Filtering::Nearest:
			p_os << L"Nearest";
			break;
		default:
			p_os << L"NearestMipmap";
			break;
		}
		return (p_os);
	}

	/**
	 * @brief Overloads the insertion operator for AbstractTexture::Wrap.
	 * This allows printing of Wrap enum values to a wide output stream in a human-readable format.
	 * 
	 * @param p_os Wide output stream to insert into.
	 * @param p_wrap Wrap enum value to be converted to a string.
	 * @return Reference to the modified output stream.
	 */
	std::wostream &operator<<(std::wostream &p_os, const AbstractTexture::Wrap &p_wrap)
	{
		switch (p_wrap)
		{
		case AbstractTexture::Wrap::ClampToBorder:
			p_os << L"Clamp to border";
			break;
		case AbstractTexture::Wrap::ClampToEdge:
			p_os << L"Clamp to edge";
			break;
		default:
			p_os << L"Repeat";
			break;
		}
		return (p_os);
	}

	/**
	 * @brief Overloads the insertion operator for AbstractTexture::Mipmap.
	 * This allows printing of Mipmap enum values to a wide output stream in a human-readable format.
	 * 
	 * @param p_os Wide output stream to insert into.
	 * @param p_mipmap Mipmap enum value to be converted to a string.
	 * @return Reference to the modified output stream.
	 */
	std::wostream &operator<<(std::wostream &p_os, const AbstractTexture::Mipmap &p_mipmap)
	{
		switch (p_mipmap)
		{
		case AbstractTexture::Mipmap::Disable:
			p_os << L"No Mipmap";
			break;
		default:
			p_os << L"Mipmap";
			break;
		}
		return (p_os);
	}
}