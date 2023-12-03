#pragma once

#include "engine/component/spk_component.hpp"
#include "math/spk_vector3.hpp"
#include "graphics/spk_pipeline.hpp"

namespace spk
{
	class DirectionalLight : public Component
	{
	private:
        static inline std::shared_ptr<spk::Pipeline::UniformBlock> _lightUniformBlock = nullptr;

		spk::Vector3 _direction;
		bool _needUpdate;

        virtual bool _onUpdate() override;
        virtual void _onRender() override;

	public:
		DirectionalLight();
		void setDirection(const spk::Vector3& p_direction);
		const spk::Vector3& direction() const;
	};
}