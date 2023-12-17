#pragma once

#include "widget/spk_widget.hpp"
#include "engine/spk_game_engine.hpp"

namespace spk
{
	/**
     * @brief Manages and integrates the GameEngine with the user interface.
     *
     * The GameEngineManager class is responsible for handling the game engine's lifecycle,
     * updating its state, and rendering it within the context of a user interface widget.
     */
    class GameEngineManager : public Widget
    {
    private:
        bool _resetPerspectiveParameters = true; ///< Flag to reset perspective parameters.
        std::shared_ptr<spk::GameEngine> _gameEngine = nullptr; ///< Pointer to the managed GameEngine instance.

        void _onGeometryChange(); ///< Handles changes in geometry, such as window resizing.
        void _onRender(); ///< Handles rendering of the game engine.
        bool _onUpdate(); ///< Handles updating the state of the game engine.

    public:
        /**
         * @brief Constructs a new Game Engine Manager object.
         *
         * @param p_name The name of the game engine manager.
         */
        GameEngineManager(const std::wstring& p_name);

        /**
         * @brief Destroys the Game Engine Manager object.
         */
        ~GameEngineManager();

        /**
         * @brief Gets the currently managed game engine.
         *
         * @return std::shared_ptr<spk::GameEngine> Shared pointer to the managed game engine.
         */
        std::shared_ptr<spk::GameEngine> gameEngine() const;

        /**
         * @brief Sets the game engine to be managed.
         *
         * @param p_gameEngine Shared pointer to the game engine to be managed.
         */
        void setGameEngine(std::shared_ptr<spk::GameEngine> p_gameEngine);
    };
}