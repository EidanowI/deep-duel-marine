#include "DisplayHubManager.h"
#include "../GUI/MainMenuGui.h"
#include "../GUI/InGameGui.h"
#include "../GUI/WinScreenGui.h"
#include "../GUI/LoseScreenGui.h"

#include "../Scene/MainMenuScene.h"
#include "../Scene/InGameScene.h"



DisplayHub* DisplayHubManager::s_current_displayHub = nullptr;
std::vector<DisplayHub*> DisplayHubManager::s_displayHubs{};


DisplayHub::DisplayHub(IGui* pGUI, IScene* pScene) noexcept {
	m_pGui = pGUI;
	m_pScene = (IScene*)pScene;
}
DisplayHub::~DisplayHub() noexcept {
	delete m_pGui;
	delete m_pScene;
}

void DisplayHub::Display() noexcept {
	m_pGui->Draw();
	m_pScene->Render();
}


void DisplayHubManager::Initialize() noexcept {
	s_displayHubs.push_back(new DisplayHub(new MainMenuGui(), new MainMenuScene()));/// MAIN_MENU state
	s_displayHubs.push_back(new DisplayHub(new InGameGui(), new InGameScene()));/// IN_GAME state
	s_displayHubs.push_back(new DisplayHub(new WinScreenGui(), new MainMenuScene()));/// WIN_SCREEN state
	s_displayHubs.push_back(new DisplayHub(new LoseScreenGui(), new MainMenuScene()));/// LOSE_SCREEN state

	ChangeState(DHM_IN_GAME);
}
void DisplayHubManager::Terminate() noexcept {
	for (int i = 0; i < s_displayHubs.size(); i++) {
		delete s_displayHubs[i];
	}
}

void DisplayHubManager::Display() noexcept {
	s_current_displayHub->Display();
}

void DisplayHubManager::ChangeState(DISPLAYHUB_MANAGER_STATE new_state) noexcept {
	s_current_displayHub = s_displayHubs[new_state];
}