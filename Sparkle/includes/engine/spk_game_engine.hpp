#pragma once

#include <string>
#include <vector>
#include "engine/spk_game_object.hpp"
#include "graphics/spk_pipeline.hpp"

namespace spk
{
    /**
     * @brief Represents the core game engine in the Sparkle framework.
     *
     * The GameEngine class manages the game loop, rendering processes, and game object lifecycle.
     * It serves as the central point for creating, updating, and rendering game objects.
     */
    class GameEngine
    {
    private:
        static inline std::shared_ptr<spk::Pipeline::UniformBlock> _cameraUniformBlock = nullptr; ///< Uniform block for camera data.
        static inline std::shared_ptr<spk::Pipeline::UniformBlock> _lightUniformBlock = nullptr; ///< Uniform block for camera data.

        std::vector<std::shared_ptr<GameObject>> _ownedGameObjects; ///< List of game objects managed by the engine.

    public:
        /**
         * @brief Constructs a new GameEngine object.
         *
         * Initializes the game engine, setting up necessary resources and states.
         */
        GameEngine();

        /**
         * @brief Updates all game objects managed by the engine.
         *
         * This method should be called in each frame of the game loop. It updates all game objects and
         * their components.
         * 
         * @return bool True if the update process was successful, false otherwise.
         */
        bool update();

        /**
         * @brief Renders all game objects managed by the engine.
         *
         * This method handles the rendering process of the game objects. It should be called
         * in each frame of the game loop after the update method.
         */
        void render();

        /**
         * @brief Adds a game object to the engine.
         *
         * This method takes ownership of the game object, ensuring it is updated and rendered
         * as part of the game loop.
         * 
         * @param p_objectToAdd Shared pointer to the game object to be added.
         * @return std::shared_ptr<GameObject> Shared pointer to the added game object.
         */
        std::shared_ptr<GameObject> addGameObject(std::shared_ptr<GameObject> p_objectToAdd);

        /**
         * @brief Removes a game object from the engine.
         *
         * @param p_objectToRemove Shared pointer to the game object to be removed.
         */
        void removeGameObject(std::shared_ptr<GameObject> p_objectToRemove);

        /**
         * @brief Removes a game object from the engine by name.
         *
         * @param p_objectName Name of the game object to be removed.
         */
        void removeGameObject(const std::wstring& p_objectName);
    };
}
