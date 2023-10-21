#pragma once

#include "graphics/image/spk_abstract_image.hpp"
#include "spk_system_define.hpp"

namespace spk
{
	class Image : public AbstractImage
	{
	private:
		GLuint _textureId;
		GLenum _format;

	public:
		Image(const std::filesystem::path& p_filePath, Filtering p_filtering = Filtering::Nearest, Wrap p_wrap = Wrap::Repeat, Mipmap p_mipmap = Mipmap::Disable);
		Image(const std::filesystem::path& p_filePath, Mipmap p_mipmap, Filtering p_filtering = Filtering::Nearest, Wrap p_wrap = Wrap::Repeat);
		~Image();

		void bind(int p_textureIndex = 0) override;
		void unbind() override;

		void uploadToGPU() override;
		void releaseGPUResources() override;
	};
}