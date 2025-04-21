#include "DisplayHubManager.h"
#include "../Scene/MainMenuScene.h"



DisplayHub* DisplayHubManager::s_current_displayHub = nullptr;
std::vector<DisplayHub*> DisplayHubManager::s_displayHubs{};


DisplayHub::DisplayHub(void* pGUI, void* pScene) noexcept {
	m_pGui = (IGUI*)pGUI;
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
	s_displayHubs.push_back(new DisplayHub(new GUI(), new MainMenuScene()));/// MAIN_MENU state
	s_displayHubs.push_back(new DisplayHub(0, 0));/// IN_GAME state

	ChangeState(DHM_MAIN_MENU);
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