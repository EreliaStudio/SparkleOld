#include "graphics/surface/vulkan/spk_vulkan_surface.hpp"

namespace spk::vulkan
{
	Surface::Surface(const std::wstring& p_title, const spk::Vector2UInt& p_size) :
		AbstractSurface(p_title, p_size)
	{

	}
	
	Surface::~Surface()
	{

	}

	void Surface::_onViewportEdition()
	{

	}
		
	void Surface::_onScissorEdition()
	{

	}

	void Surface::clear()
	{
		spk::cout << "Clearing vulkan surface" << std::endl;
	}
	
	void Surface::resize(const spk::Vector2UInt& p_size)
	{
		spk::cout << "Resizing vulkan surface" << std::endl;
	}

	void Surface::render()
	{
		spk::cout << "Render vulkan surface" << std::endl;
	}
}