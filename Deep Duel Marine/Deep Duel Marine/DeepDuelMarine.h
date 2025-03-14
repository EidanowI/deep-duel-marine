#pragma once
#include "MainWindow/MainWindow.h"

bool G_isShould_close_window = false;


class DeepDuelMarine {
public:
	DeepDuelMarine() noexcept = default;

	void Run() noexcept {
		MainWindow::Initialize();

		MainLoop();

		MainWindow::Terminate();
	}

private:
	int MainLoop() noexcept {

		while (!G_isShould_close_window) {
			const std::optional<int> proc = MainWindow::ProcessMsg();
			if (proc.has_value()) return proc.value();

			InvalidateRect(MainWindow::GetHinstnce(), 0, 1);
			Sleep(16);
		}
	}
};