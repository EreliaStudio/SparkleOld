#include "application/modules/spk_API_module.hpp"

namespace spk
{
	LRESULT CALLBACK APIModule::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		APIModule *pThis = NULL;

		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT *pCreate = (CREATESTRUCT *)lParam;
			pThis = (APIModule *)pCreate->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
		}
		else
		{
			pThis = (APIModule *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}

		if (pThis)
		{
			return pThis->_handleMessage(hwnd, uMsg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	LRESULT APIModule::_handleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		MessagePool::Object newMessage = MessagePoolInstance::instance()->obtain();

		newMessage->clear();

		*newMessage << uMsg;
		switch (uMsg)
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
			unsigned int width = LOWORD(lParam);
			unsigned int height = HIWORD(lParam);

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
			short value = GET_XBUTTON_WPARAM (wParam);

			*newMessage << value;

			_mouseQueue.push_back(newMessage);
			break;
		}
		case WM_MOUSEHWHEEL:
		case WM_MOUSEWHEEL:
		{
			short value = GET_WHEEL_DELTA_WPARAM(wParam);

			*newMessage << value;

			_mouseQueue.push_back(newMessage);
			break;
		}
		case WM_MOUSEMOVE:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

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
			if (wParam == VK_F4 && (lParam & (1 << 29)))
			{
				DEBUG_LINE();

				newMessage->clear();

				*newMessage << WM_DESTROY;

				_systemQueue.push_back(newMessage);
			}
			else
			{
				unsigned int value = static_cast<unsigned int>(wParam);

				*newMessage << value;

				_keyboardQueue.push_back(newMessage);
			}
			break;
		}

		default:
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
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

	void APIModule::update()
	{
		MSG msg = {};

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			int translateResult = TranslateMessage(&msg);
			int dispatchResult = static_cast<int>(DispatchMessage(&msg));
		}
	}
}