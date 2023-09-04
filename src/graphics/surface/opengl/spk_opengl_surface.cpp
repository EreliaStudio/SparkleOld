#include "graphics/surface/opengl/spk_opengl_surface.hpp"

namespace spk::OpenGL
{
	Surface::Surface(const std::wstring& p_title, const spk::Vector2UInt& p_size) :
		AbstractSurface(p_title, p_size)
	{

	}

	void Surface::clear()
	{
		spk::cout << "Clearing OpenGL surface" << std::endl;
	}
	
	void Surface::resize(const spk::Vector2UInt& p_size)
	{
		spk::cout << "Resizing OpenGL surface" << std::endl;
	}

	void Surface::render()
	{
		spk::cout << "Render OpenGL surface" << std::endl;
	}
}