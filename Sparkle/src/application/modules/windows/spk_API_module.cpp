#include "application/modules/spk_API_module.hpp"
#include "iostream/spk_iostream.hpp"

namespace spk
{
	LRESULT CALLBACK WindowProc(HWND p_hwnd, UINT p_messageID, WPARAM p_firstParam, LPARAM p_secondParam)
	{
		static APIModule *pThis = NULL;

		if (p_messageID == WM_NCCREATE)
		{
			CREATESTRUCT *pCreate = (CREATESTRUCT *)p_secondParam;
			pThis = (APIModule *)pCreate->lpCreateParams;
			SetWindowLongPtr(p_hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
		}

		if (pThis != NULL)
		{
			return pThis->handleMessage(p_hwnd, p_messageID, p_firstParam, p_secondParam);
		}
		else
		{
			return DefWindowProc(p_hwnd, p_messageID, p_firstParam, p_secondParam);
		}
	}

	APIModule::MessagePool::Object APIModule::_obtainMessage(const UINT& p_messageID)
	{
		MessagePool::Object result = MessagePoolInstance::instance()->obtain();

		result->clear();

		*result << p_messageID;

		return (std::move(result));
	}

	LRESULT APIModule::handleMessage(const HWND& p_hwnd, const UINT& p_messageID, const WPARAM& p_firstParam, const LPARAM& p_secondParam)
	{
		switch (p_messageID)
		{
		case WM_DESTROY:
		{
			MessagePool::Object newMessage = _obtainMessage(p_messageID);

			_systemQueue.push_back(std::move(newMessage));
			break;
		}
		case WM_MDIRESTORE:
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
		{
			MessagePool::Object newMessage = _obtainMessage(p_messageID);
			
			_windowQueue.push_back(std::move(newMessage));
			break;
		}
		case WM_MOVE:
		case WM_SIZE:
		{
			MessagePool::Object newMessage = _obtainMessage(p_messageID);
			
			unsigned int width = LOWORD(p_secondParam);
			unsigned int height = HIWORD(p_secondParam);

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
			MessagePool::Object newMessage = _obtainMessage(p_messageID);
			
			_mouseQueue.push_back(std::move(newMessage));
			break;
		}
		case WM_XBUTTONDOWN :
		case WM_XBUTTONUP :
		{
			MessagePool::Object newMessage = _obtainMessage(p_messageID);
			
			short value = GET_XBUTTON_WPARAM (p_firstParam);

			*newMessage << value;

			_mouseQueue.push_back(std::move(newMessage));
			break;
		}
		case WM_MOUSEHWHEEL:
		case WM_MOUSEWHEEL:
		{
			MessagePool::Object newMessage = _obtainMessage(p_messageID);
			
			short value = GET_WHEEL_DELTA_WPARAM(p_firstParam);

			*newMessage << value;

			_mouseQueue.push_back(std::move(newMessage));
			break;
		}
		case WM_MOUSEMOVE:
		{
			MessagePool::Object newMessage = _obtainMessage(p_messageID);
			
			int x = LOWORD(p_secondParam);
			int y = HIWORD(p_secondParam);

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
			MessagePool::Object newMessage = _obtainMessage(p_messageID);
			
			if (p_firstParam == VK_F4 && (p_secondParam & (1 << 29)))
			{
				newMessage->clear();

				*newMessage << WM_DESTROY;

				_systemQueue.push_back(std::move(newMessage));
			}
			else
			{
				unsigned int value = static_cast<unsigned int>(p_firstParam);

				*newMessage << value;

				_keyboardQueue.push_back(std::move(newMessage));
			}
			break;
		}

		default:
		{
			return DefWindowProc(p_hwnd, p_messageID, p_firstParam, p_secondParam);
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