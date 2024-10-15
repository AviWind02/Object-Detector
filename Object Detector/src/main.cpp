#include "pch.h"

int main()
{

    // Create application window
    HWND hwnd = core::window::Create(100, 100, core::window::windowSize.x, core::window::windowSize.y);
    // Initialize Direct3D
    if (!core::renderer::CreateDeviceD3D(hwnd))
    {
        core::renderer::CleanupDeviceD3D();
        core::window::Destroy(hwnd);
        return 1;
    }

    // Show the window
    core::window::Show(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = NULL;

    // Setup ImGui
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(core::renderer::g_pd3dDevice, core::renderer::g_pd3dDeviceContext);


    // Main loop
    core::window::MessageLoop(hwnd);

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    core::renderer::CleanupDeviceD3D();
    core::window::Destroy(hwnd);

    return 0;
}
