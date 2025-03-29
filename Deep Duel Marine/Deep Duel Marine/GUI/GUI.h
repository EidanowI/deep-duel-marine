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

extern bool G_isShould_close_window;
Lobby G_lobby;


enum GUI_STATE {
	MAIN_MENU,

	CREATE_LOBBY,

	LOBBY_BROWSER,

	IN_LOBBY,
	SETTINGS,
	QUIT_DIALOG
};
class GUI {
public:
	void Draw() noexcept {
		switch (m_gui_state)
		{
		case MAIN_MENU:
			DrawMainMenu();
			break;
		case CREATE_LOBBY:
			DrawCreateLobby();
			break;
		case LOBBY_BROWSER:
			DrawLobbyBrowser();
			break;
		case SETTINGS:
			DrawSetting();
			break;
		case QUIT_DIALOG:
			DrawQuitDialog();
			break;
		default:
			break;
		}
	}

	///void UpdateAvatars() noexcept;

private:
	void DrawMainMenu() noexcept {
		static auto img = SteamNetworkingManager::LoadSelfAvatar();
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::Begin("Profile info", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowBorderSize = 0.0f;
		style.ItemSpacing = ImVec2(30, 0);
		ImGui::Image(img, ImVec2(128, 128));
		ImGui::SameLine();
		ImFont* pFont = ImGui::GetFont();
		pFont->Scale = 4.0;
		ImGui::PushFont(pFont);
		ImGui::Text(SteamNetworkingManager::GetSelfUserNickName());
		ImGui::PopFont();
		ImGui::End();


		//ImFont* pFont = ImGui::GetFont();
		pFont->Scale = 2.0;

		ImGui::PushFont(pFont);
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::SetNextWindowPos(ImVec2(MainWindow::GetWindowWidth() / 17, MainWindow::GetWindowHeight() / 2.8));
		ImGui::Begin("Main menu", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
		//ImGuiStyle& style = ImGui::GetStyle();
		style.WindowBorderSize = 0.0f;
		style.ItemSpacing = ImVec2(0, MainWindow::GetWindowHeight() / 40);
		if (ImGui::Button("Create match", ImVec2(MainWindow::GetWindowWidth() /4.2, MainWindow::GetWindowHeight() /20))) {
			if (SteamNetworkingManager::IsSteamConnected()) {
				m_gui_state = CREATE_LOBBY;
				SteamNetworkingManager::ClearLobbyStructData();
			}
		}

		if (ImGui::Button("Join a match", ImVec2(MainWindow::GetWindowWidth() / 4.2, MainWindow::GetWindowHeight() / 20))) {
			if (SteamNetworkingManager::IsSteamConnected()) {
				SteamNetworkingManager::ClearLobbyStructData();
				SteamNetworkingManager::GetLobbyBrowser()->Refresh();
				m_gui_state = LOBBY_BROWSER;
			}
		}

		if (ImGui::Button("Settings", ImVec2(MainWindow::GetWindowWidth() / 4.2, MainWindow::GetWindowHeight() / 20))) {
			m_gui_state = SETTINGS;
		}

		if (ImGui::Button("Quit", ImVec2(MainWindow::GetWindowWidth() / 4.2, MainWindow::GetWindowHeight() / 20))) {
			m_gui_state = QUIT_DIALOG;
		}
		ImGui::End();
		ImGui::PopFont();
	}

	void DrawCreateLobby() noexcept {
#define CREATE_LOBBY_WIDTH 1920 / 4
#define CREATE_LOBBY_HEIGHT 1080 / 4

		ImFont* pFont = ImGui::GetFont();
		pFont->Scale = 2.0;
		ImGui::PushFont(pFont);
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::SetNextWindowPos(ImVec2((MainWindow::GetWindowWidth() - CREATE_LOBBY_WIDTH) / 2, (MainWindow::GetWindowHeight() - CREATE_LOBBY_HEIGHT) / 2));
		ImGui::Begin("Create lobby", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowBorderSize = 0.0f;
		style.ItemSpacing = ImVec2(20, 20);
		ImGui::Text("Lobby name:");

		ImGui::InputText("##", SteamNetworkingManager::GetLobbyStruct()->m_name, 63);

		if (TestButton("Public", ImVec2((CREATE_LOBBY_WIDTH - 20) / 2, 80), ImGuiButtonFlags_None, SteamNetworkingManager::GetLobbyStruct()->m_isPublic)) {
			SteamNetworkingManager::GetLobbyStruct()->m_isPublic = true;
		}
		ImGui::SameLine();
		if (TestButton("Friends-only", ImVec2((CREATE_LOBBY_WIDTH - 20) / 2, 80), ImGuiButtonFlags_None, !SteamNetworkingManager::GetLobbyStruct()->m_isPublic)) {
			SteamNetworkingManager::GetLobbyStruct()->m_isPublic = false;
		}

		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + ((CREATE_LOBBY_WIDTH - 20) / 4), ImGui::GetCursorPos().y));
		if (ImGui::Button("Confirm", ImVec2((CREATE_LOBBY_WIDTH - 20) / 2, 40))) {
			SteamNetworkingManager::CreateLobby();
		}
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + ((CREATE_LOBBY_WIDTH - 20) / 4), ImGui::GetCursorPos().y));
		if (ImGui::Button("Back", ImVec2((CREATE_LOBBY_WIDTH - 20) / 2, 40))) {
			m_gui_state = MAIN_MENU;
		}

		ImGui::End();
		ImGui::PopFont();
	}

	void DrawLobbyBrowser() noexcept {
#define LOBBY_BROWSER_WIDTH 1920 / 2.3
#define LOBBY_BROWSER_HEIGHT 1080 * 0.75

		ImGui::SetNextWindowPos(ImVec2((MainWindow::GetWindowWidth() - LOBBY_BROWSER_WIDTH) * 0.3, (MainWindow::GetWindowHeight() - LOBBY_BROWSER_HEIGHT) / 2));
		ImGui::Begin("Lobby Browser", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
		ImGuiStyle& style = ImGui::GetStyle();
		//style.WindowBorderSize = 0.0f;
		style.ItemSpacing = ImVec2(25, MainWindow::GetWindowHeight() / 40);
		ImGui::InputTextWithHint("##", "Lobby name filter", SteamNetworkingManager::GetLobbyBrowser()->GetFilterName(), 64, ImGuiInputTextFlags_CallbackEdit, InputFilterTextCalback);

		ImGui::SameLine();

		if (ImGui::Button("Refresh", ImVec2(120, 35))) {
			SteamNetworkingManager::GetLobbyBrowser()->Refresh();
		}

		static int item_current_idx = 0;
		if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (int i = 0; i < SteamNetworkingManager::GetLobbyBrowser()->GetlobbyCount(); i++)
			{
				
				const bool is_selected = (item_current_idx == i);

				std::string a = SteamNetworkingManager::GetLobbyBrowser()->GetLobbyByIndex(i).m_name;
				if (ImGui::Selectable(SteamNetworkingManager::GetLobbyBrowser()->GetLobbyByIndex(i).m_name, is_selected))
				{
					item_current_idx = i;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
					
			}
			ImGui::EndListBox();
		}

		if (ImGui::Button("Back to main menu", ImVec2(ImGui::CalcTextSize("Back to main menu ").x, 35))) {
			m_gui_state = MAIN_MENU;
		}

	
		ImGui::End();

	}

	void DrawSetting() noexcept {
#define SETTINGS_WIDTH 1920 / 3
#define SETTINGS_HEIGHT 1080 / 3 
		ImFont* pFont = ImGui::GetFont();
		pFont->Scale = 2.0;

		ImGui::PushFont(pFont);
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::SetNextWindowPos(ImVec2((MainWindow::GetWindowWidth() - SETTINGS_WIDTH) / 2, (MainWindow::GetWindowHeight() - SETTINGS_HEIGHT) / 2));
		ImGui::Begin("Settings", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowBorderSize = 0.0f;
		style.ItemSpacing = ImVec2(10, MainWindow::GetWindowHeight() / 40);

		static float volume = 0;
		ImGui::SliderFloat("##", &volume, 0, 1);
		ImGui::SameLine();
		ImGui::Text("Volume");

		if (ImGui::Button("Back to main menu", ImVec2(ImGui::CalcTextSize("Back to main menu ").x, 35))) {
			m_gui_state = MAIN_MENU;
		}

		ImGui::End();
		ImGui::PopFont();
	}

	void DrawQuitDialog() noexcept {
		ImFont* pFont = ImGui::GetFont();
		pFont->Scale = 2.0;

		ImGui::PushFont(pFont);
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::SetNextWindowPos(ImVec2((MainWindow::GetWindowWidth() - (MainWindow::GetWindowWidth() / 4.2 - 20)) / 2, MainWindow::GetWindowHeight() / 2.2));
		ImGui::Begin("Quit dialog", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowBorderSize = 0.0f;
		style.ItemSpacing = ImVec2(20, MainWindow::GetWindowHeight() / 20);

		ImGui::Text("   You shure you want to quit?");

		if (ImGui::Button("Cancel", ImVec2((MainWindow::GetWindowWidth() / 4.2 - 20) / 2, MainWindow::GetWindowHeight() / 20))) {
			m_gui_state = MAIN_MENU;
		}

		ImGui::SameLine();

		if (ImGui::Button("Quit", ImVec2((MainWindow::GetWindowWidth() / 4.2 - 20) / 2, MainWindow::GetWindowHeight() / 20))) {
			G_isShould_close_window = true;
		}

		ImGui::End();
		ImGui::PopFont();
	}

	GUI_STATE m_gui_state = MAIN_MENU;
};


ImVec2  operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
ImVec2  operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
bool TestButton(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags, bool isActive)
{
	using namespace ImGui;
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ItemSize(size, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

	// Render

	const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : isActive ? ImGuiCol_ButtonActive : ImGuiCol_Button);
	RenderNavHighlight(bb, id);
	RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);

	if (g.LogEnabled)
		LogSetNextTextDecoration("[", "]");
	RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

	// Automatically close popups
	//if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
	//    CloseCurrentPopup();

	IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
	return pressed;
}

int InputFilterTextCalback(ImGuiInputTextCallbackData* data) {
	SteamNetworkingManager::GetLobbyBrowser()->Refresh();
	return 0;
}