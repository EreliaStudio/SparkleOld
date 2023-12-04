#pragma once

#include "engine/component/spk_component.hpp"
#include "math/spk_vector3.hpp"
#include "graphics/spk_color.hpp"
#include "graphics/spk_pipeline.hpp"

namespace spk
{
	class DirectionalLight : public Component
	{
	private:
		struct GPUBuffer
		{
			spk::Vector3 direction;
			float intensity;
			spk::Color color;

			GPUBuffer();
			GPUBuffer(const spk::Vector3& p_direction, const float& p_intensity, const spk::Color& p_color);
		};

	private:
        static inline std::shared_ptr<spk::Pipeline::UniformBlock> _lightUniformBlock = nullptr;

		GPUBuffer _gpuBuffer;

		bool _needUpdate;

        virtual bool _onUpdate() override;
        virtual void _onRender() override;

	public:
		DirectionalLight();
		void setDirection(const spk::Vector3& p_direction);
		const spk::Vector3& direction() const;
		
		void setIntensity(const float& p_intensity);
		const float& intensity() const;
		
		void setColor(const spk::Color& p_color);
		const spk::Color& color() const;
	};
}