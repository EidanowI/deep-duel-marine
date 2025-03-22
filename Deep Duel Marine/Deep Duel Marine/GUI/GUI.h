#pragma once
#include "../Dependencies/imGUI/imgui.h"

#include "../MainWindow/MainWindow.h"



extern bool G_isShould_close_window;


enum GUI_STATE {
	MAIN_MENU,
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

private:
	void DrawMainMenu() noexcept {
		ImFont* pFont = ImGui::GetFont();
		pFont->Scale = 2.0;

		ImGui::PushFont(pFont);
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::SetNextWindowPos(ImVec2(MainWindow::GetWindowWidth() / 17, MainWindow::GetWindowHeight() / 2.8));
		ImGui::Begin("Main menu", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowBorderSize = 0.0f;
		style.ItemSpacing = ImVec2(0, MainWindow::GetWindowHeight() / 40);
		if (ImGui::Button("Play", ImVec2(MainWindow::GetWindowWidth() /4.2, MainWindow::GetWindowHeight() /20))) {
			m_gui_state = LOBBY_BROWSER;
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

	void DrawLobbyBrowser() noexcept {
#define LOBBY_BROWSER_WIDTH 1920 / 2.3
#define LOBBY_BROWSER_HEIGHT 1080 * 0.75

		ImGui::SetNextWindowPos(ImVec2((MainWindow::GetWindowWidth() - LOBBY_BROWSER_WIDTH) * 0.3, (MainWindow::GetWindowHeight() - LOBBY_BROWSER_HEIGHT) / 2));
		ImGui::Begin("Lobby Browser", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
		ImGuiStyle& style = ImGui::GetStyle();
		//style.WindowBorderSize = 0.0f;
		style.ItemSpacing = ImVec2(25, MainWindow::GetWindowHeight() / 40);
		static char name_filter[128] = "";
		ImGui::InputTextWithHint("##", "Lobby name filter", name_filter, IM_ARRAYSIZE(name_filter));

		ImGui::SameLine();

		if (ImGui::Button("Refresh", ImVec2(120, 35))) {
			//
		}

		static int item_current_idx = 0;
		if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (int n = 0; n < 10; n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(("Lobby #" + std::to_string(n)).c_str(), is_selected))
					item_current_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
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