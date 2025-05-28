#include "InGameGui.h"
#include "../../SteamNetworking/include/DDMMessages.h"
#include "../../SteamNetworking/include/DDMSteamWorksLib.h"
#include "../../SteamNetworking/include/DDMClient.h"
#include "../../SteamNetworking/include/DDMMessages.h"



InGameGui::InGameGui() noexcept {
}
InGameGui::~InGameGui() noexcept {
}

void InGameGui::Draw() noexcept {
	/*ImGui::Begin("Something");
	ImVec2 loading_text_size = ImGui::CalcTextSize("We are in game");
	//ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() - loading_text_size.x - 30, MainWindow::GetWindowHeight() - loading_text_size.y - 20));
	ImGui::Text("We are in game");

	if (ImGui::Button("Win")) {
		MsgClientRequestWin_t winReq = MsgClientRequestWin_t();

		DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->SendNetworkData(&winReq, sizeof(MsgClientRequestWin_t), k_nSteamNetworkingSend_Unreliable);
	}
	if (ImGui::Button("Lose")) {
		MsgClientRequestLose_t loseReq = MsgClientRequestLose_t();

		DDMSteamWorksLib::SWNetworkingManager::GetDDMClient()->SendNetworkData(&loseReq, sizeof(MsgClientRequestLose_t), k_nSteamNetworkingSend_Unreliable);
	}

	ImGui::End();
	*/

}