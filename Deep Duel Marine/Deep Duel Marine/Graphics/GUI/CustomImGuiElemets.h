#pragma once
#pragma once
#include <string>
#include <algorithm>

#include "../../Dependencies/imGUI/imgui.h"
#include "../../Dependencies/imGUI/imgui_internal.h"

#include "../MainWindow/MainWindow.h"
#include "../Textures/Texture.h"



ImVec2  operator+(const ImVec2& lhs, const ImVec2& rhs);
ImVec2  operator-(const ImVec2& lhs, const ImVec2& rhs);

namespace CustomMainMenu {
	bool MainMenuButton(const char* label, ImGuiButtonFlags flags = 0);
	bool MainMenuGitHubButton(ImGuiButtonFlags flags = 0);
	bool MainMenuTelegramButton(ImGuiButtonFlags flags = 0);
}

bool ConfBackButton(const char* label, const ImColor color_hovered, const ImColor color_active);

namespace CustomReadyOrNot {
	bool BackButton(const char* label, ImGuiButtonFlags flags = 0);
	bool ReadyButton(const char* label, bool isReady);
}

namespace CustomLobbyBrowser {
	bool RefreshButton(int side, ImVec2 img_pos, ImGuiButtonFlags flags = 0);
}