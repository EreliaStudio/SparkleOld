#include "application/modules/spk_API_module.hpp"

namespace spk
{
	LRESULT CALLBACK APIModule::WindowProc(HWND p_hwnd, UINT p_uMsg, WPARAM p_wParam, LPARAM p_lParam)
	{
		static APIModule *pThis = NULL;

		if (p_uMsg == WM_NCCREATE)
		{
			CREATESTRUCT *pCreate = (CREATESTRUCT *)p_lParam;
			pThis = (APIModule *)pCreate->lpCreateParams;
			SetWindowLongPtr(p_hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
		}

		if (pThis != NULL)
		{
			return pThis->_handleMessage(p_hwnd, p_uMsg, p_wParam, p_lParam);
		}
		else
		{
			return DefWindowProc(p_hwnd, p_uMsg, p_wParam, p_lParam);
		}
	}

	LRESULT APIModule::_handleMessage(const HWND& p_hwnd, const UINT& p_uMsg, const WPARAM& p_wParam, const LPARAM& p_lParam)
	{
		MessagePool::Object newMessage = MessagePoolInstance::instance()->obtain();

		newMessage->clear();

		*newMessage << p_uMsg;
		switch (p_uMsg)
		{
		case WM_DESTROY:
		{
			_systemQueue.push_back(newMessage);
			break;
		}
		case WM_MOVE:
		case WM_MDIRESTORE:
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
		{
			_windowQueue.push_back(newMessage);
			break;
		}
		case WM_SIZE:
		{
			unsigned int width = LOWORD(p_lParam);
			unsigned int height = HIWORD(p_lParam);

			*newMessage << width;
			*newMessage << height;

			_windowQueue.push_back(newMessage);
			break;
		}

		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		{
			_mouseQueue.push_back(newMessage);
			break;
		}
		case WM_XBUTTONDOWN :
		case WM_XBUTTONUP :
		{
			short value = GET_XBUTTON_WPARAM (p_wParam);

			*newMessage << value;

			_mouseQueue.push_back(newMessage);
			break;
		}
		case WM_MOUSEHWHEEL:
		case WM_MOUSEWHEEL:
		{
			short value = GET_WHEEL_DELTA_WPARAM(p_wParam);

			*newMessage << value;

			_mouseQueue.push_back(newMessage);
			break;
		}
		case WM_MOUSEMOVE:
		{
			int x = LOWORD(p_lParam);
			int y = HIWORD(p_lParam);

			*newMessage << x;
			*newMessage << y;

			_mouseQueue.push_back(newMessage);
			break;
		}

		// ----- Keyboard part
		case WM_CHAR:
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_HOTKEY:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		{
			if (p_wParam == VK_F4 && (p_lParam & (1 << 29)))
			{
				newMessage->clear();

				*newMessage << WM_DESTROY;

				_systemQueue.push_back(newMessage);
			}
			else
			{
				unsigned int value = static_cast<unsigned int>(p_wParam);

				*newMessage << value;

				_keyboardQueue.push_back(newMessage);
			}
			break;
		}

		default:
		{
			return DefWindowProc(p_hwnd, p_uMsg, p_wParam, p_lParam);
		}
		}
		return TRUE;
	}

	APIModule::APIModule()
	{
		MessagePoolInstance::instanciate();
	}

	APIModule::~APIModule()
	{
		MessagePoolInstance::release();
	}

	void APIModule::pullMessage()
	{
		MSG msg = {};

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			int translateResult = TranslateMessage(&msg);
			int dispatchResult = static_cast<int>(DispatchMessage(&msg));
		}
	}
}