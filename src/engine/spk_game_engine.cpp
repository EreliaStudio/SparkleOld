#include "engine/spk_game_engine.hpp"

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