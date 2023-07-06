#pragma once

#include "application/modules/spk_abstract_module.hpp"

namespace spk
{
    /**
     * @class TimeModule
     * @brief Class for managing time-related functionality.
     *
     * This class provides methods to handle and manage time-related functionality within the application.
	 * This class handle the TimeMetrics singleton.
     */
	class TimeModule : public spk::AbstractModule
	{
	public:
        /**
         * @brief Constructs the TimeModule object.
		 * Will instanciate the TimeMetrics
         */
		TimeModule();

        /**
         * @brief Destructs the TimeModule object.
		 * Will release the TimeMetrics instance
         */
		~TimeModule();

        /**
         * @brief Updates the time state.
		 * Will update the TimeMetrics
         */
		void update();
	};
}
