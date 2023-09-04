#pragma once

#include "math/spk_vector2.hpp"
#include "graphics/spk_area.hpp"

namespace spk
{
	class AbstractSurface 
	{
	protected:
		std::wstring _title;
		spk::Vector2UInt _size;

		const Viewport* _activeViewport;
		const Scissor* _activeScissor;

		virtual void _onViewportEdition() = 0;
		virtual void _onScissorEdition() = 0;

	public:
		AbstractSurface(const std::wstring& p_title, const spk::Vector2UInt& p_size);
		~AbstractSurface();

		void activateViewport(const Viewport* p_viewport);
		void activateScissor(const Scissor* p_scissor);

		spk::Vector2 convertScreenToGPU(const spk::Vector2Int& p_screenPosition);

		virtual void clear() = 0;
		virtual void resize(const spk::Vector2UInt& p_size) = 0;
		virtual void render() = 0;

		const spk::Vector2UInt& size() const;
		const Viewport* activeViewport() const;
		const Scissor* activeScissor() const;
	};
}