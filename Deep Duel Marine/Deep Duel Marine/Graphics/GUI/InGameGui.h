#pragma once
#include <string>
#include <algorithm>

#include "../../Dependencies/imGUI/imgui.h"
#include "../../Dependencies/imGUI/imgui_internal.h"
//#include "../Dependencies/imGUI/"

#include "../MainWindow/MainWindow.h"
#include "../../SteamNetworking/include/DDMSteamWorksLib.h"
//#include "../../SteamNetworking/SteamNetworking.h"
#include "IGUI.h"
#include "DisplayHubManager.h"



class InGameGui : public IGui {
public:
	InGameGui() noexcept;
	~InGameGui() noexcept;

	void Draw() noexcept override;
};