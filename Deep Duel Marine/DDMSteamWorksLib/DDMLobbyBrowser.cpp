#include "DDMLobbyBrowser.h"
#include <Windows.h>


namespace DDMSteamWorksLib {
	bool ContainsSubstring(const std::string& str, const std::string& substr);

	DDMLobbyBrowser::DDMLobbyBrowser() {
		Refresh();
		SteamAPI_RunCallbacks();
	}
	DDMLobbyBrowser::~DDMLobbyBrowser() {

	}
	
	void DDMLobbyBrowser::Refresh() {
		SteamAPICall_t hSteamAPICall = SteamMatchmaking()->RequestLobbyList();
		m_SteamCallResultLobbyMatchList.Set(hSteamAPICall, this, &DDMLobbyBrowser::OnLobbyMatchListCallback);
	}
	void DDMLobbyBrowser::OnLobbyMatchListCallback(LobbyMatchList_t* pCallback, bool bIOFailure) {
		m_lobbyIDs.clear();

		if (bIOFailure) return;

		for (int i = 0; i < pCallback->m_nLobbiesMatching; i++)
		{		
			BrowserLobby lobby;
			lobby.m_id = SteamMatchmaking()->GetLobbyByIndex(i).ConvertToUint64();
			const char* nn = SteamMatchmaking()->GetLobbyData(SteamMatchmaking()->GetLobbyByIndex(i), "name");
//#ifndef ENABLE_NOT_DEEP_DUEL_LOBBYS_IN_LOBBY_BROWSER
//			if (SteamMatchmaking()->GetLobbyData(SteamMatchmaking()->GetLobbyByIndex(i), "isDDM")) continue;
//#endif
			if (nn[0] != 0) {
				strncpy(lobby.m_name, SteamMatchmaking()->GetLobbyData(SteamMatchmaking()->GetLobbyByIndex(i), "name"), 63);

				if (ContainsSubstring(lobby.m_name, m_pFilter_name)) {
					m_lobbyIDs.push_back(lobby);
				}
			}
		}

	}

	char* DDMLobbyBrowser::GetFilterName() {
		return m_pFilter_name;
	}

	int DDMLobbyBrowser::GetlobbyCount() {
		return m_lobbyIDs.size();
	}

	const BrowserLobby& DDMLobbyBrowser::GetLobbyByIndex(int index) {
		return m_lobbyIDs[index];
	}

	bool ContainsSubstring(const std::string& str, const std::string& substr) {
		std::string lowerStr = str;
		std::string lowerSubstr = substr;

		std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
		std::transform(lowerSubstr.begin(), lowerSubstr.end(), lowerSubstr.begin(), ::tolower);

		return lowerStr.find(lowerSubstr) != std::string::npos;
	}
}