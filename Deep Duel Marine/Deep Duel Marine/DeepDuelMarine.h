#pragma once
#include "Graphics/MainWindow/MainWindow.h"
#include "Graphics/Renderer/Renderer.h"
#include "Graphics/ImGuiRender/ImGuiRender.h"
#include "Graphics/GUI/DisplayHubManager.h"
#include "Graphics/Shader/Shader.h"
#include "Graphics/Utils/Timer.h"

#include "SteamNetworking/include/DDMSteamWorksLib.h"

bool G_isShould_close_window = false;



class DeepDuelMarine {
public:
	DeepDuelMarine() noexcept = default;

	void Run() noexcept {
		ImGuiRenderer::Initialize();
		MainWindow::Initialize();
		Renderer::Initialize();
		ShaderManager::Initialize();

		DDMSteamWorksLib::SWNetworkingManager::TryToConnect(Renderer::GetDevice());
		DisplayHubManager::Initialize();

		MainLoop();

		DisplayHubManager::Terminate();
		DDMSteamWorksLib::SWNetworkingManager::Terminate();

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

			DDMSteamWorksLib::SWNetworkingManager::Update();

			Renderer::ClearFrame();
			InvalidateRect(MainWindow::GetHinstnce(), 0, 1);

			ImGuiRenderer::StartFrame();
			
			DisplayHubManager::Display();
			Timer::Update();

			ImGuiRenderer::EndFrame();

			Renderer::PresentFrame();

		}
	}
};