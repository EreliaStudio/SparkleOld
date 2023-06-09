#pragma once

#include "application/linux/input/spk_time_metrics.hpp"
#include "application/linux/modules/spk_abstract_module.hpp"

namespace spk
{
	class TimeModule : public spk::AbstractModule
	{
	public:
		TimeModule();

		~TimeModule();

		void update();
	};
}