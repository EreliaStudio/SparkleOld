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
		/**
		 * @brief Constructor for the Widget class using only the widget's name.
		 *
		 * This constructor initializes a Widget with a specific name. It's used for creating
		 * a basic widget with minimal initialization requirements.
		 *
		 * @param p_name A std::wstring representing the name of the widget. This name is used for identification
		 *               and searching within the widget hierarchy.
		 */
		Widget(const std::wstring& p_name);

		/**
		 * @brief Constructor for the Widget class using a name and an input JSON object.
		 *
		 * This constructor initializes a Widget with a name and additional configuration or properties
		 * specified in a JSON object. This allows for more complex initialization using data-driven configurations.
		 *
		 * @param p_name A std::wstring representing the name of the widget.
		 * @param p_inputObject A spk::JSON::Object containing configuration data or properties for initializing the widget.
		 */
		Widget(const std::wstring& p_name, const spk::JSON::Object& p_inputObject);

		/**
		 * @brief Destructor for the Widget class.
		 *
		 * Cleans up any resources or state associated with the Widget. This is called when a Widget instance
		 * is destroyed, either through deletion or when it goes out of scope.
		 */
		~Widget();

		/**
		 * @brief Deleted copy constructor.
		 *
		 * This disables the copying of Widget instances. Widgets are unique and should not be copied directly.
		 * This prevents accidental duplication of Widget instances.
		 *
		 * @param p_other A reference to another Widget instance.
		 */
		Widget(const Widget& p_other) = delete;

		/**
		 * @brief Deleted copy assignment operator.
		 *
		 * This disables the assignment of one Widget to another using the copy assignment operator.
		 * Like the copy constructor, it ensures that Widgets remain unique and not accidentally duplicated.
		 *
		 * @param p_other A reference to another Widget instance.
		 * @return Reference to the current Widget instance.
		 */
		Widget& operator=(const Widget& p_other) = delete;

		/**
		 * @brief Move constructor for the Widget class.
		 *
		 * Enables the transfer of a Widget instance from one variable to another, effectively
		 * transferring ownership and state without duplication. This is used for efficient manipulation
		 * of Widgets, especially in container classes or when returning from functions.
		 *
		 * @param p_other An rvalue reference to another Widget instance. This is usually a temporary or unnamed Widget.
		 */
		Widget(Widget&& p_other) noexcept;

		/**
		 * @brief Move assignment operator for the Widget class.
		 *
		 * Allows for the assignment of a Widget instance using move semantics. This operator transfers
		 * ownership and state from one Widget to another without duplication, similar to the move constructor.
		 *
		 * @param p_other An rvalue reference to another Widget instance.
		 * @return Reference to the current Widget instance.
		 */
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

		/**
		 * @brief Search for a widget by its name within the widget hierarchy.
		 *
		 * This method searches for a widget with the specified name within the hierarchy of widgets.
		 * It traverses the widget tree starting from the current widget and checks each widget's name against
		 * the provided name. The search is recursive, covering all child widgets and their descendants.
		 *
		 * @param p_name The name of the widget to search for. It is a std::wstring representing the unique
		 *               name identifier of the widget.
		 * @return spk::Widget* A pointer to the found widget if a widget with the given name exists within
		 *         the hierarchy. Returns nullptr if no widget with the specified name is found.
		 */
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
     * @brief Specialized widget class for non-visual (non-graphical) widgets.
     * 
     * A NoGraphicsWidget is a type of Widget that does not involve any graphical rendering.
     * It is suitable for widgets that only handle data or logic without needing to display anything.
     * Rendering and geometry change handlers are empty, as no graphical processing is required.
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
     * @brief Specialized widget class for graphical-only widgets.
     * 
     * An OnlyGraphicsWidget is a type of Widget that focuses solely on graphical content.
     * It does not handle any update logic, making it ideal for static visual elements in the UI.
     * The update methods return false to indicate no updates are required for this widget type.
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
