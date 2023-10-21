#pragma once

#include <filesystem>
#include "math/spk_vector2.hpp"

namespace spk
{
	class AbstractImage
	{
	public:
		enum class Filtering
		{
			Nearest,
			Linear,
			NearestMipmapNearest,
			LinearMipmapLinear
		};

		enum class Wrap {
			Repeat,
			ClampToEdge,
			ClampToBorder
		};

		enum class Mipmap {
			Disable,
			Enable
		};

	protected:
		uint8_t* _imageData;
		spk::Vector2Int _size;
		int _channels;

		Filtering _filtering;
		Wrap _wrap;
		Mipmap _mipmap;

		void _releaseImage();

	public:
		AbstractImage(const std::filesystem::path& p_filePath, Filtering p_filtering = Filtering::Nearest, Wrap p_wrap = Wrap::Repeat, Mipmap p_mipmap = Mipmap::Disable);
		AbstractImage(const std::filesystem::path& p_filePath, Mipmap p_mipmap, Filtering p_filtering = Filtering::Nearest, Wrap p_wrap = Wrap::Repeat);
		~AbstractImage();

		void loadFromFile(const std::filesystem::path& p_filePath);
		
		virtual void bind(int p_textureIndex = 0) = 0;
		virtual void unbind() = 0;

		virtual void uploadToGPU() = 0;
		virtual void releaseGPUResources() = 0;

		spk::Vector2Int size() const;
		Filtering filtering() const;
		Wrap wrap() const;
		Mipmap mipmap() const;
	};
}