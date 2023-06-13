#include "application/modules/spk_API_module.hpp"

namespace spk
{
#ifdef _WIN32
	LRESULT CALLBACK APIModule::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{

	}
#endif
	
	APIModule::APIModule()
	{

	}

	APIModule::~APIModule()
	{

	}

	void APIModule::update()
	{
		MSG msg = {};
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			int translateResult = TranslateMessage(&msg);
			int dispatchResult = static_cast<int>(DispatchMessage(&msg));
		}
	}

	void APIModule::render()
	{
		
	}
}