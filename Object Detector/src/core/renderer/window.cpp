#include "pch.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace core {
    namespace window {



        cv::Mat frame;
        std::atomic<bool> isRunning(true);  // Flag to control the capture thread

        void CaptureWebcam()
        {
            cv::VideoCapture cap(0); // Open the default webcam

            if (!cap.isOpened()) {
                std::cerr << "Error: Could not open the webcam." << std::endl;
                isRunning = false; // Stop if webcam can't open
                return;
            }

            while (isRunning) {
                cv::Mat tempFrame;
                cap >> tempFrame;  // Capture a frame

                if (tempFrame.empty()) {
                    std::cerr << "Error: Blank frame grabbed." << std::endl;
                    break;
                }

                // Copy the frame to the shared variable
                frame = tempFrame.clone();
            }

            cap.release();  // Release the webcam when done
        }


        ImVec2 windowPos{0, 0};
        ImVec2 windowSize{1920, 1080};

        LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
                return true;
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }


        HWND Create(int x, int y, int width, int height)
        {
            // Register the window class with a title bar, borders, and standard window controls
            WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WindowProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("Object Detector"), NULL };
            ::RegisterClassEx(&wc);

            // Create a standard window with title bar, minimize, maximize, and close buttons
            HWND hwnd = ::CreateWindowEx(
                0,                  // No extended styles needed for a standard window
                wc.lpszClassName,
                _T("Object Detector"), // Window title
                WS_OVERLAPPEDWINDOW,   // Standard window style with title bar and borders
                x, y, width, height,
                NULL, NULL,
                wc.hInstance,
                NULL
            );

            if (hwnd == NULL)
            {
                LOG_TAG("ERROR") << "Failed to create window!";
                return NULL;
            }

            return hwnd;
        }


        void Show(HWND hwnd)
        {
            ::ShowWindow(hwnd, SW_SHOWDEFAULT);
            ::UpdateWindow(hwnd);
        }

        void Destroy(HWND hwnd)
        {
            ::DestroyWindow(hwnd);
            ::UnregisterClass(_T("Object Detector"), GetModuleHandle(NULL));
        }

       

        void MessageLoop(HWND hwnd)
        {
            ImVec4 clear_color = ImVec4(.0f, .0f, .0f, .0f);

            MSG msg;
            ZeroMemory(&msg, sizeof(msg));
            // Start the capture thread
            std::thread captureThread(CaptureWebcam);

            ID3D11ShaderResourceView* texture = nullptr;

            while (msg.message != WM_QUIT)
            {

                if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
                {
                    ::TranslateMessage(&msg);
                    ::DispatchMessage(&msg);
                    continue;
                }

                // Convert frame to texture if a new frame is available
                if (!frame.empty()) {
                    if (texture) texture->Release();
                    texture = renderer::MatToTexture(frame);
                }

                // Start the ImGui frame
                ImGui_ImplDX11_NewFrame();
                ImGui_ImplWin32_NewFrame();
                ImGui::NewFrame();
                // ImGui window for displaying the webcam feed
                ImGui::Begin("Webcam Feed");
                if (texture)
                    ImGui::Image((void*)texture, ImVec2(frame.cols, frame.rows));
                ImGui::End();
                // Rendering
                ImGui::Render();
                renderer::g_pd3dDeviceContext->OMSetRenderTargets(1, &renderer::g_mainRenderTargetView, NULL);
                renderer::g_pd3dDeviceContext->ClearRenderTargetView(renderer::g_mainRenderTargetView, reinterpret_cast<float*>(&clear_color));
                ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
                renderer::g_pSwapChain->Present(1, 0); // Present with vsync
            }

        }
       
    }
}