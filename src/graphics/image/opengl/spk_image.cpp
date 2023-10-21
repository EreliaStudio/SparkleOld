#include "graphics/image/opengl/spk_image.hpp"

namespace spk
{
	Image::Image(const std::filesystem::path& p_filePath, Filtering p_filtering, Wrap p_wrap, Mipmap p_mipmap) :
		AbstractImage(p_filePath, p_filtering, p_wrap, p_mipmap)
	{
		uploadToGPU();
	}

	Image::Image(const std::filesystem::path& p_filePath, Mipmap p_mipmap, Filtering p_filtering, Wrap p_wrap) :
		Image(p_filePath, p_filtering, p_wrap, p_mipmap)
	{

	}

	Image::~Image()
	{
		releaseGPUResources();
	}

	GLint toOpenGLFiltering(AbstractImage::Filtering p_filtering)
	{
		switch(p_filtering)
		{
			case AbstractImage::Filtering::Nearest: 
				return GL_NEAREST;
			case AbstractImage::Filtering::Linear: 
				return GL_LINEAR;
			default:
				return GL_NEAREST;
		}
	}

	GLint toOpenGLWrap(AbstractImage::Wrap p_wrap)
	{
		switch(p_wrap)
		{
			case AbstractImage::Wrap::Repeat: 
				return GL_REPEAT;
			case AbstractImage::Wrap::ClampToEdge: 
				return GL_CLAMP_TO_EDGE;
			case AbstractImage::Wrap::ClampToBorder: 
				return GL_CLAMP_TO_BORDER;
			default:
				return GL_REPEAT;
		}
	}

	void Image::uploadToGPU()
	{
		glGenTextures(1, &_textureId);
		glBindTexture(GL_TEXTURE_2D, _textureId);

		_format = (_channels == 4) ? GL_RGBA : GL_RGB;

		glTexImage2D(GL_TEXTURE_2D, 0, _format, _size.x, _size.y, 0, _format, GL_UNSIGNED_BYTE, _imageData);

		GLint glFiltering = toOpenGLFiltering(filtering());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFiltering);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFiltering);

		GLint glWrap = toOpenGLWrap(wrap());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrap);

		if(mipmap() == AbstractImage::Mipmap::Enable)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		
		delete[] _imageData;
		_imageData = nullptr;
	}

	void Image::bind(int p_textureIndex) {
		glActiveTexture(GL_TEXTURE0 + p_textureIndex);
		glBindTexture(GL_TEXTURE_2D, _textureId);
	}

	void Image::unbind() {
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Image::releaseGPUResources() {
		glDeleteTextures(1, &_textureId);
	}
}
