#pragma once

#include "application/modules/spk_abstract_module.hpp"
#include "graphics/spk_window.hpp"
#include "iostream/spk_iostream.hpp"
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

		HWND _windowFrame;

		void _pullWinMessage();
		LRESULT _handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		APIModule();
		~APIModule();

		void update();
		void render();

		spk::ThreadSafeQueue<SystemMessage> &systemQueue() { return _systemQueue; }

		spk::ThreadSafeQueue<SystemMessage> &windowQueue() { return _windowQueue; }

		spk::ThreadSafeQueue<SystemMessage> &mouseQueue() { return _mouseQueue; }

		spk::ThreadSafeQueue<SystemMessage> &keyboardQueue() { return _keyboardQueue; }

		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
}