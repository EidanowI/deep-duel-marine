#pragma once
#include "MainWindow/MainWindow.h"
#include "Renderer/Renderer.h"
#include "ImGuiRender/ImGuiRender.h"

bool G_isShould_close_window = false;


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

			ImGuiRenderer::NewFrame();
			
			Sleep(16);
			Renderer::PresentFrame();
		}
	}
};