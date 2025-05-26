#include "WinScreenGui.h"
#include "LoseScreenGui.h"
#include "CustomImGuiElemets.h"



void BeginImGuiTranspWindow(const char* pWindow_name) noexcept;
void EndImGuiTranspWindow() noexcept;


WinScreenGui::WinScreenGui() noexcept {
}
WinScreenGui::~WinScreenGui() noexcept {
}

void WinScreenGui::Draw() noexcept {
	BeginImGuiTranspWindow("WinScreen");
	ImFont* pFont = ImGui::GetFont();

	pFont->Scale = (float)MainWindow::GetWindowWidth() / 3200.0f;
	ImGui::PushFont(pFont);

	ImVec2 loading_text_size = ImGui::CalcTextSize("You win this game!");
	ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() - loading_text_size.x - 30, MainWindow::GetWindowHeight() - loading_text_size.y - 20));
	ImGui::Text("You win this game!");

	ImGui::PopFont();

	ImGui::SetCursorScreenPos(ImVec2(7, MainWindow::GetWindowHeight() - 7 - MainWindow::GetWindowHeight() / 14.4f));
	if (CustomReadyOrNot::BackButton("Main menu")) {
		DisplayHubManager::ChangeState(DISPLAYHUB_MANAGER_STATE::DHM_MAIN_MENU);
	}

	EndImGuiTranspWindow();
}


LoseScreenGui::LoseScreenGui() noexcept {
}
LoseScreenGui::~LoseScreenGui() noexcept {
}

void LoseScreenGui::Draw() noexcept {
	BeginImGuiTranspWindow("LoseScreen");
	ImFont* pFont = ImGui::GetFont();

	pFont->Scale = (float)MainWindow::GetWindowWidth() / 3200.0f;
	ImGui::PushFont(pFont);

	ImVec2 loading_text_size = ImGui::CalcTextSize("You lose this game!");
	ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() - loading_text_size.x - 30, MainWindow::GetWindowHeight() - loading_text_size.y - 20));
	ImGui::Text("You lose this game!");

	ImGui::PopFont();

	ImGui::SetCursorScreenPos(ImVec2(7, MainWindow::GetWindowHeight() - 7 - MainWindow::GetWindowHeight() / 14.4f));
	if (CustomReadyOrNot::BackButton("Main menu")) {
		DisplayHubManager::ChangeState(DISPLAYHUB_MANAGER_STATE::DHM_MAIN_MENU);
	}

	EndImGuiTranspWindow();
}