#pragma once
#include <vector>

#include "IGui.h"
#include "../Scene/IScene.h"



class DisplayHub {
public:
	DisplayHub(IGui* pGUI, IScene* pScene) noexcept;
	~DisplayHub() noexcept;

	void Display() noexcept;

private:
	IGui* m_pGui;
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

