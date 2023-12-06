#pragma once

#include "engine/component/spk_component.hpp"
#include "math/spk_vector3.hpp"
#include "graphics/spk_color.hpp"
#include "graphics/spk_pipeline.hpp"

namespace spk
{
    /**
     * @brief Class representing a directional light in the scene.
     * 
     * Directional lights simulate light sources that are very far away and emit light in a single direction. 
     * This class handles the properties of such a light, like direction, intensity, and color, and manages
     * its representation in the GPU buffer for rendering.
     */
    class DirectionalLight : public Component
    {
    private:
        /**
         * @brief Structure representing the directional light data to be sent to the GPU.
         */
        struct GPUBuffer
        {
            spk::Vector3 direction; ///< Direction of the light.
            float intensity;        ///< Intensity of the light.
            spk::Color color;       ///< Color of the light.

            GPUBuffer();
            GPUBuffer(const spk::Vector3& p_direction, const float& p_intensity, const spk::Color& p_color);
        };

    private:
        static inline std::shared_ptr<spk::Pipeline::UniformBlock> _lightUniformBlock = nullptr; ///< Shared uniform block for light data.

        GPUBuffer _gpuBuffer; ///< GPU buffer storing light properties.

        bool _needUpdate; ///< Flag to check if the light data needs to be updated.

        /**
         * @brief Update method called every frame.
         * 
         * @return true if the update was successful.
         * @return false if the update failed.
         */
        virtual bool _onUpdate() override;

        /**
         * @brief Render method called when the light needs to be drawn.
         */
        virtual void _onRender() override;

    public:
        DirectionalLight();

        /**
         * @brief Set the Direction of the light.
         * 
         * @param p_direction New direction of the light.
         */
        void setDirection(const spk::Vector3& p_direction);

        /**
         * @brief Get the Direction of the light.
         * 
         * @return const spk::Vector3& Current direction of the light.
         */
        const spk::Vector3& direction() const;

        /**
         * @brief Set the Intensity of the light.
         * 
         * @param p_intensity New intensity of the light.
         */
        void setIntensity(const float& p_intensity);

        /**
         * @brief Get the Intensity of the light.
         * 
         * @return const float& Current intensity of the light.
         */
        const float& intensity() const;

        /**
         * @brief Set the Color of the light.
         * 
         * @param p_color New color of the light.
         */
        void setColor(const spk::Color& p_color);

        /**
         * @brief Get the Color of the light.
         * 
         * @return const spk::Color& Current color of the light.
         */
        const spk::Color& color() const;
    };
}
