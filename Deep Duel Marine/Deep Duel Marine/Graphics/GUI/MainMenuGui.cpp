#include "MainMenuGui.h"



extern bool G_isShould_close_window;
Lobby G_lobby;


MainMenuGui::MainMenuGui() noexcept {
	m_pAponent_avatar = (char*)SteamNetworkingManager::LoadAponentAvatar();
}
MainMenuGui::~MainMenuGui() noexcept {

}

void MainMenuGui::Draw() noexcept {
	if (m_pSelf_avatar == 0) {
		m_pSelf_avatar = (char*)SteamNetworkingManager::LoadSelfAvatar();
	}

	static int tick_counter = 0;
	if (tick_counter > 60) {
		tick_counter = 0;

		UpdateAponentAvatar();
	}
	tick_counter++;

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
	case LOBBY_LOADING:
		DrawLobbyLoading();
		{
			if (SteamNetworkingManager::GetLobbyStruct()->m_id)
			{
				m_gui_state = IN_LOBBY;
			}
		}
		break;
	case IN_LOBBY:
		DrawReadyOrNotMenu();
		break;
	case STARTING_SERVER:
		DrawStartingServer();
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
void MainMenuGui::SetGuiState(GUI_STATE state) noexcept {
	m_gui_state = state;
}


void BeginImGuiTranspWindow(const char* pWindow_name) noexcept {
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin(pWindow_name, 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(30, 0);
}
void EndImGuiTranspWindow() noexcept {
	ImGui::End();
}

void MainMenuGui::DrawMainMenu() noexcept {
	BeginImGuiTranspWindow("MainMenu");
	ImFont* pFont = ImGui::GetFont();

	ImGui::SetCursorPos(ImVec2(7, 7));
	ImGui::Image(m_pSelf_avatar, ImVec2((int)(MainWindow::GetWindowWidth() / 14.77f), (int)(MainWindow::GetWindowHeight() / 8.31f)));

	pFont->Scale = (float)MainWindow::GetWindowWidth() / 1920.0f;
	ImGui::PushFont(pFont);
	if (SteamNetworkingManager::GetSelfUserNickName()) {
		ImGui::SetCursorPos(ImVec2(7 + (int)(MainWindow::GetWindowWidth() / 14.77f) + 15, 7 + (int)(MainWindow::GetWindowHeight() / 8.31f / 2.0f) - 40 * (float)MainWindow::GetWindowWidth() / 1920.0f));
		ImGui::Text(SteamNetworkingManager::GetSelfUserNickName());
	}
	ImGui::PopFont();
	
	pFont->Scale = (float)MainWindow::GetWindowWidth() / 3200.0f;
	ImGui::PushFont(pFont);

	int button_size_and_spacing = MainWindow::GetWindowHeight() / 20 + MainWindow::GetWindowHeight() / 45;

	ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() / 17, MainWindow::GetWindowHeight() / 2.8 + 0 * button_size_and_spacing));
	if (CustomMainMenu::MainMenuButton("Create a match")){
		if (SteamNetworkingManager::IsSteamConnected()) {
			m_gui_state = CREATE_LOBBY;
			SteamNetworkingManager::ClearLobbyStructData();
		}
	}

	ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() / 17, MainWindow::GetWindowHeight() / 2.8 + 1 * button_size_and_spacing));
	if (CustomMainMenu::MainMenuButton("Join a match")) {
		if (SteamNetworkingManager::IsSteamConnected()) {
			SteamNetworkingManager::ClearLobbyStructData();
			SteamNetworkingManager::GetLobbyBrowser()->Refresh();
			m_gui_state = LOBBY_BROWSER;
		}
	}

	ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() / 17, MainWindow::GetWindowHeight() / 2.8 + 2 * button_size_and_spacing));
	if (CustomMainMenu::MainMenuButton("Settings")) {
		m_gui_state = SETTINGS;
	}

	ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() / 17, MainWindow::GetWindowHeight() / 2.8 + 3 * button_size_and_spacing));
	if (CustomMainMenu::MainMenuButton("Quit")) {
		m_gui_state = QUIT_DIALOG;
	}

	ImGui::PopFont();

	
	ImGui::SetCursorPos(ImVec2(7, MainWindow::GetWindowHeight() - MainWindow::GetWindowHeight() / 16.62f - 7));
	if (CustomMainMenu::MainMenuGitHubButton()) {
		ShellExecute(0, 0, L"https://github.com/EidanowI/deep-duel-marine", 0, 0, SW_SHOW);
	}

	ImGui::SetCursorPos(ImVec2(16 + MainWindow::GetWindowHeight() / 16.62f * 2.7f, MainWindow::GetWindowHeight() - MainWindow::GetWindowHeight() / 16.62f - 7));
	if (CustomMainMenu::MainMenuTelegramButton()) {
		ShellExecute(0, 0, L"https://t.me/Eidanowi", 0, 0, SW_SHOW);
	}

	EndImGuiTranspWindow();
}
void MainMenuGui::DrawCreateLobby() noexcept {
	BeginImGuiTranspWindow("CreateLobby");
	ImFont* pFont = ImGui::GetFont();

	pFont->Scale = (float)MainWindow::GetWindowWidth() / 3200.0f;
	ImGui::PushFont(pFont);
	ImVec2 lobby_name_text_size = ImGui::CalcTextSize("Lobby name");
	ImGui::SetCursorPos(ImVec2((MainWindow::GetWindowWidth() / 2) - lobby_name_text_size.x / 2,
		MainWindow::GetWindowHeight() / 3));
	ImGui::Text("Lobby name:");

	ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() * 5 / 14, ImGui::GetCursorPosY() + 7));
	ImGui::InputTextEx("##", 0, SteamNetworkingManager::GetLobbyStruct()->m_name, 63, ImVec2(MainWindow::GetWindowWidth() / 3.5, lobby_name_text_size.y), 0);
	

	auto pos_y_forconf = ImGui::GetCursorPosY() + 20;
	ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() * 5 / 14, pos_y_forconf));
	if (ConfBackButton("Back", ImColor(160, 115, 115, 255 - 115), ImColor(160, 100, 100, 255 - 100))) {
		m_gui_state = MAIN_MENU;
	}

	ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() * 5 / 14 + MainWindow::GetWindowWidth() / 7, pos_y_forconf));
	if (ConfBackButton("Confirm", ImColor(115, 160, 115, 255 - 115), ImColor(100, 160, 100, 255 - 100))) {
		m_gui_state = LOBBY_LOADING;
		SteamNetworkingManager::GetLobbyStruct()->m_isPublic = true;
		SteamNetworkingManager::CreateLobby();
	}

	ImGui::PopFont();
	EndImGuiTranspWindow();
}
void MainMenuGui::DrawLobbyBrowser() noexcept {
	BeginImGuiTranspWindow("Lobby browser");
	ImFont* pFont = ImGui::GetFont();

	pFont->Scale = (float)MainWindow::GetWindowWidth() / 3200.0f;
	ImGui::PushFont(pFont);

	ImVec2 filter_text_size = ImGui::CalcTextSize("Lobby name filter");
	ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() / 8, MainWindow::GetWindowHeight()/4));
	ImGui::InputTextEx("##", "Lobby name filter", SteamNetworkingManager::GetLobbyBrowser()->GetFilterName(), 63, ImVec2(MainWindow::GetWindowWidth() / 3, filter_text_size.y), ImGuiInputTextFlags_CallbackEdit, InputFilterTextCalback);
	
	ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() / 8 + MainWindow::GetWindowWidth() / 3 + 10, MainWindow::GetWindowHeight() / 4));
	if (CustomLobbyBrowser::RefreshButton(filter_text_size.y, ImGui::GetCursorPos())) {
		SteamNetworkingManager::GetLobbyBrowser()->Refresh();
	}

	static int item_current_idx = -1;

	ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() / 8, ImGui::GetCursorPosY() + 10));
	if (ImGui::BeginListBox("##listbox 2", ImVec2(MainWindow::GetWindowWidth() / 3 + filter_text_size.y + 10, 10 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (int i = 0; i < SteamNetworkingManager::GetLobbyBrowser()->GetlobbyCount(); i++)
		{

			const bool is_selected = (item_current_idx == i);

			std::string a = SteamNetworkingManager::GetLobbyBrowser()->GetLobbyByIndex(i).m_name;
			if (ImGui::Selectable(SteamNetworkingManager::GetLobbyBrowser()->GetLobbyByIndex(i).m_name, false, 0, ImVec2(MainWindow::GetWindowWidth() / 3 + filter_text_size.y + 10, filter_text_size.y)))
			{
				m_gui_state = LOBBY_LOADING;
				SteamNetworkingManager::JoinLobby(SteamNetworkingManager::GetLobbyBrowser()->GetLobbyByIndex(i).m_id);
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

	ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() / 8, ImGui::GetCursorPosY() + 10));
	if (ImGui::Button("Back", ImVec2(MainWindow::GetWindowWidth() / 10, filter_text_size.y))) {
		m_gui_state = MAIN_MENU;
	}

	ImGui::PopFont();
	EndImGuiTranspWindow();
}
void MainMenuGui::DrawLobbyLoading() noexcept {
	ImGui::SetNextWindowPos(ImVec2(MainWindow::GetWindowWidth()/ 2, MainWindow::GetWindowHeight() /2));
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::Begin("DrawLobbyLoading", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(25, MainWindow::GetWindowHeight() / 40);
	ImGui::Text("Loading");
	ImGui::End();
}
void MainMenuGui::DrawReadyOrNotMenu() noexcept {
	ImGui::SetNextWindowPos(ImVec2(0,0));
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::Begin("Lobby Browser", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(25, MainWindow::GetWindowHeight() / 40);

	static bool is_self_ready = false;

	bool is_aponent_ready = SteamNetworkingManager::GetAponentReadyOrNotStatus();
	ImGui::Image(m_pSelf_avatar, ImVec2(128, 128));
	ImGui::SameLine();
	ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + 64));
	ImGui::Text(SteamNetworkingManager::GetSelfUserNickName());
	if (SteamNetworkingManager::IsThisClientLobbyOwner()) {
		ImGui::SameLine();
		ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + 64));
		ImGui::Text("OWNER---");
	}
	if (is_self_ready) {
		ImGui::SameLine();
		ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + 64));
		ImGui::Text("READY");
	}


	if (SteamNetworkingManager::GetLobbyMemberCount() == 2) {
		ImGui::Image(m_pAponent_avatar, ImVec2(128, 128));
		ImGui::SameLine();
		ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + 64));
		ImGui::Text(SteamNetworkingManager::GetAponentUserNickName());
		if (!SteamNetworkingManager::IsThisClientLobbyOwner()) {
			ImGui::SameLine();
			ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + 64));
			ImGui::Text("OWNER---");
					
		}
		if (is_aponent_ready) {
			ImGui::SameLine();
			ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + 64));
			ImGui::Text("READY");
		}
	}

	ImGui::SameLine();
	ImGui::SetCursorScreenPos(ImVec2(MainWindow::GetWindowWidth() / 2, MainWindow::GetWindowHeight() / 3));
	if (SteamNetworkingManager::GetLobbyMemberCount() != 2) {
		ImGui::Text("Waiting player");
	}
	else{
		int sec_rem = 0;
		bool is_should_start_server = SteamNetworkingManager::StartServerRetarder(is_aponent_ready && is_self_ready, sec_rem);
		if (is_aponent_ready) {
			if (is_self_ready) {
				if (is_should_start_server) {
					m_gui_state = STARTING_SERVER;///---------------- TODO SERVER LOADING GUI_STATE
				}
				ImGui::Text(("Server starts after " + std::to_string(sec_rem) + " seconds").c_str());
			}
			else {
				ImGui::Text("Your aponent is ready to play");
			}
		}
		else {
			if (is_self_ready) {
				ImGui::Text("Waiting for your aponent ready to play");
			}
			else {
				ImGui::Text("Waitin for both players ready");
			}
		}
	}
	
	
	if (SteamNetworkingManager::GetLobbyStruct()->m_id != 0) {
		ImGui::SetCursorScreenPos(ImVec2(MainWindow::GetWindowWidth() - 135 - MainWindow::GetWindowWidth() / 50, MainWindow::GetWindowHeight() - MainWindow::GetWindowHeight() / 10));
		if (TestButton("Ready", ImVec2(135, 75), ImGuiButtonFlags_None, is_self_ready)) {
			is_self_ready = is_self_ready ? false : true;
			SteamNetworkingManager::SetReadyOrNotStatus(is_self_ready);
		}
	}	

	ImGui::SetCursorScreenPos(ImVec2(MainWindow::GetWindowWidth() / 50, MainWindow::GetWindowHeight() - MainWindow::GetWindowHeight() / 10));
	if (ImGui::Button("Back", ImVec2(135, 75))) {
		SteamNetworkingManager::LeaveLobby();
		m_gui_state = MAIN_MENU;
	}
	ImGui::End();
}
void MainMenuGui::DrawStartingServer() noexcept {
	ImGui::SetNextWindowPos(ImVec2(MainWindow::GetWindowWidth() / 2, MainWindow::GetWindowHeight() / 2));
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::Begin("DrawLobbyLoading", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(25, MainWindow::GetWindowHeight() / 40);
	if (SteamNetworkingManager::IsThisClientLobbyOwner()) {
		ImGui::Text("Creating server...");
	}
	else {
		ImGui::Text("Connecting to server..");
	}	
	ImGui::End();
}
void MainMenuGui::DrawSetting() noexcept {
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
void MainMenuGui::DrawQuitDialog() noexcept {
	BeginImGuiTranspWindow("Quit Dialog");
	ImFont* pFont = ImGui::GetFont();

	pFont->Scale = (float)MainWindow::GetWindowWidth() / 3200.0f;
	ImGui::PushFont(pFont);

	ImVec2 mes_size = ImGui::CalcTextSize("Are you sure you want to quit the game?");

	ImGui::SetCursorPos(ImVec2((MainWindow::GetWindowWidth() / 2) - mes_size.x / 2,
		MainWindow::GetWindowHeight() / 2.5));
	ImGui::Text("Are you sure you want to quit the game?");

	auto pos_y_forconf = ImGui::GetCursorPosY() + 20;
	ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() * 5 / 14, pos_y_forconf));
	if (ConfBackButton("Back", ImColor(115, 160, 115, 255 - 115), ImColor(100, 160, 100, 255 - 100))) {
		m_gui_state = MAIN_MENU;
	}

	ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() * 5 / 14 + MainWindow::GetWindowWidth() / 7, pos_y_forconf));
	if (ConfBackButton("Quit", ImColor(160, 115, 115, 255 - 115), ImColor(160, 100, 100, 255 - 100))) {
		G_isShould_close_window = true;
	}

	ImGui::PopFont();
	EndImGuiTranspWindow();
}

void MainMenuGui::UpdateAponentAvatar() noexcept {
	if (SteamNetworkingManager::IsSteamConnected()) {
		m_pSelf_avatar = (char*)SteamNetworkingManager::LoadSelfAvatar();
		m_pAponent_avatar = (char*)SteamNetworkingManager::LoadAponentAvatar();
	}
}


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