#include "graphics/texture/spk_abstract_texture.hpp"

namespace spk
{
	std::wostream& operator << (std::wostream& p_os, const AbstractTexture::Format& p_format)
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

	std::wostream& operator << (std::wostream& p_os, const AbstractTexture::Filtering& p_filtering)
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

	std::wostream& operator << (std::wostream& p_os, const AbstractTexture::Wrap& p_wrap)
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

	std::wostream& operator << (std::wostream& p_os, const AbstractTexture::Mipmap& p_mipmap)
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