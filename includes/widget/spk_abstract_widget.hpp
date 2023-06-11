#pragma once

#include "design_pattern/spk_inherence_object.hpp"
#include "design_pattern/spk_activable_object.hpp"
#include "math/spk_vector2.hpp"

namespace spk
{
	class AbstractWidget : public spk::InherenceObject<AbstractWidget>, public spk::ActivableObject
	{
		friend class WidgetModule;

	private:
		virtual void _onRender() = 0;
		virtual bool _onUpdate() = 0;

		virtual void _onGeometryChange() = 0;

		std::wstring _name;
		bool _geometryEdited;
		float _depth;
		spk::Vector2Int _anchor;
		spk::Vector2Int _size;
		
		virtual void _render();

		virtual bool _update();


	public:
		AbstractWidget(std::wstring p_name);

		~AbstractWidget();

		template <typename TChildrenType, typename ... Args>
		TChildrenType* addChildrenWidget(Args&& ... p_args);

		void setGeometry(spk::Vector2Int p_anchor, spk::Vector2Int p_size);
		
		void place(spk::Vector2Int p_anchor);
		
		void move(spk::Vector2Int p_anchor);

		void resize(spk::Vector2Int p_size);

		void sortChildrensByDepth();

		void setDepth(const float& p_depth);

		void rename(const std::wstring& p_name);

		constexpr const std::wstring& name() const {return (_name);}
		constexpr const float& depth() const { return (_depth);	}

		bool isPointed(spk::Vector2Int p_point);
	};

	class NoGraphicsWidget : public AbstractWidget
	{
	private:
		virtual void _onRender() {}

		virtual void _onGeometryChange() {}

		virtual void _render() { }
	};

	class OnlyGraphicsWidget : public AbstractWidget
	{
	private:
		virtual bool _onUpdate() { return (false); }
		virtual bool _update() { return (false); }
	};

};