#pragma once
#include <vector>
#include <string>
#include <algorithm>

#include "Dependencies/steam/public/steam/steam_api.h"



namespace DDMSteamWorksLib {
	struct BrowserLobby {
		unsigned long long m_id = 0;
		char m_name[64]{};
	};

	class DDMLobbyBrowser {
	public:
		DDMLobbyBrowser();
		~DDMLobbyBrowser();

		__declspec(dllexport) void Refresh();

		__declspec(dllexport) char* GetFilterName();

		__declspec(dllexport) int GetlobbyCount();
		__declspec(dllexport) const BrowserLobby& GetLobbyByIndex(int index);

	private:
		CCallResult<DDMLobbyBrowser, LobbyMatchList_t> m_SteamCallResultLobbyMatchList;
		void OnLobbyMatchListCallback(LobbyMatchList_t* pLobbyMatchList, bool bIOFailure);

	private:
		std::vector<BrowserLobby> m_lobbyIDs;
		char m_pFilter_name[64]{};
	};
}