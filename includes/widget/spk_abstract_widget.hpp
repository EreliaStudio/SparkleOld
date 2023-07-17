#pragma once

#include "design_pattern/spk_inherence_object.hpp"
#include "design_pattern/spk_activable_object.hpp"
#include "math/spk_vector2.hpp"

namespace spk
{
	/**
	 * @class AbstractWidget
	 * @brief Abstract base class for widgets.
	 * 
	 * This class defines the interface and common functionality for widgets.
	 * It inherits from InherenceObject and ActivableObject.
	 */
	class AbstractWidget : public spk::InherenceObject<AbstractWidget>, public spk::ActivableObject
	{
		friend class WidgetModule;
		friend class WidgetAtlas;

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
		spk::Vector2Int _anchor;
		spk::Vector2Int _size;
		
		virtual void _render();

		virtual bool _update();

		void addChild(Child child);

		void _setOperationnal();

	public:
		/**
		 * @brief Construct a new AbstractWidget object with a specified name.
		 * 
		 * This constructor creates a new AbstractWidget object with the specified name.
		 * 
		 * @param p_name The name of the widget.
		 */
		AbstractWidget(const std::wstring& p_name);

		/**
		 * @brief Destroy the AbstractWidget object.
		 * 
		 * This is the destructor for the AbstractWidget class.
		 */
		virtual ~AbstractWidget();

		/**
		 * @brief Add a children widget to this widget.
		 * 
		 * This function adds a children widget to this widget and returns a pointer to it.
		 * 
		 * @tparam TChildrenType The type of the children widget.
		 * @tparam Args The types of the arguments for constructing the children widget.
		 * @param p_args The arguments for constructing the children widget.
		 * @return Pointer to the children widget.
		 */
		template <typename TChildrenType, typename ... Args>
		TChildrenType* addChildrenWidget(Args&& ... p_args)
		{
			TChildrenType * result = new TChildrenType(std::forward<Args>(p_args)...);

			addChild(result);
			result->setDepth(depth() + 1);

			return (result);
		}

		/**
		 * @brief Set the geometry of the widget.
		 * 
		 * This function sets the geometry of the widget, including the anchor position and size.
		 * 
		 * @param p_anchor The anchor position of the widget.
		 * @param p_size The size of the widget.
		 */
		void setGeometry(const spk::Vector2Int& p_anchor, const spk::Vector2Int& p_size);
		
		/**
		 * @brief Place the widget at a specific anchor position.
		 * 
		 * This function places the widget at a specific anchor position, while keeping its current size.
		 * 
		 * @param p_anchor The new anchor position of the widget.
		 */
		void place(const spk::Vector2Int& p_anchor);
		
		/**
		 * @brief Move the widget by a specific amount.
		 * 
		 * This function moves the widget by a specific amount, while keeping its current size.
		 * 
		 * @param p_anchor The amount to move the widget by.
		 */
		void move(const spk::Vector2Int& p_anchor);

		/**
		 * @brief Resize the widget to a new size.
		 * 
		 * This function resizes the widget to a new size, while keeping its current anchor position.
		 * 
		 * @param p_size The new size of the widget.
		 */
		void resize(const spk::Vector2Int& p_size);


		/**
		 * @brief Set the depth value of the widget.
		 * 
		 * This function sets the depth value of the widget. Widgets with higher depth values will be rendered on top of widgets with lower depth values.
		 * 
		 * @param p_depth The depth value of the widget.
		 */
		void setDepth(const float& p_depth);

		/**
		 * @brief Rename the widget.
		 * 
		 * This function renames the widget with a new name.
		 * 
		 * @param p_name The new name of the widget.
		 */
		void rename(const std::wstring& p_name);

		/**
		 * @brief Get the name of the widget.
		 * 
		 * This function returns the name of the widget.
		 * 
		 * @return The name of the widget.
		 */
		constexpr const std::wstring& name() const {return (_name);}

		/**
		 * @brief Get the depth value of the widget.
		 * 
		 * This function returns the depth value of the widget.
		 * 
		 * @return The depth value of the widget.
		 */
		constexpr const float& depth() const { return (_depth);	}

		/**
		 * @brief Check if a point is inside the widget's boundaries.
		 * 
		 * This function checks if a given point is inside the boundaries of the widget.
		 * 
		 * @param p_point The point to check.
		 * @return True if the point is inside the widget, false otherwise.
		 */
		bool isPointed(const spk::Vector2Int& p_point);
	};

	/**
	 * @class NoGraphicsWidget
	 * @brief Class for widgets without graphics.
	 * 
	 * This class extends the AbstractWidget class and provides an implementation for widgets without graphics.
	 */
	class NoGraphicsWidget : public AbstractWidget
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
			AbstractWidget(p_name)
		{}
	};

	/**
	 * @class OnlyGraphicsWidget
	 * @brief Class for widgets with only graphics.
	 * 
	 * This class extends the AbstractWidget class and provides an implementation for widgets with only graphics.
	 */
	class OnlyGraphicsWidget : public AbstractWidget
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
			AbstractWidget(p_name)
		{}
	};


};
