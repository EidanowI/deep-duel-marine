#pragma once
#include "Graphics/MainWindow/MainWindow.h"
#include "Graphics/Renderer/Renderer.h"
#include "Graphics/ImGuiRender/ImGuiRender.h"
#include "SteamNetworking/SteamNetworking.h"
#include "Graphics/GUI/DisplayHubManager.h"
#include "Graphics/Shader/Shader.h"

bool G_isShould_close_window = false;



class DeepDuelMarine {
public:
	DeepDuelMarine() noexcept = default;

	void Run() noexcept {
		ImGuiRenderer::Initialize();
		MainWindow::Initialize();
		Renderer::Initialize();
		ShaderManager::Initialize();

		SteamNetworkingManager::TryToConnect();
		DisplayHubManager::Initialize();

		MainLoop();

		DisplayHubManager::Terminate();
		SteamNetworkingManager::Terminate();

		ShaderManager::Terminate();
		Renderer::Terminate();
		MainWindow::Terminate();
		ImGuiRenderer::Terminate();
	}

private:
	int MainLoop() noexcept {
		MainWindow::SetWindowVisibility(true);
		while (!G_isShould_close_window) {
			const std::optional<int> proc = MainWindow::ProcessMsg();
			if (proc.has_value()) return proc.value();

			SteamNetworkingManager::Update();

			Renderer::ClearFrame();
			InvalidateRect(MainWindow::GetHinstnce(), 0, 1);

			ImGuiRenderer::StartFrame();
			
			DisplayHubManager::Display();

			ImGuiRenderer::EndFrame();

			Renderer::PresentFrame();
		}
	}
};