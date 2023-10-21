#pragma once

#include "graphics/frame/spk_abstract_frame.hpp"
#include "design_pattern/spk_singleton.hpp"
#include "math/spk_vector2.hpp"
#include "spk_system_define.hpp"

namespace spk
{
    LRESULT CALLBACK WindowProc(HWND p_hwnd, UINT p_uMsg, WPARAM p_wParam, LPARAM p_lParam);
}

namespace spk
{
    class Frame : public spk::AbstractFrame
    {
        friend class APIModule;

    private:
        Vector2UInt _size;
        wchar_t *_convertedTitle;
        HINSTANCE _hInstance;
        HWND _windowFrame;
        RECT _windowSize;
        WNDCLASSW _frameClass;
        DWORD _windowStyle;
        DWORD _windowExStyle;

		bool _wasResized = false;

        void _convertTitle(const std::wstring &p_title);
        void _createGhostInstance();
        void _registerFrameClass();
        void _createWindowFrame(void *p_APIModule, const Vector2UInt &p_size);

        void _activeWindow();

        void _onResize();
		void _onSetSize();

    public:
        Frame(const std::wstring &p_title, const Vector2UInt &p_size, void *p_ptr = nullptr);
        ~Frame();
        
        HINSTANCE hInstance() const;
        HWND windowFrame() const;

		bool wasResized() const;
		void resizeHandled();

        void render();
        void clear();
    };
}