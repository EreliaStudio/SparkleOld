#pragma once

#include "application/modules/spk_abstract_module.hpp"

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