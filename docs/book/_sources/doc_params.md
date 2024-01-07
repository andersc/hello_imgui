# Application parameters

_RunnerParams_ contains all the settings and callbacks in order to run an application. 
The diagram below summarize all the possible settings and callbacks (which are explained in detail later in this document).

[![diagram](https://raw.githubusercontent.com/pthom/hello_imgui/master/src/hello_imgui/doc_src/hello_imgui_diagram.png)](https://raw.githubusercontent.com/pthom/hello_imgui/master/src/hello_imgui/doc_src/hello_imgui_diagram.png)

# RunnerParams

See [runner_params.h](https://github.com/pthom/hello_imgui/blob/master/src/hello_imgui/runner_params.h).


## Simple runner params

**SimpleRunnerParams** is a struct that contains simpler params adapted for simple use cases.

 Members:
* `guiFunction`: _VoidFunction_.
  Function that renders the Gui.
* `windowTitle`: _string, default=""_.
  Title of the application window
* `windowSizeAuto`: _bool, default=false_.
  If true, the size of the window will be computed from its widgets.
* `windowRestorePreviousGeometry`: _bool, default=true_.
  If true, restore the size and position of the window between runs.
* `windowSize`: _ScreenSize, default={800, 600}_.
  Size of the window
* `fpsIdle`: _float, default=9_.
  FPS of the application when idle (set to 0 for full speed).

For example, this is sufficient to run an application:

```cpp
void MyGui() {
    ImGui::Text("Hello, world");
    if (ImGui::Button("Exit"))
        HelloImGui::GetRunnerParams()->appShallExit = true;
}

int main(){
    auto params = HelloImGui::SimpleRunnerParams {.guiFunction = MyGui, .windowSizeAuto = true, .windowTitle = "Example"};
    HelloImGui::Run(params);
}
```


## Full params

**RunnerParams** is a struct that contains all the settings and callbacks needed to run an application.

 Members:
* `callbacks`: _see runner_callbacks.h_
   callbacks.ShowGui() will render the gui, ShowMenus() will show the menus, etc.
* `appWindowParams`: _see app_window_params.h_
   application Window Params (position, size, title)
* `imGuiWindowParams`: _see imgui_window_params.h_
   imgui window params (use docking, showMenuBar, ProvideFullScreenWindow, etc)
* `dockingParams`: _see docking_params.h_
   dockable windows content and layout
* `alternativeDockingLayouts`: _vector<DockingParams>, default=empty_
   List of possible additional layout for the applications. Only used in advanced cases when several layouts are available.
* `rememberSelectedAlternativeLayout`: _bool, default=true_
   Shall the application remember the last selected layout. Only used in advanced cases when several layouts are available.
* `backendPointers`: _see backend_pointers.h_
   A struct that contains optional pointers to the backend implementations. These pointers will be filled
   when the application starts
* `backendType`: _enum BackendType, default=BackendType::FirstAvailable_
  Select the wanted platform backend type between `Sdl`, `Glfw`. Only useful when multiple backend are compiled
  and available.
* `fpsIdling`: _FpsIdling_. Idling parameters (set fpsIdling.enableIdling to false to disable Idling)
* `useImGuiTestEngine`: _bool, default=false_.
  Set this to true if you intend to use imgui_test_engine (please read note below)

* `iniFolderType`: _IniFolderType, default = IniFolderType::CurrentFolder_
  Sets the folder where imgui will save its params.
  (possible values are: CurrentFolder, AppUserConfigFolder, DocumentsFolder, HomeFolder, TempFolder, AppExecutableFolder)
   AppUserConfigFolder is [Home]/AppData/Roaming under Windows, ~/.config under Linux, ~/Library/Application Support"
   under macOS)
* `iniFilename`: _string, default = ""_
  Sets the ini filename under which imgui will save its params. Its path is relative to the path given by iniFolderType,
  and can include a subfolder (which will be created if needed).
  If iniFilename empty, then it will be derived from appWindowParams.windowTitle (if both are empty, the ini filename will be imgui.ini).
* `iniFilename_useAppWindowTitle`: _bool, default = true_.
  Shall the iniFilename be derived from appWindowParams.windowTitle (if not empty)

 * `appShallExit`: _bool, default=false_.
  During execution, set this to true to exit the app.
  _Note: 'appShallExit' has no effect on Mobile Devices (iOS, Android) and under emscripten, since these apps
  shall not exit._
* `emscripten_fps`: _int, default = 0_.
  Set the application refresh rate (only used on emscripten: 0 stands for "let the app or the browser decide")

Notes about the use of [Dear ImGui Test & Automation Engine](https://github.com/ocornut/imgui_test_engine):
* HelloImGui must be compiled with the option HELLOIMGUI_WITH_TEST_ENGINE (-DHELLOIMGUI_WITH_TEST_ENGINE=ON)
* See demo in src/hello_imgui_demos/hello_imgui_demo_test_engine.
* imgui_test_engine is subject to a [specific license](https://github.com/ocornut/imgui_test_engine/blob/main/imgui_test_engine/LICENSE.txt)
  (TL;DR: free for individuals, educational, open-source and small businesses uses. Paid for larger businesses.)



# Fps Idling

See [runner_params.h](https://github.com/pthom/hello_imgui/blob/master/src/hello_imgui/runner_params.h).


**FpsIdling** is a struct that contains Fps Idling parameters

* `fpsIdle`: _float, default=9_.
  ImGui applications can consume a lot of CPU, since they update the screen very frequently.
  In order to reduce the CPU usage, the FPS is reduced when no user interaction is detected.
  This is ok most of the time but if you are displaying animated widgets (for example a live video),
  you may want to ask for a faster refresh: either increase fpsIdle, or set it to 0 for maximum refresh speed
  (you can change this value during the execution depending on your application refresh needs)
* `enableIdling`: _bool, default=true_.
  Set this to false to disable idling (this can be changed dynamically during execution)
* `isIdling`: bool (dynamically updated during execution)
  This bool will be updated during the application execution, and will be set to true when it is idling.
* `rememberEnableIdling`: _bool, default=true_.
  If true, the last value of enableIdling is restored from the settings at startup.


----

# Runner callbacks

See [runner_callbacks.h](https://github.com/pthom/hello_imgui/blob/master/src/hello_imgui/runner_callbacks.h).



 **RunnerCallbacks** is a struct that contains the callbacks that are called by the application

 _Members_

* `ShowGui`: *VoidFunction, default=empty*.
  Fill it with a function that will add your widgets.

* `ShowMenus`: *VoidFunction, default=empty*.
   A function that will render your menus. Fill it with a function that will add ImGui menus by calling:
   _ImGui::BeginMenu(...) / ImGui::MenuItem(...) / ImGui::EndMenu()_

   _Notes:_
   * you do not need to call _ImGui::BeginMenuBar_ and _ImGui::EndMenuBar_
   * Some default menus can be provided: see _ImGuiWindowParams_ options
      (_showMenuBar, showMenu_App_QuitAbout, showMenu_View_)

* `ShowAppMenuItems`: *VoidFunction, default=empty*.
  A function that will render items that will be placed in the App menu.
  They will be placed before the "Quit" MenuItem, which is added automatically by HelloImGui.
  This will be displayed only if ImGuiWindowParams.showMenu_App is true

* `ShowStatus`: *VoidFunction, default=empty*.
  A function that will add items to the status bar. Use small items (ImGui::Text for example),
  since the height of the status is 30. Also, remember to call ImGui::SameLine() between items.

* `PostInit`: *VoidFunction, default=empty*.
  You can here add a function that will be called once after OpenGL and ImGui are inited, but before
  the backend callback are initialized.
  If you, for instance, want to add your own glfw callbacks, you should use this function to do so."

* `BeforeExit`: *VoidFunction, default=empty*.
  You can here add a function that will be called once before exiting (when OpenGL and ImGui are
  still inited)

* `BeforeExit_PostCleanup`: *VoidFunction, default=empty*.
  You can here add a function that will be called once before exiting (after OpenGL and ImGui have been deinited)

* `PreNewFrame`: *VoidFunction, default=empty*.
  You can here add a function that will be called at each frame, and before the call to ImGui::NewFrame().
  It is a good place to dynamically add new fonts, or dynamically add new dockable windows.

* `BeforeImGuiRender`: *VoidFunction, default=empty*.
  You can here add a function that will be called at each frame, after the user Gui code,
  and just before the call to ImGui::Render() (which will also call ImGui::EndFrame()).

* `AfterSwap`: *VoidFunction, default=empty*.
  You can here add a function that will be called at each frame, after the Gui was rendered
  and swapped to the screen.

* `CustomBackground`: *VoidFunction, default=empty*.
  By default, the background is cleared using ImGuiWindowParams.backgroundColor. If set, this function
  instead gives you full control over the background that is drawn behind the Gui. An example use case
  is if you have a 3D application like a mesh editor, or game, and just want the Gui to be drawn on top
  of that content.

* `AnyBackendEventCallback`: *AnyBackendCallback, default=empty*.
  Callbacks for events from a specific backend. _Only implemented for SDL, where the event
  will be of type 'SDL_Event *'_
  This callback should return true if the event was handled and shall not be processed further.
  Note: in the case of GLFW, you should use register them in `PostInit`

* `LoadAdditionalFonts`: *VoidFunction, default=_LoadDefaultFont_WithFontAwesome*.
  A function that is called once, when fonts are ready to be loaded.
  By default, _LoadDefaultFont_WithFontAwesome_ is called but you can copy and customize it.
  (LoadDefaultFont_WithFontAwesome will load from assets/fonts/ but reverts to the ImGui embedded font if not found)

* `SetupImGuiConfig`: *VoidFunction, default=_ImGuiDefaultSettings::SetupDefaultImGuiConfig*.
  If needed, change ImGui config via SetupImGuiConfig (enable docking, gamepad, etc)

* `SetupImGuiStyle`: *VoidFunction, default=_ImGuiDefaultSettings::SetupDefaultImGuiConfig*.
  If needed, setup your own style by providing your own SetupImGuiStyle callback

* `RegisterTests`: *VoidFunction, default=empty*.
  A function that is called once ImGuiTestEngine is ready to be filled with tests and automations definitions.

* `mobileCallbacks`: *_MobileCallbacks_*. Callbacks that are called by the application
  when running under "Android, iOS and WinRT".
Notes:
  * 'mobileCallbacks' is present only if the target device is a mobile device (iOS, Android).
    Use `#ifdef HELLOIMGUI_MOBILEDEVICE` to detect this.
  * These events are currently handled only with SDL backend.



**VoidFunctionPointer** can hold any void(void) function.
```cpp
using VoidFunction = std::function<void(void)>
```

**AnyEventCallback** can hold any bool(void *) function.
  It is designed to handle callbacks for a specific backend.
```cpp
using AnyEventCallback = std::function<bool(void * backendEvent)>
```

**AppendCallback** can compose two callbacks. Use this when you want to set a callback and keep the (maybe) preexisting one.

## MobileCallbacks


**MobileCallbacks** is a struct that contains callbacks that are called by the application
 when running under "Android, iOS and WinRT".
 These events are specific to mobile and embedded devices that have different requirements
 than your usual desktop application. These events must be handled quickly,
 since often the OS needs an immediate response and will terminate your process shortly
 after sending the event if you do not handle them apprpriately.

 Note: on mobile devices, it is not possible to "Quit" an application, it can only be put on Pause.

 * `OnDestroy`: _VoidFunction, default=empty_. The application is being terminated by the OS.
 * `OnLowMemory`: _VoidFunction, default=empty_. The application is low on memory, free memory if possible.
 * `OnPause`: _VoidFunction, default=empty_. The application is about to enter the background.
 * `OnResume`: _VoidFunction, default=empty_. The application is has come to foreground and is now interactive.

 Note: 'OnPause' and 'OnResume' are called twice consecutively under iOS (before and after entering background
 or foreground).


----

# Application window params

See [app_window_params.h](https://github.com/pthom/hello_imgui/blob/master/src/hello_imgui/app_window_params.h).

## AppWindowParams


__AppWindowParams__ is a struct that defines the application window display params.
See [doc_src/hello_imgui_diagram.png](https://raw.githubusercontent.com/pthom/hello_imgui/master/src/hello_imgui/doc_src/hello_imgui_diagram.png)
for details.

Members:
* `windowTitle`: _string, default=""_. Title of the application window
* `windowGeometry`: _WindowGeometry_
  Enables to precisely set the window geometry (position, monitor, size, full screen, fake full screen, etc.)
   _Note: on a mobile device, the application will always be full screen._
* `restorePreviousGeometry`: _bool, default=false_.
  If true, then save & restore windowGeometry from last run (the geometry will be written in imgui_app_window.ini)
* `resizable`: _bool, default = false_. Should the window be resizable. This is taken into account at
  creation.
* `hidden`: _bool, default = false_. Should the window be hidden. This is taken into account dynamically (you
  can show/hide the window with this). Full screen windows cannot be hidden.

* `borderless`: _bool, default = false_. Should the window have borders. This is taken into account at creation.
* `borderlessMovable`: _bool, default = true_. If the window is borderless, should it be movable.
   If so, a drag zone is displayed at the top of the window when the mouse is over it.
* `borderlessResizable`: _bool, default = true_. If the window is borderless, should it be resizable.
   If so, a drag zone is displayed at the bottom-right of the window when the mouse is over it.
* `borderlessClosable`: _bool, default = false_. If the window is borderless, should it have a close button.
   If so, a close button is displayed at the top-right of the window when the mouse is over it.
* `borderlessHighlightColor`: _ImVec4, default = ImVec4(0.2f, 0.4f, 1.f, 0.f)_.
   Color of the highlight displayed on resize/move zones. If borderlessHighlightColor.w==0,
   then the highlightColor will be automatically set to ImGui::GetColorU32(ImGuiCol_TitleBgActive, 0.6f)

* `edgeInsets`: _EdgeInsets_. iOS only, out values filled by HelloImGui:
  if there is a notch on the iPhone, you should not display inside these insets.
  HelloImGui handles this automatically, if handleEdgeInsets is true and
  if runnerParams.imGuiWindowParams.defaultImGuiWindowType is not NoDefaultWindow.
  (warning, these values are updated only after a few frames, they are typically 0 for the first 4 frames)
* `handleEdgeInsets`: _bool, default = true_. iOS only, if true, HelloImGui will handle the edgeInsets.


## WindowGeometry


__WindowGeometry__ is a struct that defines the window geometry.

Members:
* `size`: _int[2], default="{800, 600}"_. Size of the application window
  used if fullScreenMode==NoFullScreen and sizeAuto==false
* `sizeAuto`: _bool, default=false_
  If true, adapt the app window size to the presented widgets.
  After the first frame was displayed, HelloImGui will measure its size,
  and the backend application window will be resized. As a consequence, the application window size may
  vary between the first and the second frame.

* `fullScreenMode`: _FullScreenMode, default=NoFullScreen_.
   You can choose between several full screen modes:
   ```cpp
        NoFullScreen,
        FullScreen,                    // Full screen with specified resolution
        FullScreenDesktopResolution,   // Full screen with current desktop mode & resolution
        FullMonitorWorkArea            // Fake full screen, maximized window on the selected monitor
    ```
* `positionMode`: _WindowPositionMode, default = OsDefault_.
   You can choose between several window position modes:
   ```cpp
        OsDefault,
        MonitorCenter,
        FromCoords,
    ```
* `monitorIdx`: _int, default = 0_.
  used if positionMode==MonitorCenter or if fullScreenMode!=NoFullScreen
* `windowSizeState`: _WindowSizeState, default=Standard_
   You can choose between several window size states:
   ```cpp
        Standard,
        Minimized,
        Maximized
    ```
* `windowSizeMeasureMode`: _WindowSizeMeasureMode_, default=RelativeTo96Ppi
  how the window size is specified:
  * RelativeTo96Ppi enables to give screen size that are independant from the screen density.
     For example, a window size expressed as 800x600 will correspond to a size
        - 800x600 (in screen coords) if the monitor dpi is 96
        - 1600x120 (in screen coords) if the monitor dpi is 192
      (this works with Glfw. With SDL, it only works under windows)
  * ScreenCoords: measure window size in screen coords
    (Note: screen coordinates might differ from real pixels on high dpi screen)

* `resizeAppWindowAtNextFrame`: _bool_, default=false;
  If you set this to flag to true at any point during the execution, the application window
  will then try to resize based on its content on the next displayed frame,
  and this flag will subsequently be set to false.
  Example:
  ```cpp
  // Will resize the app window at next displayed frame
  HelloImGui::GetRunnerParams()->appWindowParams.windowGeometry.resizeAppWindowAtNextFrame = true;
  ```
  Note: this flag is intended to be used during execution, not at startup (use sizeAuto at startup).

----

# ImGui window params

See [imgui_window_params.h](https://github.com/pthom/hello_imgui/blob/master/src/hello_imgui/imgui_window_params.h).

## ImGuiWindowParams


__ImGuiWindowParams__ is a struct that defines the ImGui inner windows params
These settings affect the imgui inner windows inside the application window.
In order to change the application window settings, change the _AppWindowsParams_

 Members:

  * `defaultImGuiWindowType`: _DefaultImGuiWindowType, default=ProvideFullScreenWindow_.
    By default, a full window is provided in the background. You can still
     add windows on top of it, since the Z-order of this background window is always behind

  * `backgroundColor`: _ImVec4, default=ImVec4(0.45f, 0.55f, 0.60f, 1.00f)_.
    This is the "clearColor", visible if defaultImGuiWindowType is not ProvideFullScreenWindow.
    Alternatively, you can set your own RunnerCallbacks.CustomBackground to have full
    control over what is drawn behind the Gui.

  * `showMenuBar`: _bool, default=false_.
    Show Menu bar on top of imgui main window.
    In order to fully customize the menu, set showMenuBar to true, and set showMenu_App and showMenu_View params to false.
    Then, implement the callback `RunnerParams.callbacks.ShowMenus` which can optionally call `HelloImGui::ShowViewMenu`
    and `HelloImGui::ShowAppMenu`.

  * `showMenu_App`: _bool, default=true_.
    If menu bar is shown, include or not the default app menu

   * `showMenu_App_Quit`: _bool, default=true_.
    Include or not a "Quit" item in the default app menu.
    Set this to false if you intend to provide your own quit callback with possible user confirmation
    (and implement it inside RunnerCallbacks.ShowAppMenuItems)

  * `showMenu_View`: _bool, default=true_.
    If menu bar is shown, include or not the default _View_ menu, that enables to change the layout and
    set the docked windows and status bar visibility)

  * `showStatusBar`: _bool, default=false_.
    Flag that enable to show a Status bar at the bottom. You can customize the status bar
    via RunnerCallbacks.ShowStatus()
  * `showStatus_Fps`: _bool, default=true_. If set, display the FPS in the status bar.
  * `rememberStatusBarSettings`: _bool, default=true_. If set, showStatusBar and showStatus_Fps are stored in the application settings.

  * `configWindowsMoveFromTitleBarOnly`: _bool, default=true_.
    Make windows only movable from the title bar

  * `enableViewports`: _bool, default=false_. Enable multiple viewports (i.e multiple native windows)
    If true, you can drag windows outside out the main window in order to put their content into new native windows.

   * `menuAppTitle`: _string, default=""_. Set the title of the App menu. If empty, this menu name will use
     the "windowTitle" from AppWindowParams

  * `tweakedTheme`: _ImGuiTheme::ImGuiTweakedTheme_.
    Change the ImGui theme. Several themes are available, you can query the list by calling
    HelloImGui::AvailableThemes()
  * `showMenu_View_Themes`: _bool, default=true_.
    Show theme selection in view menu
  * `rememberTheme`: _bool, default=true_.
    Remember selected theme

## Default window types


__DefaultImGuiWindowType__ is an enum class that defines whether or not a full screen background window is provided.

 Values:
  * _ProvideFullScreenWindow_: a full window is provided in the background
  * _ProvideFullScreenDockSpace_: a full screen dockspace is provided in the background
  * _NoDefaultWindow_: No default window is provided (except for ImGui's default "debug" window)


----

# Docking

See [docking_params.h](https://github.com/pthom/hello_imgui/blob/master/src/hello_imgui/docking_params.h).


HelloImGui makes it easy to use dockable windows (based on ImGui [docking branch](https://github.com/ocornut/imgui/tree/docking)).

You can define several layouts and switch between them:  each layout which will remember the user modifications and the list of opened windows

HelloImGui will then provide a "View" menu with options to show/hide the dockable windows, restore the default layout, switch between layouts, etc.

![demo docking](https://traineq.org/ImGuiBundle/HelloImGuiLayout.gif)

* Source for this example: [src/hello_imgui_demos/hello_imgui_demodocking](https://github.com/pthom/hello_imgui/tree/master/src/hello_imgui_demos/hello_imgui_demodocking)
* [Video explanation on YouTube](https://www.youtube.com/watch?v=XKxmz__F4ow) (5 minutes)


The different available layouts are provided inside RunnerParams via the two members below:
```cpp
struct RunnerParams
{
    ...
    DockingParams dockingParams;                            // default layout of the application
    std::vector<DockingParams> alternativeDockingLayouts;   // optional alternative layouts
    ...
};
```

And `DockingParams` contains members that define a layout:

```cpp
struct DockingParams
{
    std::string layoutName = "Default";          // displayed name of the layout
    std::vector<DockingSplit> dockingSplits;     // list of splits (which define spaces where the windows will be placed)
    std::vector<DockableWindow> dockableWindows; // list of windows (with their gui code, and specifying in which space they will be placed)
    ...
};
```

Inside DockingParams, the member `dockingSplits` specifies the layout, and the member `dockableWindows`
 specifies the list of dockable windows, along with their default location, and their code (given by lambdas).


## Docking Params: Example usage


Below is an example that shows how to instantiate a layout:

1. First, define the docking splits:

```cpp
std::vector<HelloImGui::DockingSplit> CreateDefaultDockingSplits()
{
    //     Here, we want to split "MainDockSpace" (which is provided automatically) into three zones, like this:
    //    ___________________________________________
    //    |        |                                |
    //    | Command|                                |
    //    | Space  |    MainDockSpace               |
    //    |        |                                |
    //    |        |                                |
    //    |        |                                |
    //    -------------------------------------------
    //    |     MiscSpace                           |
    //    -------------------------------------------
    //

    // add a space named "MiscSpace" whose height is 25% of the app height.
    // This will split the preexisting default dockspace "MainDockSpace" in two parts.
    HelloImGui::DockingSplit splitMainMisc;
    splitMainMisc.initialDock = "MainDockSpace";
    splitMainMisc.newDock = "MiscSpace";
    splitMainMisc.direction = ImGuiDir_Down;
    splitMainMisc.ratio = 0.25f;

    // Then, add a space to the left which occupies a column whose width is 25% of the app width
    HelloImGui::DockingSplit splitMainCommand;
    splitMainCommand.initialDock = "MainDockSpace";
    splitMainCommand.newDock = "CommandSpace";
    splitMainCommand.direction = ImGuiDir_Left;
    splitMainCommand.ratio = 0.25f;

    std::vector<HelloImGui::DockingSplit> splits {splitMainMisc, splitMainCommand};
    return splits;
}
```

2. Then, define the dockable windows:

```cpp
std::vector<HelloImGui::DockableWindow> CreateDockableWindows(AppState& appState)
{
    // A Command panel named "Commands" will be placed in "CommandSpace". Its Gui is provided calls "CommandGui"
    HelloImGui::DockableWindow commandsWindow;
    commandsWindow.label = "Commands";
    commandsWindow.dockSpaceName = "CommandSpace";
    commandsWindow.GuiFunction = [&] { CommandGui(appState); };

    // A Log window named "Logs" will be placed in "MiscSpace". It uses the HelloImGui logger gui
    HelloImGui::DockableWindow logsWindow;
    logsWindow.label = "Logs";
    logsWindow.dockSpaceName = "MiscSpace";
    logsWindow.GuiFunction = [] { HelloImGui::LogGui(); };

    ...
}
```

3. Finally, fill the RunnerParams

```cpp
HelloImGui::RunnerParams runnerParams;
runnerParams.imGuiWindowParams.defaultImGuiWindowType =
    HelloImGui::DefaultImGuiWindowType::ProvideFullScreenDockSpace;

runnerParams.dockingParams.dockingSplits = CreateDefaultDockingSplits();
runnerParams.dockingParams.dockableWindows = CreateDockableWindows();


HelloImGui::Run(runnerParams);
```


## Docking Splits


**DockingSplit** is a struct that defines the way the docking splits should be applied on the screen
in order to create new Dock Spaces. _DockingParams_ contains a _vector[DockingSplit]_,
in order to partition the screen at your will.

_Members:_

* `initialDock`: _DockSpaceName (aka string)_

    id of the space that should be split.
    At the start, there is only one Dock Space named "MainDockSpace".
    You should start by partitioning this space, in order to create a new dock space.

* `newDock`: _DockSpaceName (aka string)_. id of the new dock space that will be created.
* `direction`: *ImGuiDir_ (enum with ImGuiDir_Down, ImGuiDir_Down, ImGuiDir_Left, ImGuiDir_Right)*.
Direction where this dock space should be created.
* `ratio`: _float, default=0.25f_. Ratio of the initialDock size that should be used by the new dock space.
* `nodeFlags`: *ImGuiDockNodeFlags_ (enum)*. Flags to apply to the new dock space (enable/disable resizing, splitting, tab bar, etc.)


## Dockable window


**DockableWindow** is a struct that represents a window that can be docked.

_Members:_

* `label`: _string_. Title of the window.
* `dockSpaceName`: _DockSpaceName (aka string)_. Id of the dock space where this window
   should initially be placed
* `GuiFunction`: _VoidFunction_. Any function that will render this window's Gui.
* `isVisible`: _bool, default=true_. Flag that indicates whether this window is visible or not.
* `rememberIsVisible`: _bool, default=true_. Flag that indicates whether the window visibility should be saved in settings or not.
* `canBeClosed`: _bool, default=true_. Flag that indicates whether the user can close this window.
* `callBeginEnd`: _bool, default=true_. Flag that indicates whether ImGui::Begin and ImGui::End
   calls should be added automatically (with the given "label"). Set to false if you want to call
   ImGui::Begin/End yourself
* `includeInViewMenu`: _bool, default=true_. Flag that indicates whether this window should be mentioned
   in the view menu.
* `imGuiWindowFlags`: _ImGuiWindowFlags, default=0_. Window flags, see enum ImGuiWindowFlags_
* `windowSize`: _ImVec2, default=(0.f, 0.f) (i.e let the app decide)_. Window size (unused if docked)
* `windowSizeCondition`: _ImGuiCond, default=ImGuiCond_FirstUseEver_. When to apply the window size.
* `windowPos`: _ImVec2, default=(0.f, 0.f) (i.e let the app decide)_. Window position (unused if docked)
* `windowPosCondition`: _ImGuiCond, default=ImGuiCond_FirstUseEver_. When to apply the window position.
* `focusWindowAtNextFrame`: _bool, default = false_. If set to true this window will be focused at the next frame.


## Docking Params


**DockingParams** contains all the settings concerning the docking,
 together _with the Gui functions for the docked windows_.

 _Members:_

* `dockingSplits`: _vector[DockingSplit]_.
  Defines the way docking splits should be applied on the screen in order to create new Dock Spaces
* `dockableWindows`: _vector[DockableWindow]_.
  List of the dockable windows, together with their Gui code
* `layoutName`: _string, default="default"_.
  Displayed name of the layout. Only used in advanced cases when several layouts are available.
* `layoutCondition`: _enum DockingLayoutCondition, default=DockingLayoutCondition::FirstUseEver_.
  When to apply the docking layout. Choose between FirstUseEver (apply once, then keep user preference),
  ApplicationStart (always reapply at application start), and Never.
* `mainDockSpaceNodeFlags`: _ImGuiDockNodeFlags (enum), default=ImGuiDockNodeFlags_PassthruCentralNode_
   Flags to apply to the main dock space (enable/disable resizing, splitting, tab bar, etc.).
   Most flags are inherited by children dock spaces. You can also set flags for specific dock spaces via `DockingSplit.nodeFlags`
* `layoutReset`: _bool, default=false_.
  Reset layout on next frame, i.e. drop the layout customizations which were applied manually by the user.
  (layoutReset will be set to false after applying)

 _Helpers:_

 * `DockableWindow * dockableWindowOfName(const std::string & name)`: returns a pointer to a dockable window
 * `bool focusDockableWindow(const std::string& name)`: will focus a dockable window (and make its tab visible if needed)
 * `optional<ImGuiID> dockSpaceIdFromName(const std::string& dockSpaceName)`: may return the ImGuiID corresponding
   to the dockspace with this name.
   **Warning**: this will work reliably only if layoutCondition = DockingLayoutCondition::ApplicationStart. In other
   cases, the ID may be cached by ImGui himself at the first run, and HelloImGui will *not* know it on subsequent runs!

# Backend Pointers


**BackendPointers** is a struct that contains optional pointers to the backend implementations (for SDL and GLFW).

These pointers will be filled when the application starts, and you can use them to customize
your application behavior using the selected backend.

 Members:
* `glfwWindow`: _void *, default=nullptr_. Pointer to the main GLFW window (of type `GLFWwindow*`).
  Only filled if the backend is GLFW.
* `sdlWindow`: _void *, default=nullptr_. Pointer to the main SDL window (of type `SDL_Window*`).
  Only filled if the backend is SDL (or emscripten + sdl)
* `sdlGlContext`: _void *, default=nullptr_. Pointer to SDL's GlContext (of type `SDL_GLContext`).
  Only filled if the backend is SDL (or emscripten + sdl)

Note: If using the Metal, Vulkan or DirectX rendering backend, you can find some interesting pointers inside
 `src/hello_imgui/internal/backend_impls/rendering_metal.h`
 `src/hello_imgui/internal/backend_impls/rendering_vulkan.h`
 `src/hello_imgui/internal/backend_impls/rendering_dx11.h`
 `src/hello_imgui/internal/backend_impls/rendering_dx12.h`
