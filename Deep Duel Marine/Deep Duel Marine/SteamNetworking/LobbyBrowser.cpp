#include "LobbyBrowser.h"


bool ContainsSubstring(const std::string& str, const std::string& substr);


LobbyBrowser::LobbyBrowser() noexcept {
	Refresh();
}

void LobbyBrowser::Refresh() noexcept {
	SteamAPICall_t hSteamAPICall = SteamMatchmaking()->RequestLobbyList();
	m_SteamCallResultLobbyMatchList.Set(hSteamAPICall, this, &LobbyBrowser::OnLobbyMatchListCallback);
}
void LobbyBrowser::OnLobbyMatchListCallback(LobbyMatchList_t* pCallback, bool bIOFailure) {
	m_lobbys.clear();

	if (bIOFailure) return;

	for (int i = 0; i < pCallback->m_nLobbiesMatching; i++)
	{
		Lobby lobby;
		lobby.m_id = SteamMatchmaking()->GetLobbyByIndex(i).ConvertToUint64();
		const char* nn = SteamMatchmaking()->GetLobbyData(SteamMatchmaking()->GetLobbyByIndex(i), "name");
		//if (SteamMatchmaking()->GetLobbyData(SteamMatchmaking()->GetLobbyByIndex(i), "isDDM")[0] != '1') continue;
		if (nn[0] != 0) {
			strncpy(lobby.m_name, SteamMatchmaking()->GetLobbyData(SteamMatchmaking()->GetLobbyByIndex(i), "name"), 63);
			lobby.m_isPublic = SteamMatchmaking()->GetLobbyData(SteamMatchmaking()->GetLobbyByIndex(i), "isPrivate")[0] == '1' ? true : false;

			if (ContainsSubstring(lobby.m_name, m_pFilter_name)) {
				m_lobbys.push_back(lobby);
			}
		}
	}

}

char* LobbyBrowser::GetFilterName() noexcept {
	return m_pFilter_name;
}

int LobbyBrowser::GetlobbyCount() noexcept {
	return m_lobbys.size();
}
const Lobby& LobbyBrowser::GetLobbyByIndex(int index) noexcept {
	return m_lobbys[index];
}

bool ContainsSubstring(const std::string& str, const std::string& substr) {
	std::string lowerStr = str;
	std::string lowerSubstr = substr;

	std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
	std::transform(lowerSubstr.begin(), lowerSubstr.end(), lowerSubstr.begin(), ::tolower);

	return lowerStr.find(lowerSubstr) != std::string::npos;
}