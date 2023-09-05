#pragma once

#include "graphics/surface/spk_abstract_surface.hpp"

namespace spk::vulkan
{
	class Surface : public AbstractSurface
	{
	private:
		void _onViewportEdition();
		void _onScissorEdition();

	public:
		Surface(const std::wstring& p_title, const spk::Vector2UInt& p_size);
		~Surface();

		void checkGPUError(const std::wstring& p_informationMessage);
		
		void clear();
		void resize(const spk::Vector2UInt& p_size);
		void render();
	};
}