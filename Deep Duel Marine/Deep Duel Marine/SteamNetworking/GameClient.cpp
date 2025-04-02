#include "GameClient.h"
#include "../GUI/GUI.h"



GameClient::GameClient() noexcept {
	m_lobby = Lobby();
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
		is_client_owns_lobby = true;

		SteamMatchmaking()->SetLobbyData(pCallback->m_ulSteamIDLobby, "name", m_lobby.m_name);
		SteamMatchmaking()->SetLobbyData(pCallback->m_ulSteamIDLobby, "isPrivate", m_lobby.m_isPublic ? "1" : "0");
		SteamMatchmaking()->SetLobbyData(pCallback->m_ulSteamIDLobby, "isDDM", "1");
	}
}

void GameClient::LeaveLobby() noexcept {
	if (!m_lobby.m_id) return;

	SteamMatchmaking()->LeaveLobby(m_lobby.m_id);

	m_lobby.m_id = 0;
	m_lobby = Lobby();
}