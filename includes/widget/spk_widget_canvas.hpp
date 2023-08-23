#pragma once

#include "widget/spk_widget_interface.hpp"

namespace spk::Widget
{
	/**
	 * @class Canvas
	 * @brief Represents a canvas that holds and manages widgets.
	 * @details This class is responsible for handling widgets, their geometry, rendering, and other related functionalities.
	 */
	class Canvas : public spk::Widget::Interface
	{
	private:
		/**
		 * @struct Geometry
		 * @brief Represents the geometric properties of a widget within the canvas.
		 */
		struct Geometry
		{
			/**
			 * @struct Value
			 * @brief Encapsulates the different types of geometric values.
			 */
			struct Value
			{
			public:
				/**
				 * @enum Type
				 * @brief Enumerates the types of geometric values.
				 */
				enum class Type
				{
					FixedValue, ///< Represents a fixed geometric value.
					Ratio,      ///< Represents a ratio geometric value.
					Cell,       ///< Represents a cell-based geometric value.
					Formula     ///< Represents a formula-based geometric value.
				};

			private:
				// No private members

			public:
				bool active = false;                           ///< Indicates whether the value is active.
				Type type = Type::FixedValue;                  ///< The type of the geometric value.
				spk::Vector2 value = 0;                        ///< The value in vector form.
				spk::IVector2<std::wstring> formulaValue = spk::IVector2<std::wstring>(L"", L""); ///< The value in formula form.

				Value();                                       ///< Default constructor.
				Value(const spk::JSON::Object& p_object);      ///< Constructor that initializes from a JSON object.
			};

			Value anchor; ///< The anchor value.
			Value size;   ///< The size value.

			Geometry();                                 ///< Default constructor.
			Geometry(const spk::JSON::Object& p_object); ///< Constructor that initializes from a JSON object.
		};

		static inline Canvas* _activeCanvas = nullptr; ///< The currently active canvas.

		spk::Vector2Int _gridSize;                     ///< The size of the grid.
		std::map<Interface*, Geometry> _widgetGeometries; ///< Map of widgets and their geometries.

		/**
		 * @brief Calculates the value based on a given formula.
		 * @param p_target Target interface.
		 * @param p_formula The formula.
		 * @return Calculated value.
		 */
		spk::Vector2Int _calcFormula(Interface* p_target, const spk::IVector2<std::wstring>& p_formula);

		/**
		 * @brief Calculates the value based on a given geometric value.
		 * @param p_target Target interface.
		 * @param p_value The geometric value.
		 * @return Calculated value.
		 */
		spk::Vector2Int _calcValue(Interface* p_target, const Geometry::Value& p_value);

		/**
		 * @brief Callback for when geometry changes.
		 */
		void _onGeometryChange();

		/**
		 * @brief Callback for rendering.
		 */
		void _onRender();

		/**
		 * @brief Callback for updates.
		 * @return true if the update was successful, false otherwise.
		 */
		bool _onUpdate();

		/**
		 * @brief Loads the configuration from a JSON object.
		 * @param p_object The JSON object.
		 */
		void _loadConfiguration(const spk::JSON::Object& p_object);

		/**
		 * @brief Loads content from a JSON object.
		 * @param p_object The JSON object.
		 */
		void _loadContent(const spk::JSON::Object& p_object);

	public:
		/**
		 * @brief Constructor that initializes the canvas with a given file path.
		 * @param p_canvasFilePath The file path to the canvas file.
		 */
		Canvas(const std::filesystem::path& p_canvasFilePath);

		/**
		 * @brief Destructor.
		 */
		~Canvas();
	};
}
