#include "Renderer.h"

#include "../../Dependencies/imGUI/imgui.h"
#include "../../Dependencies/imgui/imgui_impl_win32.h"
#include "../../Dependencies/imgui/imgui_impl_dx11.h"

extern bool G_isShould_close_window;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")



IDXGIFactory* Renderer::s_pFactory = nullptr;
ID3D11Device* Renderer::s_pDevice = nullptr;
ID3D11DeviceContext* Renderer::s_pDeviceContext = nullptr;
IDXGISwapChain* Renderer::s_pSwapChain = nullptr;
ID3D11RenderTargetView* Renderer::s_pRenderTargetView = nullptr;
ID3D11DepthStencilView* Renderer::s_pDepthStencilView = nullptr;


void Renderer::Initialize() noexcept {
	if (MainWindow::GetHinstnce() == nullptr) return;

	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&s_pFactory);
	std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter>> adapters;
	Microsoft::WRL::ComPtr<IDXGIAdapter> tmp_pAdapter;
	for (unsigned int i = 0; s_pFactory->EnumAdapters(i, &tmp_pAdapter) == S_OK; i++) {
		adapters.push_back(tmp_pAdapter);
	}

	Microsoft::WRL::ComPtr<IDXGIAdapter> adapter = GetMostPowerfulAdapter(adapters);

	DXGI_SWAP_CHAIN_DESC swapChain_desc{};
	{
		swapChain_desc.BufferDesc.Width = 0;
		swapChain_desc.BufferDesc.Height = 0;
		swapChain_desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChain_desc.BufferDesc.RefreshRate.Numerator = 0;
		swapChain_desc.BufferDesc.RefreshRate.Denominator = 0;
		swapChain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;

		swapChain_desc.SampleDesc.Count = 1u;
		swapChain_desc.SampleDesc.Quality = 0u;

		swapChain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChain_desc.BufferCount = 1;//So we want one back buffer (double buffering one front and one back)
		swapChain_desc.OutputWindow = MainWindow::GetHinstnce();
		swapChain_desc.Windowed = TRUE;
		swapChain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;// DXGI_SWAP_EFFECT_FLIP_DISCARD;//vanila (gives you the best perfomance at many cases
		swapChain_desc.Flags = 0;
	}

	if (D3D11CreateDeviceAndSwapChain(
		adapter.Get(),
		D3D_DRIVER_TYPE_UNKNOWN,
		nullptr,
		NULL,
		nullptr,
		NULL,
		D3D11_SDK_VERSION,
		&swapChain_desc,
		&s_pSwapChain,
		&s_pDevice,
		nullptr,
		&s_pDeviceContext
	) != S_OK) {
		MessageBox(0, L"ERROR", L"With DeviceAndSwapChain creation (Penderer.cpp[52]", 0);
		G_isShould_close_window = true;
		return;
	}

	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	if (s_pSwapChain->GetBuffer(0/*0 - is a back buffer*/, __uuidof(ID3D11Resource), &pBackBuffer) != S_OK) {
		MessageBox(0, L"ERROR", L"With backbuffer  (Renderer.cpp[52]", 0);
		G_isShould_close_window = true;
		return;
	}
	s_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &s_pRenderTargetView);

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthStencilState;
	D3D11_DEPTH_STENCIL_DESC depthStencil_desc{};
	{
		depthStencil_desc.DepthEnable = TRUE;
		depthStencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencil_desc.DepthFunc = D3D11_COMPARISON_LESS;
	}

	if (s_pDevice->CreateDepthStencilState(&depthStencil_desc, &pDepthStencilState) != S_OK) {
		MessageBox(0, L"ERROR", L"With DepthStencil creation (Renderer.cpp[84]", 0);
		G_isShould_close_window = true;
		return;
	}
	s_pDeviceContext->OMSetDepthStencilState(pDepthStencilState.Get(), 1u);


	RECT rect;
	int mainWindow_width = 0;
	int mainWindow_height = 0;
	if (GetWindowRect(MainWindow::GetHinstnce(), &rect))
	{
		mainWindow_width = rect.right - rect.left;
		mainWindow_height = rect.bottom - rect.top;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencilTexture2D;
	D3D11_TEXTURE2D_DESC depthStencil_texture2D_desc{};
	{
		depthStencil_texture2D_desc.Width = mainWindow_width;
		depthStencil_texture2D_desc.Height = mainWindow_height;
		depthStencil_texture2D_desc.MipLevels = 1u;
		depthStencil_texture2D_desc.ArraySize = 1u;
		depthStencil_texture2D_desc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencil_texture2D_desc.SampleDesc.Count = 1u;
		depthStencil_texture2D_desc.SampleDesc.Quality = 0u;
		depthStencil_texture2D_desc.Usage = D3D11_USAGE_DEFAULT;
		depthStencil_texture2D_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	}
	if (s_pDevice->CreateTexture2D(&depthStencil_texture2D_desc, nullptr, &pDepthStencilTexture2D) != S_OK) {
		MessageBox(0, L"ERROR", L"With DepthStencilTexture creation (Renderer.cpp[114]", 0);
		G_isShould_close_window = true;
		return;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilView_desk{};
	{
		depthStencilView_desk.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilView_desk.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilView_desk.Texture2D.MipSlice = 0u;
	}
	s_pDevice->CreateDepthStencilView(pDepthStencilTexture2D.Get(), &depthStencilView_desk, &s_pDepthStencilView);

	SetupViewPort();
	SetRenderViews();

	ImGui_ImplDX11_Init(s_pDevice, s_pDeviceContext);
}
void Renderer::Terminate() noexcept {
	ImGui_ImplDX11_Shutdown();

	if (s_pDepthStencilView != nullptr) {
		s_pDepthStencilView->Release();
		s_pDepthStencilView = nullptr;
	}

	if (s_pRenderTargetView != nullptr) {
		s_pRenderTargetView->Release();
		s_pRenderTargetView = nullptr;
	}
	if (s_pSwapChain) {
		s_pSwapChain->Release();
		s_pSwapChain = nullptr;
	}
	if (s_pDeviceContext) {
		s_pDeviceContext->Release();
		s_pDeviceContext = nullptr;
	}
	if (s_pDevice) {
		s_pDevice->Release();
		s_pDevice = nullptr;
	}
	if (s_pFactory) {
		s_pFactory->Release();
		s_pFactory = nullptr;
	}
}

void Renderer::SetupViewPort() noexcept {
	RECT rect;

	D3D11_VIEWPORT viewPort{};
	{
		viewPort.TopLeftX = 0.f;
		viewPort.TopLeftY = 0.f;
		viewPort.Width = MainWindow::GetWindowWidth();
		viewPort.Height = MainWindow::GetWindowHeight();
		viewPort.MinDepth = 0.f;
		viewPort.MaxDepth = 1.f;
	}
	s_pDeviceContext->RSSetViewports(1, &viewPort);
}
void Renderer::SetRenderViews() noexcept {
	s_pDeviceContext->OMSetRenderTargets(1u, &s_pRenderTargetView, s_pDepthStencilView);
}

void Renderer::ClearFrame() noexcept {
	const float clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	s_pDeviceContext->ClearRenderTargetView(s_pRenderTargetView, clear_color);
	s_pDeviceContext->ClearDepthStencilView(s_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
}
void Renderer::PresentFrame() noexcept {
	s_pSwapChain->Present(1, 0);
}

ID3D11ShaderResourceView* Renderer::LoadImageToGPU(unsigned char* data, unsigned int width, unsigned int height) noexcept {
	D3D11_TEXTURE2D_DESC textureDesc{};
	{
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1u;
		textureDesc.ArraySize = 1u;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1u;
		textureDesc.SampleDesc.Quality = 0u;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0u;
		textureDesc.MiscFlags = 0u;
	}
	D3D11_SUBRESOURCE_DATA subresData{};
	{
		subresData.pSysMem = data;
		subresData.SysMemPitch = width * 4;
	}
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	s_pDevice->CreateTexture2D(&textureDesc, &subresData, &pTexture);

	ID3D11ShaderResourceView* view;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResViewDesc = {};
	{
		shaderResViewDesc.Format = textureDesc.Format;
		shaderResViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResViewDesc.Texture2D.MipLevels = 1;
	}
	s_pDevice->CreateShaderResourceView(pTexture.Get(), &shaderResViewDesc, &view);

	return view;
}

ID3D11Device* Renderer::GetDevice() noexcept {
	return s_pDevice;
}
ID3D11DeviceContext* Renderer::GetDeviceContext() noexcept {
	return s_pDeviceContext;
}
IDXGISwapChain* Renderer::GetSwapChain() noexcept {
	return s_pSwapChain;
}

IDXGIAdapter* Renderer::GetMostPowerfulAdapter(std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter>> adapters) noexcept{
	int bestScore = -1;
	IDXGIAdapter* bestScore_adapter = nullptr;

	DXGI_ADAPTER_DESC desc{};
	for (int i = 0; i < adapters.size(); i++) {
		int score = 0;
		adapters[i]->GetDesc(&desc);
		std::wstring description(desc.Description);

		if (description.find(L"RTX") != std::wstring::npos || description.find(L"GTX") != std::wstring::npos || description.find(L"RX") != std::wstring::npos) {
			score += 5;
		}
		score += (int)(desc.DedicatedVideoMemory / 1000000000);


		if (score > bestScore) {
			bestScore = score;
			bestScore_adapter = adapters[i].Get();
		}
	}

	return bestScore_adapter;
}