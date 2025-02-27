#include "pch.h"

namespace core {
    namespace renderer {

        ID3D11Device* g_pd3dDevice = NULL;
        ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
        IDXGISwapChain* g_pSwapChain = NULL;
        ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

        ID3D11ShaderResourceView* MatToTexture(const cv::Mat& mat)
        {
            cv::Mat matRGBA;
            cv::cvtColor(mat, matRGBA, cv::COLOR_BGR2RGBA);

            D3D11_TEXTURE2D_DESC desc;
            ZeroMemory(&desc, sizeof(desc));
            desc.Width = mat.cols;
            desc.Height = mat.rows;
            desc.MipLevels = 1;
            desc.ArraySize = 1;
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc.SampleDesc.Count = 1;
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

            ID3D11Texture2D* pTexture = nullptr;
            D3D11_SUBRESOURCE_DATA initData;
            initData.pSysMem = matRGBA.data;
            initData.SysMemPitch = matRGBA.step;
            initData.SysMemSlicePitch = 0;
            g_pd3dDevice->CreateTexture2D(&desc, nullptr, &pTexture);

            D3D11_MAPPED_SUBRESOURCE mappedResource;
            if (SUCCEEDED(g_pd3dDeviceContext->Map(pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
            {
                memcpy(mappedResource.pData, matRGBA.data, mat.total() * mat.elemSize());
                g_pd3dDeviceContext->Unmap(pTexture, 0);
            }

            ID3D11ShaderResourceView* pShaderResourceView = nullptr;
            g_pd3dDevice->CreateShaderResourceView(pTexture, nullptr, &pShaderResourceView);
            pTexture->Release();  // Release the texture as we only need the shader view

            return pShaderResourceView;
        }


        bool CreateDeviceD3D(HWND hWnd)
        {

            // Setup swap chain
            DXGI_SWAP_CHAIN_DESC sd;
            ZeroMemory(&sd, sizeof(sd));
            sd.BufferCount = 2;
            sd.BufferDesc.Width = 0;
            sd.BufferDesc.Height = 0;
            sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            sd.BufferDesc.RefreshRate.Numerator = 60;
            sd.BufferDesc.RefreshRate.Denominator = 1;
            sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
            sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            sd.OutputWindow = hWnd;
            sd.SampleDesc.Count = 1;
            sd.SampleDesc.Quality = 0;
            sd.Windowed = TRUE;
            sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

            UINT createDeviceFlags = 0;
            D3D_FEATURE_LEVEL featureLevel;
            const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

            if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
            {
                LOG_TAG("ERROR") << "Failed to create D3D device and swap chain.";

                return false;
            }

            CreateRenderTarget();
            return true;
        }

        void CleanupDeviceD3D()
        {

            CleanupRenderTarget();
            if (g_pSwapChain)
            {
                g_pSwapChain->Release();
                g_pSwapChain = NULL;
            }
            if (g_pd3dDeviceContext)
            {
                g_pd3dDeviceContext->Release();
                g_pd3dDeviceContext = NULL;
            }
            if (g_pd3dDevice)
            {
                g_pd3dDevice->Release();
                g_pd3dDevice = NULL;
            }
        }

        void CreateRenderTarget()
        {

            ID3D11Texture2D* pBackBuffer;
            g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
            g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
            pBackBuffer->Release();

        }

        void CleanupRenderTarget()
        {

            if (g_mainRenderTargetView)
            {
                g_mainRenderTargetView->Release();
                g_mainRenderTargetView = NULL;
            }

        }
    }
}