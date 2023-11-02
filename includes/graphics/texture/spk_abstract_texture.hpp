#pragma once

#include <iostream>

namespace spk
{
	class AbstractTexture
	{
	public:
		enum class Format
		{
			RGB,
			RGBA,
			BGR,
			BGRA,
			R
		};

		enum class Filtering
		{
			Nearest,
			Linear,
			NearestMipmapNearest,
			LinearMipmapLinear
		};

		enum class Wrap
		{
			Repeat,
			ClampToEdge,
			ClampToBorder
		};

		enum class Mipmap
		{
			Disable,
			Enable
		};

	private:

	public:

		virtual void bind(int p_textureIndex) = 0;
		virtual void unbind() = 0;

		friend std::wostream& operator << (std::wostream& p_os, const Format& p_format);
		friend std::wostream& operator << (std::wostream& p_os, const Filtering& p_format);
		friend std::wostream& operator << (std::wostream& p_os, const Wrap& p_format);
		friend std::wostream& operator << (std::wostream& p_os, const Mipmap& p_format);
	};
}