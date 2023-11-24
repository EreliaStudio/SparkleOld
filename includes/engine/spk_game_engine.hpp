#pragma once

#include <string>
#include <vector>
#include "engine/spk_game_object.hpp"
#include "graphics/spk_pipeline.hpp"

namespace spk
{
	class GameEngine
	{
	private:
		static inline std::shared_ptr<spk::Pipeline::UniformBlock> _cameraUniformBlock = nullptr;
		
		std::vector<std::shared_ptr<GameObject>> _ownedGameObjects;

	public:
		GameEngine();

		bool update();
		void render();
		
		std::shared_ptr<GameObject> addGameObject(std::shared_ptr<GameObject> p_objectToAdd);
		void removeGameObject(std::shared_ptr<GameObject> p_objectToRemove);
		void removeGameObject(const std::wstring& p_objectName);
	};
}