#pragma once

#include "widget/spk_widget_interface.hpp"
#include "input/spk_input.hpp"

namespace spk::Widget
{
    /**
     * @class InputGroupManager
     * @brief Manager class for grouping inputs.
     *
     * This class is a no-graphics widget that manages grouping of inputs. 
     * Inputs can be added or removed to/from the group.
     */
    class InputGroupManager : public spk::Widget::NoGraphics
    {
    private:
        /**
         * @brief The onUpdate function that is called when the manager updates.
         *
         * This function is called when the manager updates, and is responsible for managing the inputs
         * that have been added to the group.
         *
         * @return Returns true if the update was successful, false otherwise.
         */
        bool _onUpdate();
    
    public:
        /**
         * @brief Constructs an InputGroupManager with a name.
         *
         * @param p_name The name of the manager.
         */
        InputGroupManager(const std::wstring& p_name);

        /**
         * @brief Destroys the InputGroupManager.
         *
         * This destructor ensures that all inputs in the group are properly cleaned up when the manager is destroyed.
         */
        ~InputGroupManager();
    };

    /**
     * @brief Adds an input to the group.
     *
     * This function adds an input to the group managed by the InputGroupManager.
     *
     * @param p_input The input to be added.
     */
    void addInput(Input* p_input);

    /**
     * @brief Removes an input from the group.
     *
     * This function removes an input from the group managed by the InputGroupManager.
     *
     * @param p_input The input to be removed.
     */
    void removeInput(Input* p_input);
}
