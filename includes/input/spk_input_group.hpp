#pragma once

#include "input/spk_input.hpp"
#include <vector>

namespace spk
{
    /**
     * @class InputGroup
     * @brief Manages a group of input objects.
     *
     * An InputGroup allows the grouping and management of multiple input objects,
     * including adding, removing, and updating inputs collectively.
     */
    class InputGroup
    {
    private:
        /// A list of input objects managed by this InputGroup.
        std::vector<Input*> _inputs;

    public:
        /**
         * @brief Adds an Input object to this group.
         * 
         * The given Input object will be added to the internal list, allowing it to be
         * managed collectively with other inputs in the group.
         *
         * @param p_input The Input object to add to the group.
         */
        void addInput(Input* p_input);

        /**
         * @brief Removes an Input object from this group.
         *
         * The given Input object will be removed from the internal list, and it will no longer
         * be managed with other inputs in the group.
         *
         * @param p_input The Input object to remove from the group.
         */
        void removeInput(Input* p_input);

        /**
         * @brief Updates the state of all Input objects in the group.
         *
         * This method will iterate over all Input objects in the internal list, and call
         * their respective update methods.
         */
        void update();
    };
}
