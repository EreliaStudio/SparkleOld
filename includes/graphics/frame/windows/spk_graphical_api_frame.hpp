#pragma once

#include "graphics/frame/spk_graphical_api_abstract_frame.hpp"
#include "design_pattern/spk_singleton.hpp"
#include "math/spk_vector2.hpp"
#include "spk_system_define.hpp"

namespace spk
{
    LRESULT CALLBACK WindowProc(HWND p_hwnd, UINT p_uMsg, WPARAM p_wParam, LPARAM p_lParam);
}

namespace spk::GraphicalAPI
{

    class Frame : public spk::GraphicalAPI::AbstractFrame
    {
        friend class APIModule;

    private:
        Vector2UInt _size;
        wchar_t *_convertedTitle;
        HINSTANCE _hInstance;
        HWND _windowFrame;
        RECT _windowSize;
        WNDCLASS _frameClass;
        DWORD _windowStyle;
        DWORD _windowExStyle;

        void _convertTitle(const std::wstring &p_title)
        {
            _convertedTitle = new wchar_t[p_title.size() + 1];
            for (size_t i = 0; i < p_title.size(); i++)
            {
                _convertedTitle[i] = p_title[i];
            }
            _convertedTitle[p_title.size()] = '\0';
        }

        void _createGhostInstance()
        {
            _hInstance = GetModuleHandle(NULL);
        }

        void _registerFrameClass()
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

            RegisterClass(&_frameClass);
        }

        void _createWindowFrame(void *p_APIModule, const Vector2UInt &p_size)
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

        void _activeWindow()
        {
            ShowWindow(_windowFrame, SW_SHOW);
            UpdateWindow(_windowFrame);
        }

        void _onResize()
        {

        }

		void _onSetSize()
		{

		}

    public:
        Frame(const std::wstring &p_title, const Vector2UInt &p_size, void *p_ptr = nullptr) :
            spk::GraphicalAPI::AbstractFrame(p_size)
        {
            _convertTitle(p_title);

            _createGhostInstance();

            _registerFrameClass();

            _createWindowFrame(p_ptr, p_size);

            _activeWindow();
        }

        ~Frame()
        {
            delete _convertedTitle;
        }

        void render()
        {

        }

        void clear()
        {
            InvalidateRect(_windowFrame, NULL, TRUE);
        }
    };
}