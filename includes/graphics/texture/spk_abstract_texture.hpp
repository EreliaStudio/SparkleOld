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
			R,
			GreyLevel
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
		AbstractTexture() = default;
		AbstractTexture(const AbstractTexture& p_other) = delete;
		AbstractTexture& operator = (const AbstractTexture& p_other) = delete;

		virtual void bind(int p_textureIndex) const = 0;
		virtual void unbind() const = 0;

		friend std::wostream& operator << (std::wostream& p_os, const Format& p_format);
		friend std::wostream& operator << (std::wostream& p_os, const Filtering& p_format);
		friend std::wostream& operator << (std::wostream& p_os, const Wrap& p_format);
		friend std::wostream& operator << (std::wostream& p_os, const Mipmap& p_format);
	};
}