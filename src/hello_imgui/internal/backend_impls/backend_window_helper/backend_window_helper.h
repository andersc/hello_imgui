#pragma once
#include "hello_imgui/app_window_params.h"

#include <string>
#include <iostream>
#include <optional>


#define BACKEND_THROW_IMPL(msg) \
    { \
        std::cerr << "throw runtime_error: " << msg << "\t\t at " << __FILE__ << ":" << __LINE__ << "\n"; \
        throw std::runtime_error(msg); \
    }

#define BACKEND_THROW(...) BACKEND_THROW_IMPL((__VA_ARGS__));


namespace HelloImGui { namespace BackendApi
{
    enum class Backend3dMode
    {
        No3d,
        OpenGl,
        Vulkan,
        Metal,
        DirectX
    };


    struct BackendOptions
    {
        // If the window is created with the `allowHighDpi` flag, its size
        // * in pixels may differ from its size in screen coordinates on platforms with
        // * high-DPI support (e.g. iOS and macOS)
        bool allowHighDpi = true;

        Backend3dMode backend3DMode = Backend3dMode::OpenGl;
    };


    // Container for pointers to SDLWindow, GLFWwindow, etc.
    using WindowPointer = void *;


    class IBackendWindowHelper
    {
        // Note: this is a fake class, it has no member
        // It is only a class in order to enforce a consistent API between backends.
    public:
        virtual WindowPointer CreateWindow(AppWindowParams &info, const BackendOptions& backendOptions) = 0;

        virtual size_t GetNbMonitors() = 0;

        //The area of a monitor not occupied by global task bars or menu bars is the work area.
        //This is specified in screen coordinates.
        //Pixels and screen coordinates may map 1:1 on your machine, but they won't on every other machine,
        //for example on a Mac with a Retina display.
        virtual ScreenBounds GetOneMonitorWorkArea(int monitorIndex) = 0;

        virtual bool IsWindowIconified(WindowPointer window) = 0;

        virtual void RaiseWindow(WindowPointer window) = 0;

        virtual ScreenBounds GetWindowBounds(WindowPointer window) = 0;

        virtual void SetWindowBounds(WindowPointer window, ScreenBounds windowBounds) = 0;

        // Screenshot!

        //
        //  Concrete methods below
        //
        struct SearchForMonitorResult
        {
            int monitorIdx;
            std::optional<ScreenPosition> newPosition;
        };
        SearchForMonitorResult SearchForMonitor(const WindowGeometry& geometry);

        void EnsureWindowFitsMonitor(WindowPointer window, int idxMonitor, FullScreenMode fullScreenMode);
    };
}} // namespace HelloImGui { namespace BackendApi
