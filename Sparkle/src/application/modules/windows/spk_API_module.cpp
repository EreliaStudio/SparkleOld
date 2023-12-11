#include "application/modules/spk_API_module.hpp"
#include "iostream/spk_iostream.hpp"

namespace spk
{
	LRESULT CALLBACK WindowProc(HWND p_hwnd, UINT p_uMsg, WPARAM p_wParam, LPARAM p_lParam)
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
			return pThis->handleMessage(p_hwnd, p_uMsg, p_wParam, p_lParam);
		}
		else
		{
			return DefWindowProc(p_hwnd, p_uMsg, p_wParam, p_lParam);
		}
	}

	APIModule::MessagePool::Object APIModule::_obtain(const UINT& p_uMsg)
	{
		MessagePool::Object result = _messagePool.obtain();

		result->clear();

		*result << p_uMsg;

		return (std::move(result));
	}

	LRESULT APIModule::handleMessage(const HWND& p_hwnd, const UINT& p_uMsg, const WPARAM& p_wParam, const LPARAM& p_lParam)
	{
		switch (p_uMsg)
		{
		case WM_DESTROY:
		{
			_systemQueue.push_back(_obtain(p_uMsg));
			break;
		}
		case WM_MDIRESTORE:
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
		{			
			_windowQueue.push_back(_obtain(p_uMsg));
			break;
		}
		case WM_MOVE:
		case WM_SIZE:
		{
			MessagePool::Object newMessage = _obtain(p_uMsg);
			
			unsigned int width = LOWORD(p_lParam);
			unsigned int height = HIWORD(p_lParam);

			*newMessage << width;
			*newMessage << height;

			_windowQueue.push_back(std::move(newMessage));
			break;
		}

		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		{
			_mouseQueue.push_back(std::move(_obtain(p_uMsg)));
			break;
		}
		case WM_XBUTTONDOWN :
		case WM_XBUTTONUP :
		{
			MessagePool::Object newMessage = _obtain(p_uMsg);
			
			short value = GET_XBUTTON_WPARAM (p_wParam);

			*newMessage << value;

			_mouseQueue.push_back(std::move(newMessage));
			break;
		}
		case WM_MOUSEHWHEEL:
		case WM_MOUSEWHEEL:
		{
			MessagePool::Object newMessage = _obtain(p_uMsg);
			
			short value = GET_WHEEL_DELTA_WPARAM(p_wParam);

			*newMessage << value;

			_mouseQueue.push_back(std::move(newMessage));
			break;
		}
		case WM_MOUSEMOVE:
		{
			MessagePool::Object newMessage = _obtain(p_uMsg);
			
			int x = LOWORD(p_lParam);
			int y = HIWORD(p_lParam);

			*newMessage << x;
			*newMessage << y;

			_mouseQueue.push_back(std::move(newMessage));
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
			MessagePool::Object newMessage = _obtain(p_uMsg);
			
			if (p_wParam == VK_F4 && (p_lParam & (1 << 29)))
			{
				newMessage->clear();

				*newMessage << WM_DESTROY;

				_systemQueue.push_back(std::move(newMessage));
			}
			else
			{
				unsigned int value = static_cast<unsigned int>(p_wParam);

				*newMessage << value;

				_keyboardQueue.push_back(std::move(newMessage));
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

	}

	APIModule::~APIModule()
	{
		
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