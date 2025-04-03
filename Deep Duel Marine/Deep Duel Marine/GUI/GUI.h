#pragma once
#include <string>
#include <algorithm>

#include "../Dependencies/imGUI/imgui.h"
#include "../Dependencies/imGUI/imgui_internal.h"
//#include "../Dependencies/imGUI/"

#include "../MainWindow/MainWindow.h"
#include "../SteamNetworking/SteamNetworking.h"



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
class GUI {
public:
	GUI() noexcept;

	void Draw() noexcept;

	void SetGuiState(GUI_STATE state) noexcept;
	///void UpdateAvatars() noexcept;

private:
	void DrawMainMenu() noexcept;

	void DrawCreateLobby() noexcept;

	void DrawLobbyBrowser() noexcept;

	void DrawLobbyLoading() noexcept;

	void DrawReadyOrNotMenu() noexcept;

	void DrawSetting() noexcept;

	void DrawQuitDialog() noexcept;
	
	void UpdateAponentAvatar() noexcept;

private:

	GUI_STATE m_gui_state = MAIN_MENU;

	char* m_pSelf_avatar;
	char* m_pAponent_avatar;
};