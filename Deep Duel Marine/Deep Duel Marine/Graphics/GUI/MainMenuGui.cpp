#include "MainMenuGui.h"



extern bool G_isShould_close_window;


MainMenuGui::MainMenuGui() noexcept {
}
MainMenuGui::~MainMenuGui() noexcept {
}

void MainMenuGui::Draw() noexcept {
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
			if (DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->GetLobby()->GetID())
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
	ImGui::Image(DDMSteamWorksLib::SWNetworkingManager::GetSelfAvatarResView(), ImVec2((int)(MainWindow::GetWindowWidth() / 14.77f), (int)(MainWindow::GetWindowHeight() / 8.31f)));

	pFont->Scale = (float)MainWindow::GetWindowWidth() / 1920.0f;
	ImGui::PushFont(pFont);
	if (DDMSteamWorksLib::SWNetworkingManager::GetSelfNickname()) {
		ImGui::SetCursorPos(ImVec2(7 + (int)(MainWindow::GetWindowWidth() / 14.77f) + 15, 7 + (int)(MainWindow::GetWindowHeight() / 8.31f / 2.0f) - 40 * (float)MainWindow::GetWindowWidth() / 1920.0f));
		ImGui::Text(DDMSteamWorksLib::SWNetworkingManager::GetSelfNickname());
	}
	ImGui::PopFont();
	
	pFont->Scale = (float)MainWindow::GetWindowWidth() / 3200.0f;
	ImGui::PushFont(pFont);

	int button_size_and_spacing = MainWindow::GetWindowHeight() / 20 + MainWindow::GetWindowHeight() / 45;

	ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() / 17, MainWindow::GetWindowHeight() / 2.8 + 0 * button_size_and_spacing));
	if (CustomMainMenu::MainMenuButton("Create a match")){
		if (DDMSteamWorksLib::SWNetworkingManager::IsConnectedToSteam()) {
			DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->GetLobby()->ClearLobbyData();
			m_gui_state = CREATE_LOBBY;
		}
	}

	ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() / 17, MainWindow::GetWindowHeight() / 2.8 + 1 * button_size_and_spacing));
	if (CustomMainMenu::MainMenuButton("Join a match")) {
		if (DDMSteamWorksLib::SWNetworkingManager::IsConnectedToSteam()) {
			DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->GetLobby()->ClearLobbyData();
			DDMSteamWorksLib::SWNetworkingManager::GetLobbyBrowser()->Refresh();
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
	ImGui::InputTextEx("##", 0, DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->GetLobby()->GetName(), 63, ImVec2(MainWindow::GetWindowWidth() / 3.5, lobby_name_text_size.y), 0);
	

	auto pos_y_forconf = ImGui::GetCursorPosY() + 20;
	ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() * 5 / 14, pos_y_forconf));
	if (ConfBackButton("Back", ImColor(160, 115, 115, 255 - 115), ImColor(160, 100, 100, 255 - 100))) {
		DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->GetLobby()->ClearLobbyData();
		m_gui_state = MAIN_MENU;
	}

	ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() * 5 / 14 + MainWindow::GetWindowWidth() / 7, pos_y_forconf));
	if (ConfBackButton("Confirm", ImColor(115, 160, 115, 255 - 115), ImColor(100, 160, 100, 255 - 100))) {
		DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->CreateLobby();
		m_gui_state = LOBBY_LOADING;
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
	ImGui::InputTextEx("##", "Lobby name filter", DDMSteamWorksLib::SWNetworkingManager::GetLobbyBrowser()->GetFilterName(), 63, ImVec2(MainWindow::GetWindowWidth() / 3, filter_text_size.y), ImGuiInputTextFlags_CallbackEdit, InputFilterTextCalback);
	
	ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() / 8 + MainWindow::GetWindowWidth() / 3 + 10, MainWindow::GetWindowHeight() / 4));
	if (CustomLobbyBrowser::RefreshButton(filter_text_size.y, ImGui::GetCursorPos())) {
		DDMSteamWorksLib::SWNetworkingManager::GetLobbyBrowser()->Refresh();
	}

	static int item_current_idx = -1;

	ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() / 8, ImGui::GetCursorPosY() + 10));
	if (ImGui::BeginListBox("##listbox 2", ImVec2(MainWindow::GetWindowWidth() / 3 + filter_text_size.y + 10, 10 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (int i = 0; i < DDMSteamWorksLib::SWNetworkingManager::GetLobbyBrowser()->GetlobbyCount(); i++)
		{

			const bool is_selected = (item_current_idx == i);

			std::string a = DDMSteamWorksLib::SWNetworkingManager::GetLobbyBrowser()->GetLobbyByIndex(i).m_name;
			if (ImGui::Selectable(DDMSteamWorksLib::SWNetworkingManager::GetLobbyBrowser()->GetLobbyByIndex(i).m_name, false, 0, ImVec2(MainWindow::GetWindowWidth() / 3 + filter_text_size.y + 10, filter_text_size.y)))
			{
				m_gui_state = LOBBY_LOADING;
				DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->JoinLobby(DDMSteamWorksLib::SWNetworkingManager::GetLobbyBrowser()->GetLobbyByIndex(i).m_id);
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
	BeginImGuiTranspWindow("Loading");
	ImFont* pFont = ImGui::GetFont();

	pFont->Scale = (float)MainWindow::GetWindowWidth() / 3200.0f;
	ImGui::PushFont(pFont);

	ImVec2 loading_text_size = ImGui::CalcTextSize("Loading");
	//ImGui::SetCursorPos(ImVec2((MainWindow::GetWindowWidth() - loading_text_size.x) / 2, (MainWindow::GetWindowHeight() - loading_text_size.y) / 2));
	ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() - loading_text_size.x - 30, MainWindow::GetWindowHeight() - loading_text_size.y - 20));
	ImGui::Text("Loading");

	ImGui::PopFont();

	EndImGuiTranspWindow();
}
void MainMenuGui::DrawReadyOrNotMenu() noexcept {
	static Texture ready_avatar_blend_tex = Texture("AvatarReady.png");
	static Texture leader_icon_tex = Texture("LeaderIcon.png"); 
	BeginImGuiTranspWindow("ReadyOrNot");
	ImFont* pFont = ImGui::GetFont();

	static bool is_self_ready = false;
	bool is_aponent_ready = false;
	if (DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->GetLobby()->GetLobbyMemberCount() == 2) {
		is_aponent_ready = DDMSteamWorksLib::SWNetworkingManager::GetAponentReadyOrNotStatus();
	}

	ImGui::SetCursorPos(ImVec2(7, 7));
	ImGui::Image(DDMSteamWorksLib::SWNetworkingManager::GetSelfAvatarResView(), ImVec2((int)(MainWindow::GetWindowWidth() / 14.77f), (int)(MainWindow::GetWindowHeight() / 8.31f)));

	if (is_self_ready) {
		ImGui::SetCursorPos(ImVec2(7, 7));
		ImGui::Image(ready_avatar_blend_tex.GetShaderResView(), ImVec2((int)(MainWindow::GetWindowWidth() / 14.77f), (int)(MainWindow::GetWindowHeight() / 8.31f)));
	}

	pFont->Scale = (float)MainWindow::GetWindowWidth() / 1920.0f;
	ImGui::PushFont(pFont);
	if (DDMSteamWorksLib::SWNetworkingManager::GetSelfNickname()) {
		ImVec2 self_nickname_size = ImGui::CalcTextSize(DDMSteamWorksLib::SWNetworkingManager::GetSelfNickname());
		ImGui::SetCursorPos(ImVec2(7 + (int)(MainWindow::GetWindowWidth() / 14.77f) + 15, 7 + (int)(MainWindow::GetWindowHeight() / 8.31f / 2.0f) - 40 * (float)MainWindow::GetWindowWidth() / 1920.0f));
		ImGui::Text(DDMSteamWorksLib::SWNetworkingManager::GetSelfNickname());

		if (DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->IsOwnsLobby()) {
			ImGui::SetCursorPos(ImVec2(7 + (int)(MainWindow::GetWindowWidth() / 14.77f) + 15
				+ self_nickname_size.x + 5,
				7 + MainWindow::GetWindowHeight() / 8.31f / 2.0f - MainWindow::GetWindowHeight() / 8.31f / 2.2f / 2.0f));
			ImGui::Image(leader_icon_tex.GetShaderResView(), ImVec2((int)(MainWindow::GetWindowWidth() / 14.77f / 2.2f), (int)(MainWindow::GetWindowHeight() / 8.31f / 2.2f)));
		}
	}
	ImGui::PopFont();

	///FOR APONENT
	ImVec2 iter_pos = ImVec2(MainWindow::GetWindowWidth() - 7 - MainWindow::GetWindowWidth() / 14.77f, 7);
	ImGui::SetCursorPos(iter_pos);
	ImGui::Image(DDMSteamWorksLib::SWNetworkingManager::GetAponentAvatarResView(), ImVec2((int)(MainWindow::GetWindowWidth() / 14.77f), (int)(MainWindow::GetWindowHeight() / 8.31f)));
	
	if (is_aponent_ready) {
		ImGui::SetCursorPos(ImVec2(iter_pos));
		ImGui::Image(ready_avatar_blend_tex.GetShaderResView(), ImVec2((int)(MainWindow::GetWindowWidth() / 14.77f), (int)(MainWindow::GetWindowHeight() / 8.31f)));
	}

	pFont->Scale = (float)MainWindow::GetWindowWidth() / 1920.0f;
	ImGui::PushFont(pFont);

	if (DDMSteamWorksLib::SWNetworkingManager::GetAponentNickname()) {
		ImVec2 aponent_nickname_size = ImGui::CalcTextSize(DDMSteamWorksLib::SWNetworkingManager::GetAponentNickname());

		iter_pos.x -= aponent_nickname_size.x + 15;

		ImGui::SetCursorPos(ImVec2(iter_pos.x, 7 + (int)(MainWindow::GetWindowHeight() / 8.31f / 2.0f) - 40 * (float)MainWindow::GetWindowWidth() / 1920.0f));
		ImGui::Text(DDMSteamWorksLib::SWNetworkingManager::GetAponentNickname());

		if (!DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->IsOwnsLobby()) {
			iter_pos.x -= 5 + MainWindow::GetWindowWidth() / 14.77f / 2.2f;
			ImGui::SetCursorPos(ImVec2(iter_pos.x,
				7 + MainWindow::GetWindowHeight() / 8.31f / 2.0f - MainWindow::GetWindowHeight() / 8.31f / 2.2f / 2.0f));
			ImGui::Image(leader_icon_tex.GetShaderResView(), ImVec2((int)(MainWindow::GetWindowWidth() / 14.77f / 2.2f), (int)(MainWindow::GetWindowHeight() / 8.31f / 2.2f)));
		}
	}
	ImGui::PopFont();

	pFont->Scale = (float)MainWindow::GetWindowWidth() / 3600.0f;
	ImGui::PushFont(pFont);
	if (DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->GetLobby()->GetLobbyMemberCount() != 2) {
		ImVec2 text_size = ImGui::CalcTextSize("Waiting player =(");

		ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() / 2 - text_size.x / 2, MainWindow::GetWindowHeight() - 10 - text_size.y));

		ImGui::Text("Waiting player =(");
	}
	else {
		if (is_aponent_ready) {
			if (is_self_ready) {
				m_gui_state = STARTING_SERVER;
			}
			else {
				ImVec2 text_size = ImGui::CalcTextSize("Your opponent is ready to play!");

				ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() / 2 - text_size.x / 2, MainWindow::GetWindowHeight() - 10 - text_size.y));

				ImGui::Text("Your opponent is ready to play!");
			}
		}
		else {
			if (is_self_ready) {
				ImVec2 text_size = ImGui::CalcTextSize("Waiting for your opponent ready to play");

				ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() / 2 - text_size.x / 2, MainWindow::GetWindowHeight() - 10 - text_size.y));

				ImGui::Text("Waiting for your opponent ready to play");
			}
			else {
				ImVec2 text_size = ImGui::CalcTextSize("Waiting for both players ready to play");

				ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() / 2 - text_size.x / 2, MainWindow::GetWindowHeight() - 10 - text_size.y));

				ImGui::Text("Waiting for both players ready to play");
			}
		}
	}
	ImGui::PopFont();

	ImGui::SetCursorScreenPos(ImVec2(MainWindow::GetWindowWidth() - 7 - MainWindow::GetWindowWidth() / 7.68f, MainWindow::GetWindowHeight() - 7 - MainWindow::GetWindowHeight() / 14.4f));
	if (CustomReadyOrNot::ReadyButton("Ready", is_self_ready)) {
		is_self_ready = is_self_ready ? false : true;
		DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->SetReadyOrNotStatus(is_self_ready);
	}

	ImGui::SetCursorScreenPos(ImVec2(7, MainWindow::GetWindowHeight() - 7 - MainWindow::GetWindowHeight() / 14.4f));
	if (CustomReadyOrNot::BackButton("Leave")) {
		DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->LeaveLobby();
		m_gui_state = MAIN_MENU;
	}

	EndImGuiTranspWindow();
	/*ImGui::SetNextWindowPos(ImVec2(0,0));
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::Begin("Lobby Browser", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(25, MainWindow::GetWindowHeight() / 40);

	static bool is_self_ready = false;
	bool is_aponent_ready = false;
	if (DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->GetLobby()->GetLobbyMemberCount() == 2) {
		is_aponent_ready = DDMSteamWorksLib::SWNetworkingManager::GetAponentReadyOrNotStatus();
	}
	ImGui::Image(DDMSteamWorksLib::SWNetworkingManager::GetSelfAvatarResView(), ImVec2(128, 128));
	ImGui::SameLine();
	ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + 64));
	ImGui::Text(DDMSteamWorksLib::SWNetworkingManager::GetSelfNickname());
	if (DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->IsOwnsLobby()) {
		ImGui::SameLine();
		ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + 64));
		ImGui::Text("OWNER---");
	}
	if (is_self_ready) {
		ImGui::SameLine();
		ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + 64));
		ImGui::Text("READY");
	}


	if (DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->GetLobby()->GetLobbyMemberCount() == 2) {
		///TODO He can get 0
		ImGui::Image(DDMSteamWorksLib::SWNetworkingManager::GetAponentAvatarResView(), ImVec2(128, 128));
		ImGui::SameLine();
		ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + 64));
		ImGui::Text(DDMSteamWorksLib::SWNetworkingManager::GetAponentNickname());
		if (!DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->IsOwnsLobby()) {
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
	if (DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->GetLobby()->GetLobbyMemberCount() != 2) {
		ImGui::Text("Waiting player");
	}
	else{
		if (is_aponent_ready) {
			if (is_self_ready) {
				m_gui_state = STARTING_SERVER;
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
	
	
	ImGui::SetCursorScreenPos(ImVec2(MainWindow::GetWindowWidth() - 135 - MainWindow::GetWindowWidth() / 50, MainWindow::GetWindowHeight() - MainWindow::GetWindowHeight() / 10));
	if (TestButton("Ready", ImVec2(135, 75), ImGuiButtonFlags_None, is_self_ready)) {
		is_self_ready = is_self_ready ? false : true;
		DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->SetReadyOrNotStatus(is_self_ready);
	}

	ImGui::SetCursorScreenPos(ImVec2(MainWindow::GetWindowWidth() / 50, MainWindow::GetWindowHeight() - MainWindow::GetWindowHeight() / 10));
	if (ImGui::Button("Back", ImVec2(135, 75))) {
		DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->LeaveLobby();
		m_gui_state = MAIN_MENU;
	}
	ImGui::End();*/
}
void MainMenuGui::DrawStartingServer() noexcept {
	///TODO starting
	//DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->GetLobby()->ClearLobbyData();
	ImGui::SetNextWindowPos(ImVec2(MainWindow::GetWindowWidth() / 2, MainWindow::GetWindowHeight() / 2));
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::Begin("DrawLobbyLoading", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(25, MainWindow::GetWindowHeight() / 40);
	if (DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->IsOwnsLobby()) {
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

//void MainMenuGui::UpdateAponentAvatar() noexcept {
//	if (SteamNetworkingManager::IsSteamConnected()) {
//		m_pSelf_avatar = (char*)SteamNetworkingManager::LoadSelfAvatar();
//		m_pAponent_avatar = (char*)SteamNetworkingManager::LoadAponentAvatar();
//	}
//}


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
	DDMSteamWorksLib::SWNetworkingManager::GetLobbyBrowser()->Refresh();
	return 0;
}