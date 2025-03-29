#include "GameClient.h"



GameClient::GameClient() noexcept {
	if (SteamUser()->BLoggedOn())
	{
		m_client_steamID = SteamUser()->GetSteamID();
	}

	//m_pServer = nullptr;
	m_client_connectionState = k_EClientNotConnected;
	m_client_gameState = GAME_STATE_IN_MENU;
}
GameClient::~GameClient() noexcept {

}

void GameClient::CreateLobby() noexcept {
	if (!m_steamCallResultLobbyCreated.IsActive())
	{
		SteamAPICall_t hSteamAPICall = SteamMatchmaking()->CreateLobby(m_lobby.m_isPublic ? k_ELobbyTypePublic : k_ELobbyTypeFriendsOnly, MAX_LOBBY_MEMBERS);
		m_steamCallResultLobbyCreated.Set(hSteamAPICall, this, &GameClient::OnLobbyCreated);
	}
	SteamFriends()->SetRichPresence("status", "Creating a lobby");
}
void GameClient::OnLobbyCreated(LobbyCreated_t* pCallback, bool bIOFailure) {
	if (pCallback->m_eResult == k_EResultOK)
	{
		m_lobby.m_id = pCallback->m_ulSteamIDLobby;
		m_client_gameState = GAME_STATE_IN_LOBBY;

		SteamMatchmaking()->SetLobbyData(pCallback->m_ulSteamIDLobby, "name", m_lobby.m_name);
		SteamMatchmaking()->SetLobbyData(pCallback->m_ulSteamIDLobby, "isPrivate", m_lobby.m_isPublic ? "1" : "0");
		SteamMatchmaking()->SetLobbyData(pCallback->m_ulSteamIDLobby, "isDDM", "1");
	}
}