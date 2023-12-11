#pragma once

#include <memory>
#include "engine/component/spk_component.hpp"
#include "math/spk_vector2.hpp"
#include "math/spk_matrix4x4.hpp"
#include "engine/component/spk_transform_component.hpp"
#include "graphics/spk_pipeline.hpp"

namespace spk
{
	/**
     * @brief Represents a camera in the Sparkle engine.
     * 
     * The camera can be either perspective or orthographic, providing
     * functionalities to manipulate its projection and view transformations.
     */
	class Camera : public Component
	{
	public:
		/**
         * @brief Enum for camera type.
         */
		enum class Type {
			Perspective,
			Orthographic
		};

	private:
		static inline Camera* _mainCamera = nullptr; ///< Static pointer to the main camera.
        Type _type; ///< Type of the camera (Perspective or Orthographic).
        spk::Matrix4x4 _projectionMatrix; ///< Matrix representing the camera's projection.
        spk::Matrix4x4 _MVP; ///< Model-View-Projection matrix.
        bool _positionEdited = false; ///< Flag to check if the position has been edited.
        bool _MVPEdited = false; ///< Flag to check if the MVP has been edited.
        std::shared_ptr<Transform::TranslationType::Contract> _translationContract; ///< Contract for translation transformations.
        std::shared_ptr<Transform::RotationType::Contract> _rotationContract; ///< Contract for rotation transformations.


		// --- Perspective camera attributes
		float _fov; ///< Field of view for perspective camera.
        float _aspectRatio; ///< Aspect ratio for perspective camera.
        float _nearPlane; ///< Near clipping plane for perspective camera.
        float _farPlane; ///< Far clipping plane for perspective camera.

		// --- Orthographic camera attributes
		spk::Vector2 _size; ///< Size of the orthographic camera view.

		/**
         * @brief Method to update the projection matrix.
         */
		void _updateProjectionMatrix();

		/**
         * @brief Method to update the Model-View-Projection matrix.
         */
		void _updateMVP();

	protected:
        virtual bool _onUpdate() override;
        virtual void _onRender() override;

	public:
		/**
         * @brief Construct a new Camera object
         * 
         * @param p_type Type of the camera (Perspective/Orthographic).
         */
        Camera(Type p_type = Type::Perspective);

        /**
         * @brief Destroy the Camera object.
         */
        ~Camera();

        /**
         * @brief Get the main camera.
         * 
         * @return std::shared_ptr<Camera> A shared pointer to the main camera.
         */
        static std::shared_ptr<Camera> mainCamera();

		/**
		 * @brief Sets this camera as the main camera.
		 *
		 * This method designates the current camera as the main camera used by the engine.
		 */
		void setAsMainCamera();

		/**
		 * @brief Sets the type of the camera.
		 *
		 * @param p_type The type of the camera (Perspective or Orthographic).
		 */
		void setType(Type p_type);

		/**
		 * @brief Sets the parameters for a perspective camera.
		 *
		 * @param p_fov Field of view in degrees.
		 * @param p_aspectRatio Aspect ratio of the camera.
		 * @param p_nearPlane Near clipping plane of the camera.
		 * @param p_farPlane Far clipping plane of the camera.
		 */
		void setPerspectiveParameters(float p_fov, float p_aspectRatio, float p_nearPlane, float p_farPlane);

		/**
		 * @brief Sets the parameters for an orthographic camera.
		 *
		 * @param p_size Size of the orthographic view.
		 * @param p_nearPlane Near clipping plane of the camera.
		 * @param p_farPlane Far clipping plane of the camera.
		 */
		void setOrthographicParameters(const spk::Vector2& p_size, float p_nearPlane, float p_farPlane);

		/**
		 * @brief Indicates whether the MVP matrix has been edited.
		 *
		 * @return true If the MVP matrix has been edited.
		 * @return false Otherwise.
		 */
		bool MVPEdited() const;

		/**
		 * @brief Pushes the MVP matrix to a uniform block of the pipeline.
		 *
		 * @param p_uniformField The field of the uniform block where the MVP matrix will be sent.
		 */
		void pushMVP(AbstractPipeline::UniformBlock::Field & p_uniformField);

		/**
		 * @brief Retrieves the Model-View-Projection (MVP) matrix of the camera.
		 *
		 * @return const spk::Matrix4x4& A constant reference to the MVP matrix.
		 */
		const spk::Matrix4x4& MVP() const;
	};
}