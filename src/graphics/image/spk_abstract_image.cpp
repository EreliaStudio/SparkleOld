#include "graphics/image/spk_abstract_image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "external_libraries/stb_image.h"

namespace spk
{
	AbstractImage::AbstractImage(const std::filesystem::path& p_filePath, Filtering p_filtering, Wrap p_wrap, Mipmap p_mipmap)  : 
		_filtering(p_filtering),
		_wrap(p_wrap),
		_mipmap(p_mipmap)
	{
		loadFromFile(p_filePath);
	}

	AbstractImage::AbstractImage(const std::filesystem::path& p_filePath, Mipmap p_mipmap, Filtering p_filtering, Wrap p_wrap) :
		AbstractImage(p_filePath, p_filtering, p_wrap, p_mipmap)
	{

	}

	AbstractImage::~AbstractImage()
	{

	}

	void AbstractImage::loadFromFile(const std::filesystem::path& p_filePath)
	{
		_imageData = stbi_load(p_filePath.string().c_str(), &_size.x, &_size.y, &_channels, 0);
	}

	spk::Vector2Int AbstractImage::size() const {
		return _size;
	}
	
	AbstractImage::Filtering AbstractImage::filtering() const
	{
		return _filtering;
	}

	AbstractImage::Wrap AbstractImage::wrap() const
	{ 
		return _wrap;
	}

	AbstractImage::Mipmap AbstractImage::mipmap() const
	{
		return _mipmap;
	}
}