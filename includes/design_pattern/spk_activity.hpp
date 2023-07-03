#pragma once

#include <functional>

namespace spk
{
    /**
     * @brief Represents an activity.
     * This class defines an activity that can be managed by an ActivityManager.
     * It provides methods for updating, moving to the next step, starting, and checking the status of the activity.
     */
    class Activity
    {
        friend class ActivityManager;

    public:
        /**
         * @brief Enumerates the possible status of the activity.
         */
        enum class Status
        {
            Stopped,   /**< The activity is stopped. */
            OnEnter,   /**< The activity is in the onEnter phase. */
            Running,   /**< The activity is running. */
            OnExit,    /**< The activity is in the onExit phase. */
            Complete   /**< The activity is complete. */
        };

        Status _internalWorkingState; /**< The internal working state of the activity. */

        /**
         * @brief Updates the activity.
         * This function updates the activity, typically called once per frame.
         * It is responsible for executing the activity's current step and managing the state transitions.
         */
        void _update();

        /**
         * @brief Moves to the next step of the activity.
         * This function moves the activity to the next step without executing it.
         * It is used for advancing the activity's progress without performing any actions.
         */
        void _moveNextStep();

        /**
         * @brief Moves to the next step of the activity and executes it.
         * This function moves the activity to the next step and executes it.
         * It is used for advancing the activity's progress and performing the associated actions.
         */
        void _moveNextStepAndExecute();

        /**
         * @brief Starts the activity.
         * This function starts the activity, setting its initial state and entering the onEnter phase.
         * It is called when the activity is initially activated.
         */
        void _start();

    protected:
        /**
         * @brief Executes the onEnter phase of the activity.
         * This pure virtual function is called when the activity enters the onEnter phase.
         * Subclasses must override this function to provide custom implementation.
         */
        virtual void _onEnter() = 0;

        /**
         * @brief Executes the main execution phase of the activity.
         * This pure virtual function is called when the activity is in the running state and needs to execute its main logic.
         * Subclasses must override this function to provide custom implementation.
         */
        virtual void _execute() = 0;

        /**
         * @brief Executes the onExit phase of the activity.
         * This pure virtual function is called when the activity enters the onExit phase.
         * Subclasses must override this function to provide custom implementation.
         */
        virtual void _onExit() = 0;

    public:
        /**
         * @brief Default constructor for Activity.
         * This constructor initializes the internal working state of the activity.
         */
        Activity();

        /**
         * @brief Returns the current status of the activity.
         * @return A const reference to the current status of the activity.
         */
        const Status& state() const;

        /**
         * @brief Checks if the activity is completed.
         * @return True if the activity is complete, false otherwise.
         */
        bool isCompleted() const;
    };
}
