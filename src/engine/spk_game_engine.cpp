#include "engine/spk_game_engine.hpp"
#include "engine/component/spk_camera_component.hpp"
#include "graphics/spk_pipeline.hpp"

namespace spk
{
	GameEngine::GameEngine()
	{

	}

	bool GameEngine::update()
	{
		for (size_t i = 0; i < _ownedGameObjects.size(); i++)
		{
			if (_ownedGameObjects[i]->isActive() == true && _ownedGameObjects[i]->update() == true)
				return (true);
		}
		return (false);
	}

	void GameEngine::render()
	{
		if (spk::Camera::mainCamera() == nullptr)
			spk::throwException(L"Can't render a game engine without MainCamera defined");
		
		spk::Pipeline::UniformBlock& cameraUniformBlock = dynamic_cast<spk::Pipeline::UniformBlock&>(spk::Pipeline::uniform(spk::Pipeline::Uniform::Key(0, 0)));
		spk::Matrix4x4 MVP = spk::Camera::mainCamera()->MVP();
		cameraUniformBlock.field(L"MVP") = MVP;

		for (size_t i = 0; i < _ownedGameObjects.size(); i++)
		{
			if (_ownedGameObjects[i]->isActive() == true)
				_ownedGameObjects[i]->render();
		}
	}

	std::shared_ptr<GameObject> GameEngine::subscribe(std::shared_ptr<GameObject> p_objectToAdd)
	{
		_ownedGameObjects.push_back(p_objectToAdd);

		return (p_objectToAdd);
	}

	void GameEngine::unsubscribe(std::shared_ptr<GameObject> p_objectToRemove)
	{
		for (auto it = _ownedGameObjects.begin(); it != _ownedGameObjects.end(); ++it)
		{
			if (*it == p_objectToRemove)
			{
				_ownedGameObjects.erase(it);
				return;
			}
		}
	}

	void GameEngine::unsubscribe(const std::wstring& p_objectName)
	{
		for (auto it = _ownedGameObjects.begin(); it != _ownedGameObjects.end(); ++it)
		{
			if ((*it)->name() == p_objectName)
			{
				_ownedGameObjects.erase(it);
				return;
			}
		}
	}
}