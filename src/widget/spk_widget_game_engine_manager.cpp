#include "widget/spk_widget_game_engine_manager.hpp"
#include "engine/component/spk_camera_component.hpp"

namespace spk
{
	void GameEngineManager::_onGeometryChange()
	{
		_resetPerspectiveParameters = true;
	}

	void GameEngineManager::_onRender()
	{
		if (_gameEngine != nullptr)
		{
			if (_resetPerspectiveParameters == true && spk::Camera::mainCamera() != nullptr)
			{
				spk::Camera::mainCamera()->setPerspectiveParameters(90.0f, static_cast<float>(size().x) / static_cast<float>(size().y), 0.1f, 10000.0f);
				_resetPerspectiveParameters = false;
			}
			_gameEngine->render();
		}
	}

	bool GameEngineManager::_onUpdate()
	{
		if (_gameEngine != nullptr)
		{
			_gameEngine->update();
		}

		return (false);
	}

	GameEngineManager::GameEngineManager(const std::wstring &p_name) :
		spk::Widget::Interface(p_name)
	{
		_gameEngine = nullptr;
	}

	GameEngineManager::~GameEngineManager()
	{
	}

	std::shared_ptr<spk::GameEngine> GameEngineManager::gameEngine() const
	{
		return (_gameEngine);
	}

	void GameEngineManager::setGameEngine(std::shared_ptr<spk::GameEngine> p_gameEngine)
	{
		_gameEngine = p_gameEngine;
	}
}