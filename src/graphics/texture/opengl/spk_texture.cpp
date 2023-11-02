#include "graphics/texture/opengl/spk_texture.hpp"

namespace spk
{
	GLenum toOpenGLFormat(const AbstractTexture::Format& p_format)
	{
		switch(p_format)
		{
			case AbstractTexture::Format::RGB: 
				return GL_RGB;
			case AbstractTexture::Format::RGBA: 
				return GL_RGBA;
			case AbstractTexture::Format::BGR: 
				return GL_BGR;
			case AbstractTexture::Format::BGRA: 
				return GL_BGRA;
			case AbstractTexture::Format::R: 
				return GL_R;
			default:
				return GL_RGBA;
		}
	}

	GLint toOpenGLFiltering(const AbstractTexture::Filtering& p_filtering)
	{
		switch(p_filtering)
		{
			case AbstractTexture::Filtering::Nearest: 
				return GL_NEAREST;
			case AbstractTexture::Filtering::Linear: 
				return GL_LINEAR;
			default:
				return GL_NEAREST;
		}
	}

	GLint toOpenGLWrap(const AbstractTexture::Wrap& p_wrap)
	{
		switch(p_wrap)
		{
			case AbstractTexture::Wrap::Repeat: 
				return GL_REPEAT;
			case AbstractTexture::Wrap::ClampToEdge: 
				return GL_CLAMP_TO_EDGE;
			case AbstractTexture::Wrap::ClampToBorder: 
				return GL_CLAMP_TO_BORDER;
			default:
				return GL_REPEAT;
		}
	}

	Texture::Texture() :
		_loaded(false)
	{

	}

	Texture::Texture(const uint8_t* p_textureData, const spk::Vector2UInt& p_textureSize,
		const Format& p_format, const Filtering& p_filtering,
		const Wrap& p_wrap, const Mipmap& p_mipmap)
	{
		load(p_textureData, p_textureSize, p_format, p_filtering, p_wrap, p_mipmap);
	}

	Texture::~Texture()
	{
		if (_loaded == true)
		{
			glDeleteTextures(1, &_textureID);
			_loaded = false;
		}
	}

	void Texture::load(const uint8_t* p_textureData, const spk::Vector2UInt& p_textureSize,
		const Format& p_format, const Filtering& p_filtering,
		const Wrap& p_wrap, const Mipmap& p_mipmap)
	{
		glGenTextures(1, &_textureID);
		glBindTexture(GL_TEXTURE_2D, _textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, toOpenGLFormat(p_format), p_textureSize.x, p_textureSize.y, 0, toOpenGLFormat(p_format), GL_UNSIGNED_BYTE, p_textureData);

		GLint glFiltering = toOpenGLFiltering(p_filtering);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFiltering);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFiltering);

		GLint glWrap = toOpenGLWrap(p_wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrap);

		if(p_mipmap == AbstractTexture::Mipmap::Enable)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		_loaded = true;
	}

	void Texture::bind(int p_textureIndex)
	{  
		glActiveTexture(GL_TEXTURE0 + p_textureIndex);
		glBindTexture(GL_TEXTURE_2D, _textureID);
	}

	void Texture::unbind()
	{
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}