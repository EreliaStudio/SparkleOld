#pragma once

#include "system/spk_timer.hpp"

namespace spk
{
	class ProfilerModule
	{
	private:
		spk::Timer _oneSecondTime;
		size_t _renderIPSCounter;
		size_t _updateIPSCounter;

		void _resetCounter();

	public:
		ProfilerModule();
		~ProfilerModule();

		void updateData();
		void increaseRenderIPS();
		void increaseUpdateIPS();
	};
}