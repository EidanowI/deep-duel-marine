#include "DDMClient.h"
#include "DDMSteamWorksLib.h"



namespace DDMSteamWorksLib {
	DDMLobby::DDMLobby() : m_id(0) {
		memset(m_name, 0, 64);
		strncpy(m_name, SWNetworkingManager::GetSelfNickname(), 63);
		if (strlen(m_name) + 7 < 63) {
			memcpy(m_name + strlen(m_name), " lobby", 7);
		}
	}
	DDMLobby::~DDMLobby() {
		if (m_id) {
			LeaveLobby();
		}
	}

	void DDMLobby::SetID(unsigned long long id) {
		if (m_id) {
			LeaveLobby();
		}
		m_id = id;
	}
	unsigned long long DDMLobby::GetID() {
		return m_id;
	}
	char* DDMLobby::GetName() {
		return m_name;
	}

	int DDMLobby::GetLobbyMemberCount() {
		return SteamMatchmaking()->GetNumLobbyMembers(m_id);
	}

	void DDMLobby::ClearLobbyData() {
		LeaveLobby();

		memset(m_name, 0, 64);
		strncpy(m_name, SWNetworkingManager::GetSelfNickname(), 63);
		if (strlen(m_name) + 7 < 63) {
			memcpy(m_name + strlen(m_name), " lobby", 7);
		}
	}
	void DDMLobby::LeaveLobby() {
		if (!m_id) return;

		SteamMatchmaking()->LeaveLobby(m_id);

		m_id = 0;
	}


	DDMClient::DDMClient() {
		m_steamID = SteamUser()->GetSteamID();

		m_connectionState = k_EClientNotConnected;
		m_gameState = GAME_STATE_IN_MENU;
		m_lobby = DDMLobby();
	}
	DDMClient::~DDMClient(){}

	void DDMClient::Update() {

	}

	void DDMClient::CreateLobby() {
		m_lobby.LeaveLobby();

		if (!m_steamCallResultLobbyCreated.IsActive())
		{
			SteamAPICall_t hSteamAPICall = SteamMatchmaking()->CreateLobby(k_ELobbyTypePublic, MAX_LOBBY_MEMBERS);
			m_steamCallResultLobbyCreated.Set(hSteamAPICall, this, &DDMClient::OnLobbyCreated);
		}
		SteamFriends()->SetRichPresence("status", "Creating a lobby");
	}
	void DDMClient::OnLobbyCreated(LobbyCreated_t* pCallback, bool bIOFailure) {
		if (pCallback->m_eResult == k_EResultOK)
		{
			m_lobby.SetID(pCallback->m_ulSteamIDLobby);
			m_gameState = GAME_STATE_IN_LOBBY;

			SteamMatchmaking()->SetLobbyData(pCallback->m_ulSteamIDLobby, "name", m_lobby.GetName());
			SteamMatchmaking()->SetLobbyData(pCallback->m_ulSteamIDLobby, "isDDM", "1");
			SetReadyOrNotStatus(false);
		}
	}
	void DDMClient::JoinLobby(CSteamID id) {
		m_lobby.LeaveLobby();

		SteamAPICall_t hSteamAPICall = SteamMatchmaking()->JoinLobby(id);
		m_SteamCallResultLobbyEntered.Set(hSteamAPICall, this, &DDMClient::OnLobbyEntered);
	}
	void DDMClient::OnLobbyEntered(LobbyEnter_t* pCallback, bool bIOFailure) {
		if (m_gameState == GAME_STATE_IN_LOBBY)
			return;

		if (pCallback->m_EChatRoomEnterResponse != k_EChatRoomEnterResponseSuccess)
		{
			MessageBoxA(0, "ERROR", "Cant join", MB_ICONERROR);
			return;
		}

		// success
		m_lobby.SetID(pCallback->m_ulSteamIDLobby);
		m_gameState = GAME_STATE_IN_LOBBY;
		SetReadyOrNotStatus(false);
	}
	void DDMClient::LeaveLobby() {
		m_gameState = GAME_STATE_IN_MENU;

		m_lobby.ClearLobbyData();
	}

	void DDMClient::SetReadyOrNotStatus(bool isReady) {
		if(m_lobby.GetID()) SteamMatchmaking()->SetLobbyMemberData(m_lobby.GetID(), "ready", isReady ? "1" : "0");
	}

	bool DDMClient::IsOwnsLobby() {
		return SteamUser()->GetSteamID() == SteamMatchmaking()->GetLobbyOwner(m_lobby.GetID());
	}

	DDMLobby* DDMClient::GetLobby() {
		return &m_lobby;
	}
}