#pragma once
#include "MainWindow/MainWindow.h"
#include "Renderer/Renderer.h"
#include "ImGuiRender/ImGuiRender.h"
#include "GUI/GUI.h"

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

		MainLoop();

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