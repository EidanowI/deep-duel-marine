#pragma once
#include <string>
#include <algorithm>

#include "../../Dependencies/imGUI/imgui.h"
#include "../../Dependencies/imGUI/imgui_internal.h"
//#include "../Dependencies/imGUI/"

#include "../MainWindow/MainWindow.h"
#include "../../SteamNetworking/SteamNetworking.h"
#include "IGUI.h"
#include "CustomImGuiElemets.h"



bool TestButton(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags, bool isActive);
int InputFilterTextCalback(ImGuiInputTextCallbackData* data);


enum GUI_STATE {
	MAIN_MENU,
	CREATE_LOBBY,
	LOBBY_BROWSER,
	LOBBY_LOADING,
	IN_LOBBY,
	STARTING_SERVER,
	SETTINGS,
	QUIT_DIALOG
};

class MainMenuGui : public IGui {
public:
	MainMenuGui() noexcept;
	~MainMenuGui() noexcept;

	void Draw() noexcept override;

	void SetGuiState(GUI_STATE state) noexcept;

private:
	void DrawMainMenu() noexcept;

	void DrawCreateLobby() noexcept;

	void DrawLobbyBrowser() noexcept;

	void DrawLobbyLoading() noexcept;

	void DrawReadyOrNotMenu() noexcept;

	void DrawStartingServer() noexcept;

	void DrawSetting() noexcept;

	void DrawQuitDialog() noexcept;

private:
	GUI_STATE m_gui_state = MAIN_MENU;
};