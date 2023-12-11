#include "engine/spk_game_object.hpp"

namespace spk
{
	GameObject::GameObject(const std::wstring& p_name) : 
		_name(p_name)
	{
		_transform = addComponent<Transform>();
		activate();
	}

	GameObject::GameObject(const std::wstring& p_name, const spk::Vector3& p_position) :
		GameObject(p_name)
	{
		transform()->place(p_position);
	}

	GameObject::~GameObject()
	{
		
	}

	std::shared_ptr<GameObject> GameObject::addChildrenGameObject(const std::wstring& p_childrenName)
	{
		std::shared_ptr<GameObject> result = std::make_shared<GameObject>(p_childrenName);

		result->setParent(this);

		return (result);
	}

	const std::wstring& GameObject::name() const
	{
		return (_name);
	}

	std::shared_ptr<Transform> GameObject::transform()
	{
		return (_transform);
	}

	std::shared_ptr<const Transform> GameObject::transform() const
	{
		return (_transform);
	}

	void GameObject::render()
	{
		if (isActive() == false)
			return ;

		for (size_t i = 0; i < _components.size(); i++)
		{
			_components[i]->render();
		}

		for (size_t i = 0; i < childrens().size(); i++)
		{
			childrens()[i]->render();
		}
	}
	bool GameObject::update()
	{
		if (isActive() == false)
			return (false);

		for (size_t i = 0; i < childrens().size(); i++)
		{
			if (childrens()[i]->update() == true)
				return (true);
		}

		for (size_t i = 0; i < _components.size(); i++)
		{
			if (_components[i]->update() == true)
				return (true);
		}
		return (false);
	}
}