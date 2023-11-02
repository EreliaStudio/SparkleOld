#pragma once

#include "spk_system_define.hpp"
#include "math/spk_vector2.hpp"
#include "graphics/texture/spk_abstract_texture.hpp"

namespace spk
{
	class Texture : public AbstractTexture
	{
	private:
		bool _loaded;
		GLuint _textureID;
		
	public:
		Texture();
		Texture(const uint8_t* p_textureData, const spk::Vector2UInt& p_textureSize,
			const Format& p_format, const Filtering& p_filtering,
			const Wrap& p_wrap, const Mipmap& p_mipmap);
		~Texture();

		void load(const uint8_t* p_textureData, const spk::Vector2UInt& p_textureSize,
			const Format& p_format, const Filtering& p_filtering,
			const Wrap& p_wrap, const Mipmap& p_mipmap);

		void bind(int p_textureIndex = 0);
		void unbind();
	};
}