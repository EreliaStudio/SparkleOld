#include "graphics/spk_image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "external_libraries/stb_image.h"

namespace spk
{
	Image::Image(const std::filesystem::path& p_path) :
		_size(0),
		_channels(0),
		_textureData(nullptr),
		Texture()
	{

		_textureData = stbi_load(p_path.string().c_str(), &_size.x, &_size.y, &_channels, 0);

		load(_textureData, _size, (_channels == 3 ? Texture::Format::RGB : Texture::Format::RGBA),
			Texture::Filtering::Nearest,
			Texture::Wrap::Repeat,
			Texture::Mipmap::Disable);
	}
}