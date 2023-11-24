#pragma once

#include <vector>
#include <string>
#include "design_pattern/spk_activable_object.hpp"
#include "design_pattern/spk_inherence_object.hpp"
#include "engine/spk_components.hpp"

namespace spk
{
	class GameObject : public spk::ActivableObject, public spk::InherenceObject<GameObject>
	{
		friend class Component;
	private:
		static inline GameObject* _insertingGameObject = nullptr;

		std::wstring _name;
		std::vector<std::shared_ptr<Component>> _components;
		std::shared_ptr<Transform> _transform;

	public:
		GameObject(const std::wstring& p_name);
		GameObject(const std::wstring& p_name, const spk::Vector3& p_position);
		~GameObject();

		template <typename TComponentType, typename... Args>
		std::shared_ptr<TComponentType> addComponent(Args&&... p_args)
		{
			_insertingGameObject = this;
			std::shared_ptr<TComponentType> result = std::make_shared<TComponentType>(std::forward<Args>(p_args)...);
			_insertingGameObject = nullptr;
			_components.push_back(result);
			return (result);
		}

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

		std::shared_ptr<GameObject> addChildrenGameObject(const std::wstring& p_childrenName);

		const std::wstring& name() const;
		std::shared_ptr<Transform> transform();
		std::shared_ptr<const Transform> transform() const;

		void render();
		bool update();
	};
}