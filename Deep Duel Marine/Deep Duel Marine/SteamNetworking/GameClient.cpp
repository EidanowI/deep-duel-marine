#include <Windows.h>
#include "GameClient.h"



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
	LeaveLobby();
}
void GameClient::Update() noexcept {
	if (m_lobby.m_id) {
		auto count = SteamMatchmaking()->GetNumLobbyMembers(m_lobby.m_id);
		if (m_lobby_members_count != count) {
			m_lobby_members_count = count;

			is_client_owns_lobby = SteamUser()->GetSteamID() == SteamMatchmaking()->GetLobbyOwner(m_lobby.m_id);
		}
	}
}

void GameClient::SetReadyOrNotStatus(bool isReady) noexcept {
	if (!m_lobby.m_id) return;

	SteamMatchmaking()->SetLobbyMemberData(m_lobby.m_id, "ready", isReady ? "1" : "0");
}

int GameClient::GetLobbyMemberCount() noexcept {
	if ( m_lobby.m_id) {
		return SteamMatchmaking()->GetNumLobbyMembers(m_lobby.m_id);
	}
	else {
		return 0;
	}
}

void GameClient::CreateLobby() noexcept {
	if (!m_steamCallResultLobbyCreated.IsActive())
	{
		SteamAPICall_t hSteamAPICall = SteamMatchmaking()->CreateLobby(k_ELobbyTypePublic, MAX_LOBBY_MEMBERS);
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
		//SteamMatchmaking()->SetLobbyData(pCallback->m_ulSteamIDLobby, "isPrivate", m_lobby.m_isPublic ? "1" : "0");
		SteamMatchmaking()->SetLobbyData(pCallback->m_ulSteamIDLobby, "isDDM", "1");
		SetReadyOrNotStatus(false);
	}
}

void GameClient::JoinLobby(CSteamID id) noexcept {
	SteamAPICall_t hSteamAPICall = SteamMatchmaking()->JoinLobby(id);
	m_SteamCallResultLobbyEntered.Set(hSteamAPICall, this, &GameClient::OnLobbyEntered);
}
void GameClient::OnLobbyEntered(LobbyEnter_t* pCallback, bool bIOFailure) {
	if (m_client_gameState == GAME_STATE_IN_LOBBY)
		return;

	if (pCallback->m_EChatRoomEnterResponse != k_EChatRoomEnterResponseSuccess)
	{
		MessageBoxA(0, "ERROR", "Cant join", MB_ICONERROR);
		return;
	}

	// success
	m_lobby.m_id = pCallback->m_ulSteamIDLobby;
	m_client_gameState = GAME_STATE_IN_LOBBY;
	is_client_owns_lobby = false;
	SetReadyOrNotStatus(false);
}

void GameClient::LeaveLobby() noexcept {
	if (!m_lobby.m_id) return;

	m_client_gameState = GAME_STATE_IN_MENU;

	SteamMatchmaking()->LeaveLobby(m_lobby.m_id);

	m_lobby.m_id = 0;
	m_lobby = Lobby();
}