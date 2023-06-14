#pragma once

#include "application/modules/spk_abstract_module.hpp"
#include "design_pattern/spk_singleton.hpp"
#include "threading/spk_thread_safe_queue.hpp"

namespace spk
{
	class APIModule : public spk::AbstractModule
	{
	private:
		using MessagePool = spk::Pool<spk::DataBuffer>;
		using MessagePoolInstance = spk::Singleton<MessagePool>;

		spk::ThreadSafeQueue<SystemMessage> _systemQueue;
		spk::ThreadSafeQueue<SystemMessage> _windowQueue;
		spk::ThreadSafeQueue<SystemMessage> _mouseQueue;
		spk::ThreadSafeQueue<SystemMessage> _keyboardQueue;

		LRESULT _handleMessage(const HWND& p_hwnd, const UINT& p_uMsg, const WPARAM& p_wParam, const LPARAM& p_lParam);

	public:
		APIModule();
		~APIModule();

		void update();

		spk::ThreadSafeQueue<SystemMessage> &systemQueue() { return _systemQueue; }

		spk::ThreadSafeQueue<SystemMessage> &windowQueue() { return _windowQueue; }

		spk::ThreadSafeQueue<SystemMessage> &mouseQueue() { return _mouseQueue; }

		spk::ThreadSafeQueue<SystemMessage> &keyboardQueue() { return _keyboardQueue; }

		static LRESULT CALLBACK WindowProc(HWND p_hwnd, UINT p_uMsg, WPARAM p_wParam, LPARAM p_lParam);
	};
}