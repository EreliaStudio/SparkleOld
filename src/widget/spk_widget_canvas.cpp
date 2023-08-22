#include "widget/spk_widget_canvas.hpp"
#include "miscellaneous/JSON/spk_JSON_file.hpp"
#include "widget/spk_widget_atlas.hpp"

namespace spk::Widget
{
	spk::Vector2Int Canvas::_calcValue(Interface* p_target, const Geometry::Value& p_value)
	{
		spk::Vector2Int result;

		switch (p_value.type)
		{
			case Geometry::Value::Type::FixedValue:
				result = p_value.value;
				break;
			case Geometry::Value::Type::Ratio:
				result = p_value.value * size();
				break;
			case Geometry::Value::Type::Cell:
				result = p_value.value * size() / _gridSize;
				break;
			case Geometry::Value::Type::Formula:
				result = _calcFormula(p_target, p_value.formulaValue);
				break;
			default:

				break;
		}
		return (result);
	}

	void Canvas::_onGeometryChange()
	{
		for (auto item : _widgetGeometries)
		{
			Interface* target = item.first;
			Geometry& value = item.second;
			spk::Vector2Int targetAnchor = _calcValue(target, value.anchor);
			spk::Vector2Int targetSize = _calcValue(target, value.size);

			spk::cout << L"Widget [" << target->name() << L"] -> anchor [" << targetAnchor << L"] / size [" << targetSize << L"]" << std::endl;
			target->setGeometry(targetAnchor, targetSize);
		}
	}

	void Canvas::_onRender()
	{

	}

	bool Canvas::_onUpdate()
	{
		return (false);
	}


	void Canvas::_loadConfiguration(const spk::JSON::Object& p_object)
	{
		if (p_object.contains(L"Name") == false)
			spk::throwException(L"Canvas Configuration json object must contain a \"Name\" attribute.");
		rename(p_object[L"Name"].as<std::wstring>());

		if (p_object.contains(L"GridSize") == false)
			spk::throwException(L"Canvas Configuration json object must contain a \"GridSize\" attribute.");
		_gridSize = spk::Vector2Int(p_object[L"GridSize"]);
	}

	Canvas::Geometry::Value::Value()
	{

	}

	Canvas::Geometry::Value::Value(const spk::JSON::Object& p_object)
	{
		if (p_object.contains(L"Type") == false)
			throw std::runtime_error("No member Type found");

		std::wstring typeString = p_object[L"Type"].as<std::wstring>();
		if (typeString == L"FixedValue")
		{
			type = Type::FixedValue;
			value = spk::Vector2Int(p_object);
		}
		else if (typeString == L"Ratio")
		{
			type = Type::Ratio;
			value = spk::Vector2(p_object);
		}
		else if (typeString == L"Cell")
		{
			type = Type::Cell;
			value = spk::Vector2(p_object);
		}
		else if (typeString == L"Formula")
		{
			type = Type::Formula;
			formulaValue.x = p_object[L"X"].as<std::wstring>();	
			formulaValue.y = p_object[L"Y"].as<std::wstring>();	
		}
		else
		{
			throw std::runtime_error("Invalid Type value. Accepted value : \"FixedValue\", \"Ratio\", \"Cell\" and \"Formula\"");
		}
	}

	Canvas::Geometry::Geometry()
	{

	}

	Canvas::Geometry::Geometry(const spk::JSON::Object& p_object)
	{
		if (p_object.contains(L"Anchor") == true)
		{
			anchor = Canvas::Geometry::Value(p_object.operator[](L"Anchor"));
		}
		if (p_object.contains(L"Size") == true)
		{
			size = Canvas::Geometry::Value(p_object.operator[](L"Size"));
		}
	}

	void Canvas::_loadContent(const spk::JSON::Object& p_object)
	{
		for (auto item : p_object.members())
		{
			std::wstring classType = item.second->operator[](L"Type").as<std::wstring>();
			spk::Widget::Atlas::Instanciator lambda = spk::Widget::Atlas::classInstanciatorLambda[classType];

			spk::Widget::Interface* newWidget = lambda(item.first, *(item.second));

			_widgetGeometries[newWidget] = Geometry(*(item.second));
		}
	}

	Canvas::Canvas(const std::filesystem::path& p_canvasFilePath) : 
		spk::Widget::Interface(L"FixedValueName")
	{
		spk::JSON::File file = spk::JSON::File(p_canvasFilePath);

		if (file.contains(L"Configuration") == false)
			spk::throwException(L"Canvas json must contain a \"Configuration\" attribute.");
		_loadConfiguration(file[L"Configuration"]);

		if (file.contains(L"Content") == false)
			spk::throwException(L"Canvas json must contain a \"Content\" attribute.");
		_loadContent(file[L"Content"]);
	}

	Canvas::~Canvas()
	{

	}
}