#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <vector>
#include <string>

#include "../MainWindow/MainWindow.h"



class Renderer {
public:
	static void Initialize() noexcept;
	static void Terminate() noexcept;

	static void SetupViewPort() noexcept;
	static void SetRenderViews() noexcept;
	static void ClearFrame() noexcept;
	static void PresentFrame() noexcept;

	static ID3D11ShaderResourceView* LoadImageToGPU(unsigned char* data, unsigned int width, unsigned int height) noexcept;

private:
	static IDXGIAdapter* GetMostPowerfulAdapter(std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter>> adapters) noexcept;

private:
	static IDXGIFactory* s_pFactory;
	static ID3D11Device* s_pDevice;
	static ID3D11DeviceContext* s_pDeviceContext;
	static IDXGISwapChain* s_pSwapChain;
	static ID3D11RenderTargetView* s_pRenderTargetView;
	static ID3D11DepthStencilView* s_pDepthStencilView;
};