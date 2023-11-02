#pragma once

#include "math/spk_vector2.hpp"
#include "graphics/spk_texture.hpp"

namespace spk
{
	class Image : public Texture
	{
	private:
		uint8_t *_textureData;
		spk::Vector2Int _size;
		int _channels;

	public:
		Image(const std::filesystem::path& p_path);
	};
}