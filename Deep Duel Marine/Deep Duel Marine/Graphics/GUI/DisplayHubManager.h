#pragma once
#include <vector>

#include "IGUI.h"
#include "GUI.h"



class DisplayHub {
public:
	DisplayHub(void* pGUI, void* pScene) noexcept {
		m_pGui = (IGUI*)pGUI;
		///m_pScene ...
	}
	~DisplayHub() noexcept {
		delete m_pGui;
		///delete m_pScene;
	}

	void Display() noexcept {
		m_pGui->Draw();
		///m_pScene->Render();
	}

private:
	IGUI* m_pGui;
	///TODO scene class
};

enum DISPLAYHUB_MANAGER_STATE{
	DHM_MAIN_MENU,
	DHM_IN_GAME
};

class DisplayHubManager
{
public:
	static void Initialize() noexcept;
	static void Terminate() noexcept;

	static void Display() noexcept;

	static void ChangeState(DISPLAYHUB_MANAGER_STATE new_state) noexcept;

private:
	static DisplayHub* s_current_displayHub;
	static std::vector<DisplayHub*> s_displayHubs;
};

