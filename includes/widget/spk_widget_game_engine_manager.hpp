#pragma once

#include "widget/spk_widget_interface.hpp"
#include "engine/spk_game_engine.hpp"

namespace spk
{
	class GameEngineManager : public spk::Widget::Interface
	{
	private:
		bool _resetPerspectiveParameters = true;
		std::shared_ptr<spk::GameEngine> _gameEngine = nullptr;

		void _onGeometryChange();
		void _onRender();
		bool _onUpdate();

	public:
		GameEngineManager(const std::wstring& p_name);
		~GameEngineManager();

		std::shared_ptr<spk::GameEngine> gameEngine() const;
		void setGameEngine(std::shared_ptr<spk::GameEngine> p_gameEngine);
	};
}