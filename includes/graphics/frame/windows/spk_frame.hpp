#pragma once

#include "graphics/frame/spk_abstract_frame.hpp"
#include "design_pattern/spk_singleton.hpp"
#include "math/spk_vector2.hpp"
#include "spk_system_define.hpp"

namespace spk
{
    /// Forward declaration of Window Procedure function.
    LRESULT CALLBACK WindowProc(HWND p_hwnd, UINT p_uMsg, WPARAM p_wParam, LPARAM p_lParam);
}

namespace spk
{
    /**
     * @class Frame
     * @brief Concrete implementation of a frame using WinAPI.
     * 
     * This class extends the AbstractFrame class and provides concrete implementations
     * using WinAPI for Windows platform.
     */
    class Frame : public spk::AbstractFrame
    {
        friend class APIModule;  ///< Allow APIModule class to access private members.

    private:
        /// Frame anchor in unsigned integers.
        Vector2Int _anchor;

        /// Frame size in unsigned integers.
        Vector2UInt _size;

        /// Pointer to converted window title.
        wchar_t *_convertedTitle;

        /// Handle to the application instance.
        HINSTANCE _hInstance;

        /// Handle to the window frame.
        HWND _windowFrame;

        /// Structure containing the dimensions of the window.
        RECT _windowSize;

        /// Window class information.
        WNDCLASSW _frameClass;

        /// Window styles.
        DWORD _windowStyle;

        /// Extended window styles.
        DWORD _windowExStyle;

        /// Flag indicating if the window was resized.
        bool _wasResized = false;

        /**
         * @brief Convert the title to a wchar_t array.
         * 
         * @param p_title Title to convert.
         */
        void _convertTitle(const std::wstring &p_title);

        /// Create a ghost window instance for class registration.
        void _createGhostInstance();

        /// Register the window class.
        void _registerFrameClass();

        /**
         * @brief Create the actual window frame.
         * 
         * @param p_APIModule API module pointer.
         * @param p_size Initial frame size.
         */
        void _createWindowFrame(void *p_APIModule, const Vector2UInt &p_size);

        /// Activate the window.
        void _activeWindow();

        /// Method to handle frame resizing logic.
        void _onResize();

        /// Method to handle setting the size of the frame.
		void _onSetSize();

    public:
        /**
         * @brief Constructor for Frame.
         * 
         * @param p_title Title of the frame.
         * @param p_size Initial size of the frame.
         * @param p_ptr Optional pointer to additional data (default is nullptr).
         */
        Frame(const std::wstring &p_title, const Vector2UInt &p_size, void *p_ptr = nullptr);

        /// Destructor.
        ~Frame();

        /// Get the application instance handle.
        HINSTANCE hInstance() const;

        /// Get the window frame handle.
        HWND windowFrame() const;

        /// Check if the frame was resized.
		bool wasResized() const;

        /// Mark that the resize has been handled.
		void resizeHandled();

        /// Render the frame content.
        void render();

        /// Clear the frame content.
        void clear();
    };
}
