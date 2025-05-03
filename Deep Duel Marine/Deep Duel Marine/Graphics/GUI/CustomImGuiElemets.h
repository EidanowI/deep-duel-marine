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


bool MainMenuButton(const char* label, ImGuiButtonFlags flags = 0);
bool MainMenuGitHubButton(ImGuiButtonFlags flags = 0);
bool MainMenuTelegramButton(ImGuiButtonFlags flags = 0);