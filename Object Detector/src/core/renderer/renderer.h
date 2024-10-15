#pragma once
namespace core {
    namespace renderer {

        extern ID3D11Device* g_pd3dDevice;
        extern ID3D11DeviceContext* g_pd3dDeviceContext;
        extern IDXGISwapChain* g_pSwapChain;
        extern ID3D11RenderTargetView* g_mainRenderTargetView;

        bool CreateDeviceD3D(HWND hWnd);
        void CleanupDeviceD3D();
        void CreateRenderTarget();
        void CleanupRenderTarget();
        ID3D11ShaderResourceView* MatToTexture(const cv::Mat& mat);

    }
}