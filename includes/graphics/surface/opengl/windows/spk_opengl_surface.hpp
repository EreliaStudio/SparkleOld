#pragma once

#include "math/spk_vector2.hpp"
#include "graphics/surface/spk_abstract_surface.hpp"

namespace spk::OpenGL
{
	class Surface : public AbstractSurface
	{
	private:
		HGLRC _hrc;
		HDC _hdc;
		PIXELFORMATDESCRIPTOR _pfd;

	public:
		Surface(const std::wstring& p_title, const spk::Vector2UInt& p_size);

		void clear();
		void resize(const spk::Vector2UInt& p_size);
		void render();
	};
}