#pragma once

#include "design_pattern/spk_inherence_object.hpp"
#include "design_pattern/spk_activable_object.hpp"
#include "graphics/spk_viewport.hpp"
#include "application/modules/spk_widget_module.hpp"

namespace spk::Widget
{
	class Atlas;

	/**
	 * @class Interface
	 * @brief Abstract base class for widgets.
	 * 
	 * This class defines the interface and common functionality for widgets.
	 * It inherits from InherenceObject and ActivableObject.
	 */
	class Interface : public spk::InherenceObject<Interface>, public spk::ActivableObject
	{
		friend class spk::WidgetModule;
		friend class spk::Widget::Atlas;

	private:
		virtual void _onRender() = 0;
		virtual bool _onUpdate() = 0;

		virtual void _onGeometryChange() = 0;

		bool _isOperationnal = false;
		Contract _activationCallback;
		Contract _deactivationCallback;

		std::wstring _name;
		bool _geometryEdited;
		float _depth = 0;
		spk::Viewport _viewport;
		spk::Vector2 _anchorRatio;
		spk::Vector2 _sizeRatio;
		
		virtual void _render();

		virtual bool _update();

		void addChild(Child child);

		void _setOperationnal();

		void _computeResizeRatio();
		void resize();

	public:
		/**
		 * @brief Construct a new Interface object with a specified name.
		 * @param p_name The name of the widget.
		 */
		Interface(const std::wstring& p_name);

		/**
		 * @brief Construct a new Interface object with a specified name and input data.
		 * @param p_name The name of the widget.
		 * @param p_inputObject The JSON object provided to construct this Interface.
		 */
		Interface(const std::wstring& p_name, const spk::JSON::Object& p_inputObject);

		/**
		 * @brief Destroy the Interface object.
		 */
		virtual ~Interface();

		/**
		 * @brief Add a children widget to this widget.
		 * @tparam TChildrenType The type of the children widget.
		 * @tparam Args The types of the arguments for constructing the children widget.
		 * @param p_args The arguments for constructing the children widget.
		 * @return Pointer to the children widget.
		 */
		template <typename TChildrenType, typename ... Args>
		std::shared_ptr<TChildrenType> addChildrenWidget(Args&& ... p_args)
		{
			TChildrenType * result = new TChildrenType(std::forward<Args>(p_args)...);

			addChild(result);
			result->setDepth(depth() - 0.025f);

			return (std::shared_ptr<TChildrenType>(result, [](TChildrenType*) {}));
		}

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

		/**
		 * @brief Get the name of the widget.
		 * @return The name of the widget.
		 */
		constexpr const std::wstring& name() const {return (_name);}

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
	 * @class NoGraphics
	 * @brief Class for widgets without graphics.
	 * 
	 * This class extends the Interface class and provides an implementation for widgets without graphics.
	 */
	class NoGraphics : public Interface
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
		 * @brief Constructor for the NoGraphics class.
		 *
		 * @param p_name The name of the widget.
		 */
		NoGraphics(const std::wstring& p_name) :
			Interface(p_name)
		{}
	};

	/**
	 * @class OnlyGraphics
	 * @brief Class for widgets with only graphics.
	 * 
	 * This class extends the Interface class and provides an implementation for widgets with only graphics.
	 */
	class OnlyGraphics : public Interface
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
		 * @brief Constructor for the OnlyGraphics class.
		 *
		 * @param p_name The name of the widget.
		 */
		OnlyGraphics(const std::wstring& p_name) :
			Interface(p_name)
		{}
	};


};
