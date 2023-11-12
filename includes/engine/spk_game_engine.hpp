#pragma once

#include <string>
#include <vector>
#include "engine/spk_game_object.hpp"

namespace spk
{
	class GameEngine
	{
	private:
		std::vector<std::shared_ptr<GameObject>> _ownedGameObjects;

	public:
		GameEngine();

		bool update();
		void render();
		
		std::shared_ptr<GameObject> subscribe(std::shared_ptr<GameObject> p_objectToAdd);
		void unsubscribe(std::shared_ptr<GameObject> p_objectToRemove);
		void unsubscribe(const std::wstring& p_objectName);
	};
}