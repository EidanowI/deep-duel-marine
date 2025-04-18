#pragma once
#include <vector>

#include "IGUI.h"
#include "GUI.h"
#include "../Scene/IScene.h"



class DisplayHub {
public:
	DisplayHub(void* pGUI, void* pScene) noexcept;
	~DisplayHub() noexcept;

	void Display() noexcept;

private:
	IGUI* m_pGui;
	IScene* m_pScene;
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

