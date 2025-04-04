#pragma once
#include "../../Dependencies/imGUI/imgui.h"

#include "../MainWindow/MainWindow.h"



bool CustomButton(const char* label) noexcept {
	ImGui::PushID(label);

	ImVec2 button_size = ImVec2(MainWindow::GetWindowWidth() / 14, MainWindow::GetWindowHeight() / 12);

	ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
	bool is_clicked = ImGui::InvisibleButton(label, button_size);


	ImGui::PopID();

	return 1;
}