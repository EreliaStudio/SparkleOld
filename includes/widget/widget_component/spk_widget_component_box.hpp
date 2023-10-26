#pragma once

#include "design_pattern/spk_value.hpp"
#include "graphics/spk_color.hpp"
#include "graphics/spk_area.hpp"
#include "graphics/spk_pipeline.hpp"

namespace spk::WidgetComponent
{
	class Box
	{
	private:
		static inline std::shared_ptr<spk::Pipeline> _renderingPipeline = nullptr;
		static inline const size_t BackgroundIndex = 0;
		static inline const size_t BorderIndex = 1;
		std::shared_ptr<spk::Pipeline::Object> _renderingObjects[2];

		static inline spk::Value<Area>::Default _defaultArea = spk::Area(0, 0);
		static inline spk::Value<float>::Default _defaultDepth = 0.0f;
		static inline spk::Value<spk::Color>::Default _defaultBackgroundColor = spk::Color(120, 120, 120);
		static inline spk::Value<spk::Color>::Default _defaultBorderColor = spk::Color(150, 150, 150);
		static inline spk::Value<size_t>::Default _defaultBorderSize = 5;

		template<typename TType>
		struct Attribute
		{
			using AttributeType = spk::Value<TType>;

			std::recursive_mutex mutex;
			bool needUpdate;
			AttributeType value;
			AttributeType::Contract contract;

			Attribute(const typename AttributeType::Default& p_defaultValue) :
				needUpdate(true),
				value(p_defaultValue),
				contract(value.subscribe([&](){
						std::lock_guard<std::recursive_mutex> lockGuard(mutex);
						needUpdate = true;
					}))
			{

			}

			Attribute& operator = (const TType& p_value)
			{
				value = p_value;
				return (*this);
			}
		};

		Attribute<Area> _area;
		Attribute<float> _depth;
		Attribute<Color> _backgroundColor;
		Attribute<Color> _borderColor;
		Attribute<size_t> _borderSize;

		void _composeBackgroundObjectGeometry();
		void _composeBorderObjectGeometry();

		void _updateArea();
		void _updateDepth();
		void _updateBackgroundColor();
		void _updateBorderColor();
		void _updateBorderSize();

	public:
		Box();

		void setBackgroundColor(const spk::Color& p_backgroundColor);
		void setBorderColor(const spk::Color& p_borderColor);
		void setColors(const spk::Color& p_backgroundColor, const spk::Color& p_borderColor);

		void setGeometry(const spk::Area& p_area);
		void setGeometry(const size_t& p_borderSize);
		void setGeometry(const spk::Area& p_area, const size_t& p_borderSize);

		void setDepth(const float& p_depth);

		void render();
	};
}