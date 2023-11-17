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
		
		std::shared_ptr<GameObject> subscribe(std::shared_ptr<GameObject> p_objectToAdd);
		void unsubscribe(std::shared_ptr<GameObject> p_objectToRemove);
		void unsubscribe(const std::wstring& p_objectName);
	};
}