#pragma once

#include "graphics/pipeline/spk_shader_module.hpp"

namespace spk
{
    /**
     *  @brief Extern declaration for the Vertex Shader Module of the Box Component.
     *
     *  This shader module is intended to handle the vertex shading logic
     *  specific to rendering a Box Component in the graphics pipeline.
     */
    extern spk::ShaderModule boxComponentVertexShaderModule;

    /**
     *  @brief Extern declaration for the Fragment Shader Module of the Box Component.
     *
     *  This shader module is intended to handle the fragment shading logic
     *  specific to rendering a Box Component in the graphics pipeline.
     */
    extern spk::ShaderModule boxComponentFragmentShaderModule;

    /**
     *  @brief Extern declaration for the Vertex Shader Module of the TextLabel Component.
     *
     *  This shader module is intended to handle the vertex shading logic
     *  specific to rendering a TextLabel Component in the graphics pipeline.
     */
    extern spk::ShaderModule textLabelComponentVertexShaderModule;

    /**
     *  @brief Extern declaration for the Fragment Shader Module of the TextLabel Component.
     *
     *  This shader module is intended to handle the fragment shading logic
     *  specific to rendering a TextLabel Component in the graphics pipeline.
     */
    extern spk::ShaderModule textLabelComponentFragmentShaderModule;

    /**
     *  @brief Extern declaration for the Vertex Shader Module of the Mesh Renderer Component.
     *
     *  This shader module is intended to handle the vertex shading logic
     *  specific to rendering a Mesh Renderer Component in the graphics pipeline.
     *  It defines how vertices are processed and transformed during the vertex shading stage.
     */
    extern spk::ShaderModule meshRendererComponentVertexShaderModule;

    /**
     *  @brief Extern declaration for the Fragment Shader Module of the Mesh Renderer Component.
     *
     *  This shader module is intended to handle the fragment shading logic
     *  specific to rendering a Mesh Renderer Component in the graphics pipeline.
     *  It defines the processing of fragments (potential pixels) and their properties,
     *  determining the final color and other attributes of pixels on the screen.
     */
    extern spk::ShaderModule meshRendererComponentFragmentShaderModule;
}