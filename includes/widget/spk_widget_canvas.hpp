#pragma once

#include "widget/spk_widget_interface.hpp"

namespace spk::Widget
{
	class Canvas : public spk::Widget::Interface
	{
	private:
		struct Geometry
		{
			struct Value
			{
			public:
				enum class Type
				{
					FixedValue,
					Ratio,
					Cell,
					Formula
				};
			private:

			public:
				bool active = false;
				Type type = Type::FixedValue;
				spk::Vector2 value = 0;
				spk::IVector2<std::wstring> formulaValue = spk::IVector2<std::wstring>(L"", L"");

				Value();
				Value(const spk::JSON::Object& p_object);
			};

			Value anchor;
			Value size;

			Geometry();
			Geometry(const spk::JSON::Object& p_object);
		};

		spk::Vector2Int _gridSize;
		std::map<Interface*, Geometry> _widgetGeometries;

		spk::Vector2Int _calcFormula(Interface* p_target, const spk::IVector2<std::wstring>& p_formula);
		spk::Vector2Int _calcValue(Interface* p_target, const Geometry::Value& p_value);

		void _onGeometryChange();

		void _onRender();
		
		bool _onUpdate();

		void _loadConfiguration(const spk::JSON::Object& p_object);
		void _loadContent(const spk::JSON::Object& p_object);

	public:
		Canvas(const std::filesystem::path& p_canvasFilePath);
		~Canvas();
	};
}