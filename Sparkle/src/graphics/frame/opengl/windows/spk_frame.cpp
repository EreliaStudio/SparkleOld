#include "graphics/frame/spk_frame.hpp"

namespace spk
{
        void Frame::_convertTitle(const std::wstring &p_title)
        {
            _convertedTitle = new wchar_t[p_title.size() + 1];
            for (size_t i = 0; i < p_title.size(); i++)
            {
                _convertedTitle[i] = p_title[i];
            }
            _convertedTitle[p_title.size()] = '\0';
        }

        void Frame::_createGhostInstance()
        {
            _hInstance = GetModuleHandle(NULL);
        }

        void Frame::_registerFrameClass()
        {
            _frameClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            _frameClass.lpfnWndProc = (WNDPROC) WindowProc;
            _frameClass.cbClsExtra = 0;
            _frameClass.cbWndExtra = 0;
            _frameClass.hInstance = _hInstance;
            _frameClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
            _frameClass.hCursor = LoadCursor(NULL, IDC_ARROW);
            _frameClass.hbrBackground = NULL;
            _frameClass.lpszMenuName = NULL;
            _frameClass.lpszClassName = _convertedTitle;

            RegisterClassW(&_frameClass);
        }

        void Frame::_createWindowFrame(void *p_APIModule, const Vector2UInt &p_size)
        {
            _windowSize.left = (long)0;
            _windowSize.right = (long)p_size.x;
            _windowSize.top = (long)0;
            _windowSize.bottom = (long)p_size.y;

            _windowExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
            _windowStyle = WS_OVERLAPPEDWINDOW;

            AdjustWindowRectEx(&_windowSize, _windowStyle, FALSE, _windowExStyle);

            _windowFrame = CreateWindowEx(_windowExStyle, (LPCTSTR)(_convertedTitle), (LPCTSTR)(_convertedTitle), WS_OVERLAPPEDWINDOW,
                                          CW_USEDEFAULT, 0, _windowSize.right - _windowSize.left, _windowSize.bottom - _windowSize.top, NULL, NULL, _hInstance, p_APIModule);
        }

        void Frame::_activeWindow()
        {
            ShowWindow(_windowFrame, SW_SHOW);
            UpdateWindow(_windowFrame);
        }

        void Frame::_onResize()
        {
            
        }

		void Frame::_onSetSize()
		{
			_wasResized = true;
		}

        Frame::Frame(const std::wstring &p_title, const Vector2UInt &p_size, void *p_ptr) :
            spk::AbstractFrame(p_size)
        {
            _convertTitle(p_title);

            _createGhostInstance();

            _registerFrameClass();

            _createWindowFrame(p_ptr, p_size);

            _activeWindow();
        }

        Frame::~Frame()
        {
            delete _convertedTitle;
        }
        
        HINSTANCE Frame::hInstance() const
        {
            return (_hInstance);
        }
        
        HWND Frame::windowFrame() const
        {
            return (_windowFrame);
        }

		bool Frame::wasResized() const
		{
			return (_wasResized);
		};

		void Frame::resizeHandled()
		{
			_wasResized = false;
		}

        void Frame::render()
        {

        }

        void Frame::clear()
        {
            InvalidateRect(_windowFrame, NULL, TRUE);
        }
}