#pragma once

#include "engine/component/spk_component.hpp"
#include "engine/component/spk_transform_component.hpp"
#include "engine/spk_mesh.hpp"
#include "engine/spk_material.hpp"
#include "graphics/spk_pipeline.hpp"
#include "graphics/spk_texture.hpp"

namespace spk
{
	/**
     * @brief Represents a mesh renderer component in the Sparkle engine.
     *
     * The MeshRenderer class is responsible for rendering a mesh with a given texture.
     * It handles the integration of the mesh with the rendering pipeline and applies
     * transformations to the mesh.
     */
    class MeshRenderer : public Component
    {
    protected:
		/**
		 *  @brief Vertex Shader Module of the Mesh Renderer Component.
		 *
		 *  This shader module is intended to handle the vertex shading logic
		 *  specific to rendering a Mesh Renderer Component in the graphics pipeline.
		 *  It defines how vertices are processed and transformed during the vertex shading stage.
		 */
		static spk::ShaderModule VertexShaderModule;

		/**
		 *  @brief Fragment Shader Module of the Mesh Renderer Component.
		 *
		 *  This shader module is intended to handle the fragment shading logic
		 *  specific to rendering a Mesh Renderer Component in the graphics pipeline.
		 *  It defines the processing of fragments (potential pixels) and their properties,
		 *  determining the final color and other attributes of pixels on the screen.
		 */
		static spk::ShaderModule FragmentShaderModule;

        static inline std::shared_ptr<spk::Pipeline> _renderingPipeline = nullptr; ///< Shared rendering pipeline.
        static inline std::shared_ptr<spk::Pipeline::SamplerUniform> _textureIDUniform = nullptr; ///< Uniform for texture ID.
        std::shared_ptr<spk::Pipeline::Object> _renderingObject = nullptr; ///< Object in the rendering pipeline.

        std::shared_ptr<spk::Texture> _texture; ///< Texture applied to the mesh.
        bool _updateTransform = true; ///< Flag indicating if the transform needs an update.

        std::shared_ptr<Transform::TranslationType::Contract> _translationContract; ///< Translation contract for the mesh.
        std::shared_ptr<Transform::ScaleType::Contract> _scaleContract; ///< Scale contract for the mesh.
        std::shared_ptr<Transform::RotationType::Contract> _rotationContract; ///< Rotation contract for the mesh.

        virtual bool _onUpdate() override;
        virtual void _onRender() override;

        std::shared_ptr<spk::Mesh> _mesh; ///< Mesh to be rendered.
		spk::Material _lastMaterial; ///< Last material, used to check if the material have changed during last tick.
		std::shared_ptr<spk::Material> _material; ///< Material to be rendered.

        void _updateMeshModelData(); ///< Updates the mesh model data.
		void _updateMaterial(); ///< Updates the material data.

    public:
        /**
         * @brief Constructs a new MeshRenderer object.
         */
        MeshRenderer();

        /**
         * @brief Constructs a new MeshRenderer object with specified mesh and texture.
         *
         * @param p_mesh Shared pointer to the mesh to be rendered.
         * @param p_texture Shared pointer to the texture to be applied.
         */
        MeshRenderer(std::shared_ptr<spk::Mesh> p_mesh, std::shared_ptr<spk::Texture> p_texture);

        /**
         * @brief Initializes the shader for mesh rendering.
         *
         * This static method sets up the shader program used for rendering meshes.
         */
        static void initializeMeshRendererShader();
	
		/**
		 * @brief Sets the mesh to be rendered.
		 *
		 * This method assigns a new mesh to the renderer. The provided mesh will be used
		 * for subsequent rendering operations.
		 * 
		 * @param p_mesh Shared pointer to the mesh to be rendered.
		 */
		void setMesh(std::shared_ptr<spk::Mesh> p_mesh);

		/**
		 * @brief Gets the currently assigned mesh.
		 *
		 * This method retrieves the mesh currently set for rendering.
		 * 
		 * @return std::shared_ptr<spk::Mesh> Shared pointer to the currently set mesh.
		 */
		std::shared_ptr<spk::Mesh> mesh();

		/**
		 * @brief Gets the currently assigned mesh (const version).
		 *
		 * This method retrieves the mesh currently set for rendering, providing a const reference.
		 * 
		 * @return std::shared_ptr<const spk::Mesh> Const shared pointer to the currently set mesh.
		 */
		std::shared_ptr<const spk::Mesh> mesh() const;
	
		/**
		 * @brief Sets the material to be rendered.
		 *
		 * This method assigns a new material to the renderer. The provided material will be used
		 * for subsequent rendering operations.
		 * 
		 * @param p_material Shared pointer to the material to be rendered.
		 */
		void setMaterial(std::shared_ptr<spk::Material> p_material);

		/**
		 * @brief Gets the currently assigned material.
		 *
		 * This method retrieves the material currently set for rendering.
		 * 
		 * @return std::shared_ptr<spk::Material> Shared pointer to the currently set material.
		 */
		std::shared_ptr<spk::Material> material();

		/**
		 * @brief Gets the currently assigned material (const version).
		 *
		 * This method retrieves the material currently set for rendering, providing a const reference.
		 * 
		 * @return std::shared_ptr<const spk::Material> Const shared pointer to the currently set material.
		 */
		std::shared_ptr<const spk::Material> material() const;

		/**
		 * @brief Sets the texture to be applied to the mesh.
		 *
		 * This method assigns a new texture to the renderer. The provided texture will be applied
		 * to the mesh during rendering.
		 * 
		 * @param p_texture Shared pointer to the texture to be applied.
		 */
		void setTexture(std::shared_ptr<spk::Texture> p_texture);

		/**
		 * @brief Gets the currently assigned texture.
		 *
		 * This method retrieves the texture currently set to be applied to the mesh.
		 * 
		 * @return std::shared_ptr<spk::Texture> Shared pointer to the currently set texture.
		 */
		std::shared_ptr<spk::Texture> texture();

		/**
		 * @brief Gets the currently assigned texture (const version).
		 *
		 * This method retrieves the texture currently set to be applied to the mesh,
		 * providing a const reference.
		 * 
		 * @return std::shared_ptr<const spk::Texture> Const shared pointer to the currently set texture.
		 */
		std::shared_ptr<const spk::Texture> texture() const;
	};
}