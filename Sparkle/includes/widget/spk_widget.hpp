#pragma once

#include "design_pattern/spk_tree_node.hpp"
#include "design_pattern/spk_activable_object.hpp"
#include "graphics/spk_viewport.hpp"
#include "profiler/spk_time_consumption_metrics.hpp"

namespace spk
{
	class WidgetModule;

	/**
	 * @class Widget
	 * @brief Abstract base class for widgets.
	 * 
	 * This class defines the interface and common functionality for widgets.
	 * It inherits from TreeNode and ActivableObject.
	 */
	class Widget : public spk::TreeNode<Widget>, public spk::ActivableObject
	{
		friend class spk::WidgetModule;

	private:
		virtual void _onRender() = 0;
		virtual bool _onUpdate() = 0;

		virtual void _onGeometryChange() = 0;

		bool _isOperationnal = false;
		CallbackContainer::Contract _activationContract;
		CallbackContainer::Contract _deactivationContract;
		spk::TimeConsumption& _timeConsomptionMetrics;

		std::wstring _name;
		bool _geometryEdited;
		float _depth = 0;
		spk::Viewport _viewport;
		spk::Vector2 _anchorRatio;
		spk::Vector2 _sizeRatio;
		
		virtual void _render();

		virtual bool _update();

		void _setOperationnal();

		void _computeResizeRatio();
		void _applyResizeOperation();

	public:
		Widget(const std::wstring& p_name);	
		Widget(const std::wstring& p_name, const spk::JSON::Object& p_inputObject);
		~Widget();

		Widget(const Widget& p_other) = delete;
		Widget& operator=(const Widget& p_other) = delete;

		Widget(Widget&& p_other) noexcept;
		Widget& operator=(Widget&& p_other);

		/**
		 * @brief Set the anchor of the widget.
		 * @param p_anchor The anchor position of the widget.
		 */
		void setAnchor(const spk::Vector2Int& p_anchor);

		/**
		 * @brief Set the size of the widget.
		 * @param p_size The size of the widget.
		 */
		void setSize(const spk::Vector2UInt& p_size);

		/**
		 * @brief Set the geometry of the widget.
		 * @param p_anchor The anchor position of the widget.
		 * @param p_size The size of the widget.
		 */
		void setGeometry(const spk::Vector2Int& p_anchor, const spk::Vector2UInt& p_size);

		/**
		 * @brief Set the geometry of the widget.
		 * @param p_area The area in which the widget should render itself.
		 */
		void setGeometry(const spk::Area& p_area);
		
		/**
		 * @brief Place the widget at a specific anchor position.
		 * @param p_anchor The new anchor position of the widget.
		 */
		void place(const spk::Vector2Int& p_anchor);
		
		/**
		 * @brief Move the widget by a specific amount.
		 * @param p_delta The amount to move the widget by.
		 */
		void move(const spk::Vector2Int& p_delta);

		/**
		 * @brief Resize the widget to a new size.
		 * @param p_size The new size of the widget.
		 */
		void resize(const spk::Vector2UInt& p_size);


		/**
		 * @brief Set the depth value of the widget.
		 * @param p_depth The depth value of the widget.
		 */
		void setDepth(const float& p_depth);

		/**
		 * @brief Rename the widget.
		 * @param p_name The new name of the widget.
		 */
		void rename(const std::wstring& p_name);

		spk::Widget* searchWidget(const std::wstring& p_name) const;

		/**
		 * @brief Get the name of the widget.
		 * @return The name of the widget.
		 */
		constexpr const std::wstring& name() const {return (_name);}

		/**
		 * @brief Get the viewport of the widget
		 * @return The viewport of the widget, relative to its parent.
		*/
		spk::Viewport& viewport() { return (_viewport);}
		
		/**
		 * @brief Get the viewport of the widget
		 * @return The viewport of the widget, relative to its parent.
		*/
		constexpr const spk::Viewport& viewport() const { return (_viewport);}

		/**
		 * @brief Get the area value of the widget.
		 * @return The area of the widget.
		 */
		constexpr const spk::Area& area() const { return (_viewport.area());	}

		/**
		 * @brief Get the size value of the widget.
		 * @return The size of the widget.
		 */
		constexpr const spk::Vector2UInt& size() const { return (_viewport.area().size());	}

		/**
		 * @brief Get the anchor value of the widget.
		 * @return The anchor of the widget, relative to the parent of said widget.
		 */
		constexpr const spk::Vector2Int& anchor() const { return (_viewport.area().anchor());	}

		/**
		 * @brief Get the depth value of the widget.
		 * @return The depth value of the widget.
		 */
		constexpr const float& depth() const { return (_depth);	}

		/**
		 * @brief Check if a point is inside the widget's boundaries.
		 * @param p_point The point to check.
		 * @return True if the point is inside the widget, false otherwise.
		 */
		bool isPointed(const spk::Vector2Int& p_point);
	};

	/**
	 * @class NoGraphicsWidget
	 * @brief Class for widgets without graphics.
	 * 
	 * This class extends the Widget class and provides an implementation for widgets without graphics.
	 */
	class NoGraphicsWidget : public Widget
	{
	private:
		/**
		 * @brief A private method that is an empty implementation for rendering.
		 */
		virtual void _onRender() {}

		/**
		 * @brief A private method that is an empty implementation for responding to geometry changes.
		 */
		virtual void _onGeometryChange() {}

		/**
		 * @brief A private method that is an empty implementation for rendering.
		 */
		virtual void _render() { }

	public:
		/**
		 * @brief Constructor for the NoGraphicsWidget class.
		 *
		 * @param p_name The name of the widget.
		 */
		NoGraphicsWidget(const std::wstring& p_name) :
			Widget(p_name)
		{}
	};

	/**
	 * @class OnlyGraphicsWidget
	 * @brief Class for widgets with only graphics.
	 * 
	 * This class extends the Widget class and provides an implementation for widgets with only graphics.
	 */
	class OnlyGraphicsWidget : public Widget
	{
	private:
		/**
		 * @brief A private method that is an empty implementation for updating. Returns false as there is no update.
		 * 
		 * @return false
		 */
		virtual bool _onUpdate() { return (false); }

		/**
		 * @brief A private method that is an empty implementation for updating. Returns false as there is no update.
		 * 
		 * @return false
		 */
		virtual bool _update() { return (false); }

	public:
		/**
		 * @brief Constructor for the OnlyGraphicsWidget class.
		 *
		 * @param p_name The name of the widget.
		 */
		OnlyGraphicsWidget(const std::wstring& p_name) :
			Widget(p_name)
		{}
	};


};
