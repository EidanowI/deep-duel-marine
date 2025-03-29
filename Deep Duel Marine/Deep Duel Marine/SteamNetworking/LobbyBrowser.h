#pragma once
#include <vector>
#include <string>
#include <algorithm>

#include "../Dependencies/steam/public/steam/steam_api.h"

#include "Lobby.h"


class LobbyBrowser {
public:
	LobbyBrowser() noexcept;

	void Refresh() noexcept;

	char* GetFilterName() noexcept;

	int GetlobbyCount() noexcept;
	const Lobby& GetLobbyByIndex(int index) noexcept;

private:
	CCallResult<LobbyBrowser, LobbyMatchList_t> m_SteamCallResultLobbyMatchList;
	void OnLobbyMatchListCallback(LobbyMatchList_t* pLobbyMatchList, bool bIOFailure);

private:
	std::vector<Lobby> m_lobbys;
	char m_pFilter_name[64]{};
};