#pragma once
#include "Graphics/MainWindow/MainWindow.h"
#include "Graphics/Renderer/Renderer.h"
#include "Graphics/ImGuiRender/ImGuiRender.h"
#include "Graphics/GUI/GUI.h"
#include "SteamNetworking/SteamNetworking.h"

bool G_isShould_close_window = false;



class DisplayHub {
public:
	DisplayHub() noexcept {
		m_pGui = new GUI();
		///m_pScene ...
	}
	~DisplayHub() noexcept {
		delete m_pGui;
		///delete m_pScene;
	}

	void Display() noexcept {
		m_pGui->Draw();
		///m_pScene->Render();
	}

private:
	GUI* m_pGui;
	///TODO scene class
};


class DeepDuelMarine {
public:
	DeepDuelMarine() noexcept = default;

	void Run() noexcept {
		ImGuiRenderer::Initialize();
		MainWindow::Initialize();
		Renderer::Initialize();
		SteamNetworkingManager::TryToConnect();

		MainLoop();

		SteamNetworkingManager::Terminate();
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
			
			m_displayHub.Display();

			ImGuiRenderer::EndFrame();

			Renderer::PresentFrame();
		}
	}

private:
	DisplayHub m_displayHub;
};