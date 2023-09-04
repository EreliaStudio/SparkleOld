#pragma once

#include "graphics/surface/spk_abstract_surface.hpp"

namespace spk
{
	class OpenGLSurface : public AbstractSurface
	{
	private:

	public:
		OpenGLSurface(const std::wstring& p_title, const spk::Vector2UInt& p_size) :
			AbstractSurface(p_title, p_size)
		{}

		void clear();
		void resize(const spk::Vector2UInt& p_size);
		void render();
	};
}