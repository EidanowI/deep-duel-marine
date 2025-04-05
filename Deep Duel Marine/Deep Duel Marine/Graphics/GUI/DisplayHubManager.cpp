#include "DisplayHubManager.h"



DisplayHub* DisplayHubManager::s_current_displayHub = nullptr;
std::vector<DisplayHub*> DisplayHubManager::s_displayHubs{};


void DisplayHubManager::Initialize() noexcept {
	s_displayHubs.push_back(new DisplayHub(new GUI(), 0));/// MAIN_MENU state
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