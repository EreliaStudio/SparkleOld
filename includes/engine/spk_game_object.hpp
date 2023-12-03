#pragma once

#include <vector>
#include <string>
#include "design_pattern/spk_activable_object.hpp"
#include "design_pattern/spk_inherence_object.hpp"
#include "engine/spk_components.hpp"

namespace spk
{
	/**
     * @brief Represents a game object in the Sparkle engine.
     *
     * A game object is a fundamental entity in the game world, capable of containing
     * components and being part of the scene's hierarchy.
     */
    class GameObject : public spk::ActivableObject, public spk::InherenceObject<GameObject>
    {
        friend class Component;

    private:
        static inline GameObject* _insertingGameObject = nullptr; ///< Static pointer used during component insertion.

        std::wstring _name; ///< Name of the game object.
        std::vector<std::shared_ptr<Component>> _components; ///< List of components attached to the game object.
        std::shared_ptr<Transform> _transform; ///< Transform component of the game object.

	public:
		/**
         * @brief Construct a new GameObject.
         * 
         * @param p_name The name of the game object.
         */
        GameObject(const std::wstring& p_name);

        /**
         * @brief Construct a new GameObject with initial position.
         * 
         * @param p_name The name of the game object.
         * @param p_position The initial position of the game object.
         */
        GameObject(const std::wstring& p_name, const spk::Vector3& p_position);

        /**
         * @brief Destroy the GameObject.
         */
        ~GameObject();

        /**
         * @brief Adds a component to the game object.
         * 
         * @tparam TComponentType The type of component to add.
         * @tparam Args Types of arguments to pass to the component's constructor.
         * @param p_args Arguments to pass to the component's constructor.
         * @return std::shared_ptr<TComponentType> A shared pointer to the created component.
         */
		template <typename TComponentType, typename... Args>
		std::shared_ptr<TComponentType> addComponent(Args&&... p_args)
		{
			_insertingGameObject = this;
			std::shared_ptr<TComponentType> result = std::make_shared<TComponentType>(std::forward<Args>(p_args)...);
			_insertingGameObject = nullptr;
			_components.push_back(result);
			return (result);
		}

		/**
         * @brief Retrieves a component of a specific type from the game object.
         * 
         * @tparam TComponentType The type of component to retrieve.
         * @return std::shared_ptr<TComponentType> A shared pointer to the retrieved component, or nullptr if not found.
         */
		template <typename TComponentType>
		std::shared_ptr<TComponentType> getComponent()
		{
			for (auto& component : _components)
			{
				std::shared_ptr<TComponentType> castedComponent = std::dynamic_pointer_cast<TComponentType>(component);
				if (castedComponent != nullptr)
				{
					return castedComponent;
				}
			}
			return nullptr;
		}

		/**
         * @brief Retrieves a component of a specific type from the game object.
         * 
         * @tparam TComponentType The type of component to retrieve.
         * @return std::shared_ptr<TComponentType> A shared pointer to the retrieved component, or nullptr if not found.
         */
		template <typename TComponentType>
		std::shared_ptr<TComponentType> getComponent(const std::wstring& p_componentName)
		{
			for (auto& component : _components)
			{
				std::shared_ptr<TComponentType> castedComponent = std::dynamic_pointer_cast<TComponentType>(component);
				if (castedComponent != nullptr && castedComponent->name() == p_componentName)
				{
					return castedComponent;
				}
			}
			return nullptr;
		}
		
        /**
         * @brief Adds a child game object.
         * 
         * @param p_childrenName The name of the child game object.
         * @return std::shared_ptr<GameObject> A shared pointer to the created child game object.
         */
		std::shared_ptr<GameObject> addChildrenGameObject(const std::wstring& p_childrenName);

		/**
         * @brief Gets the name of the game object.
         * 
         * @return const std::wstring& Reference to the name of the game object.
         */
        const std::wstring& name() const;

        /**
         * @brief Gets the transform component of the game object.
         * 
         * @return std::shared_ptr<Transform> A shared pointer to the transform component.
         */
        std::shared_ptr<Transform> transform();

        /**
         * @brief Gets the transform component of the game object (const version).
         * 
         * @return std::shared_ptr<const Transform> A const shared pointer to the transform component.
         */
        std::shared_ptr<const Transform> transform() const;

        /**
         * @brief Renders the game object.
         */
        void render();

        /**
         * @brief Updates the game object.
         * 
         * @return bool True if the update is successful, false otherwise.
         */
        bool update();
	};
}