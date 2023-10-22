#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	void AbstractPipeline::UniformBlock::_launchPushData()
	{
		_pushData();
	}

	AbstractPipeline::UniformBlock::UniformBlock(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout) :
		Uniform(p_uniformBlockLayout)
	{
		for (const auto& field : p_uniformBlockLayout.fields())
		{
			_fields.emplace(
				spk::to_wstring(field.name),
				Field(
					p_uniformBlockLayout.name() + L"::" + spk::to_wstring(field.name),
					this,
					_data.data(),
					field.offset,
					field.data.format * field.data.size
				)
			);
		}
	}

	AbstractPipeline::UniformBlock::Field &AbstractPipeline::UniformBlock::field(const std::wstring &p_fieldName)
	{
		if (_fields.contains(p_fieldName) == false)
			spk::throwException(L"Field [" + p_fieldName + L"] doesn't exist in FieldMap");
		return (_fields[p_fieldName]);
	}

	AbstractPipeline::UniformBlock::Field &AbstractPipeline::UniformBlock::operator[](const std::wstring &p_fieldName)
	{
		return (this->field(p_fieldName));
	}
}