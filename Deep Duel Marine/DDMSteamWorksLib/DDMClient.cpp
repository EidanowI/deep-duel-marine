#include "DDMClient.h"
#include "DDMSteamWorksLib.h"
#include "DDMMessages.h"



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

		m_gameState = GAME_STATE_IN_MENU;
		m_lobby = DDMLobby();
	}
	DDMClient::~DDMClient(){}

	void DDMClient::Update() {
		if (m_gameState == GAME_STATE_IN_LOBBY) {
			if (IsOwnsLobby()) {
				if (SWNetworkingManager::GetServer() && SWNetworkingManager::GetServer()->IsConnectedToSteam()) {
					SteamMatchmaking()->SetLobbyGameServer(m_lobby.GetID(), 0, 0, SWNetworkingManager::GetServer()->GetSteamID());
					InitiateServerConnection(SWNetworkingManager::GetServer()->GetSteamID());
				}
			}
		}

		ReceiveNetworkData();
	}

	GAME_STATE DDMClient::GetGameState() {
		return m_gameState;
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
			//SteamMatchmaking()->SetLobbyData(pCallback->m_ulSteamIDLobby, "isDDM", "1");
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
		if (!m_lobby.GetID()) return false;
		return SteamUser()->GetSteamID() == SteamMatchmaking()->GetLobbyOwner(m_lobby.GetID());
	}

	DDMLobby* DDMClient::GetLobby() {
		return &m_lobby;
	}
	CSteamID DDMClient::GetID() {
		return m_steamID;
	}

	void DDMClient::SetGameResultCalbacks(void (*pWin_func_calback)(), void (*pLose_func_claback)()) {
		m_pWin_func_calback = pWin_func_calback;
		m_pLose_func_claback = pLose_func_claback;
	}
	void DDMClient::SetSessionStartCallback(void (*pSession_start)()) {
		m_pSession_start = pSession_start;
	}
	void DDMClient::SetTurnCallback(void (*pTurn_calback)()) {
		m_pTurn_callback = pTurn_calback;
	}
	void DDMClient::SetShotCalback(void (*pShot_calback)(int x, int y, bool is_dead)) {
		m_pShot_calback = pShot_calback;
	}
	void DDMClient::SetGotDeadCalback(void (*pGotDead_calback)(int x, int y)) {
		m_pGotDead = pGotDead_calback;
	}

	void DDMClient::ReceiveNetworkData() {
		if (!SteamNetworkingSockets())
			return;
		if (m_hConnServer == k_HSteamNetConnection_Invalid)
			return;

		SteamNetworkingMessage_t* msgs[32];
		int res = SteamNetworkingSockets()->ReceiveMessagesOnConnection(m_hConnServer, msgs, 32);
		for (int i = 0; i < res; i++)
		{
			SteamNetworkingMessage_t* message = msgs[i];
			uint32 cubMsgSize = message->GetSize();

			// make sure we're connected
			if (m_gameState != GAME_STATE_CONNECTED)
			{
				message->Release();
				continue;
			}

			if (cubMsgSize < sizeof(DWORD))
			{
				//OutputDebugString("Got garbage on client socket, too short\n");
				message->Release();
				continue;
			}

			EMessage eMsg = (EMessage)*(DWORD*)message->GetData();
			switch (eMsg)
			{
			case k_EMsgServerSendInfo:
			{
				if (cubMsgSize != sizeof(MsgServerSendInfo_t))
				{
					break;
				}
				MsgServerSendInfo_t* pMsg = (MsgServerSendInfo_t*)message->GetData();

				// pull the IP address of the user from the socket
				OnReceiveServerInfo(CSteamID(pMsg->GetSteamIDServer()), pMsg->GetSecure(), pMsg->GetServerName());
			}
			break;
			case k_EMsgClientPassAuth:
			{
				if (cubMsgSize != sizeof(MsgServerPassAuthentication_t))
				{
					break;
				}
				MsgServerPassAuthentication_t* pMsg = (MsgServerPassAuthentication_t*)message->GetData();

				// Our game client doesn't really care about whether the server is secure, or what its
				// steamID is, but if it did we would pass them in here as they are part of the accept message
				OnReceiveServerAuthenticationResponse(true);
			}
			break;
			case k_EMsgClientFailAuth:
			{
				OnReceiveServerAuthenticationResponse(false);
			}
			break;
			case k_EMsgClientWin:
			{
				if (cubMsgSize != sizeof(MsgServerSendWinToClient_t))
				{
					//OutputDebugString("Bad server info msg\n");
					break;
				}
				MsgServerSendWinToClient_t* pMsg = (MsgServerSendWinToClient_t*)message->GetData();

				DisconnectFromServer();

				m_pWin_func_calback();
			}
			break;
			case k_EMsgClientLose:
			{
				if (cubMsgSize != sizeof(MsgServerSendLoseToClient_t))
				{
					//OutputDebugString("Bad server info msg\n");
					break;
				}
				MsgServerSendLoseToClient_t* pMsg = (MsgServerSendLoseToClient_t*)message->GetData();

				DisconnectFromServer();

				m_pLose_func_claback();
			}
			break;
			case k_EMsgTwoClientsReadySoStart:
			{
				if (cubMsgSize != sizeof(MsgTwoClientsAreReadySoStart_t))
				{
					//OutputDebugString("Bad server info msg\n");
					break;
				}
				MsgTwoClientsAreReadySoStart_t* pMsg = (MsgTwoClientsAreReadySoStart_t*)message->GetData();

				m_pSession_start();

			}
			break;
			case k_EMsgTurn:
			{
				if (cubMsgSize != sizeof(MsgTurn_t))
				{
					//OutputDebugString("Bad server info msg\n");
					break;
				}
				MsgTurn_t* pMsg = (MsgTurn_t*)message->GetData();

				m_pTurn_callback();
			}
			break;
			case k_EMsgShotResult:
			{
				if (cubMsgSize != sizeof(MsgShotResult_t))
				{
					//OutputDebugString("Bad server info msg\n");
					break;
				}
				MsgShotResult_t* pMsg = (MsgShotResult_t*)message->GetData();

				m_pShot_calback(pMsg->m_x, pMsg->m_y, pMsg->is_dead);
			}
			break;
			case k_EMsgGotDead:
			{
				if (cubMsgSize != sizeof(MsgGotDead_t))
				{
					//OutputDebugString("Bad server info msg\n");
					break;
				}
				MsgGotDead_t* pMsg = (MsgGotDead_t*)message->GetData();

				m_pGotDead(pMsg->m_x, pMsg->m_y);
			}
			break;


			default:

				break;
			}

			message->Release();
		}
	}
	bool DDMClient::SendNetworkData(const void* pData, unsigned int nSizeOfData, int nSendFlags) {
		EMessage eMsg = (EMessage)*(DWORD*)pData;
		EResult res = SteamNetworkingSockets()->SendMessageToConnection(m_hConnServer, pData, nSizeOfData, nSendFlags, nullptr);
		switch (res)
		{
		case k_EResultOK:
		case k_EResultIgnored:
			break;

		case k_EResultInvalidParam:
			OutputDebugStringA("Failed sending data to server: Invalid connection handle, or the individual message is too big\n");
			return false;
		case k_EResultInvalidState:
			OutputDebugStringA("Failed sending data to server: Connection is in an invalid state\n");
			return false;
		case k_EResultNoConnection:
			OutputDebugStringA("Failed sending data to server: Connection has ended\n");
			return false;
		case k_EResultLimitExceeded:
			OutputDebugStringA("Failed sending data to server: There was already too much data queued to be sent\n");
			return false;
		default:
		{
			char msg[256];
			sprintf(msg, "SendMessageToConnection returned %d\n", res);
			OutputDebugStringA(msg);
			return false;
		}
		}
		return true;
	}
	void DDMClient::OnReceiveServerInfo(CSteamID steamIDGameServer, bool bVACSecure, const char* pchServerName) {
		

		//m_client_connectionState = k_EClientConnectedAndAuthenticated;

		m_steamIDGameServer = steamIDGameServer;
		SteamNetConnectionInfo_t info;
		SteamNetworkingSockets()->GetConnectionInfo(m_hConnServer, &info);
		m_unServerIP = info.m_addrRemote.GetIPv4();
		m_usServerPort = info.m_addrRemote.m_port;

		///TODO ADD UpdateRichPresenceConnectionInfo FOR CONNECTING USING FRIEND LIST
		// set how to connect to the game server, using the Rich Presence API
		// this lets our friends connect to this game via their friends list
		//UpdateRichPresenceConnectionInfo();

		MsgClientBeginAuthentication_t msg;
		msg.SetSteamID(m_steamID.ConvertToUint64());


		SendNetworkData(&msg, sizeof(msg), k_nSteamNetworkingSend_Reliable);
	}
	void DDMClient::OnReceiveServerAuthenticationResponse(bool bSuccess)
	{
		if (!bSuccess)
		{
			MessageBoxA(0,"Connection failure","Multiplayer authentication failed",0);
			DisconnectFromServer();
		}
		else
		{
			if (m_gameState == GAME_STATE_CONNECTED)
				return;

			m_gameState = GAME_STATE_CONNECTED;
			//m_client_connectionState = k_EClientConnectedAndAuthenticated;

			// set information so our friends can join the lobby
			///TODO  --  UpdateRichPresenceConnectionInfo();

			// tell steam china duration control system that we are in a match and not to be interrupted
			SteamUser()->BSetDurationControlOnlineState(k_EDurationControlOnlineState_OnlineHighPri);
		}
	}

	void DDMClient::InitiateServerConnection(CSteamID steamIDGameServer) {
		if (m_gameState != GAME_STATE_IN_LOBBY)
			return;

		if (m_gameState == GAME_STATE_IN_LOBBY && CSteamID(m_lobby.GetID()).IsValid())
		{
			m_lobby.ClearLobbyData();
		}

		m_gameState = GAME_STATE_CONNECTED;

		SteamNetworkingIdentity identity;
		identity.SetSteamID(steamIDGameServer);

		m_hConnServer = SteamNetworkingSockets()->ConnectP2P(identity, 0, 0, nullptr);
	}

	void DDMClient::DisconnectFromServer() {
		if (m_hConnServer != k_HSteamNetConnection_Invalid)
			SteamNetworkingSockets()->CloseConnection(m_hConnServer, k_EDRClientDisconnect, nullptr, false);
		m_hConnServer = k_HSteamNetConnection_Invalid;

		m_lobby.ClearLobbyData();///Na vsiakiy sluchai

		m_gameState = GAME_STATE_IN_MENU;
	}
	void DDMClient::OnLobbyGameCreated(LobbyGameCreated_t* pCallback) {
		
		if (m_gameState != GAME_STATE_IN_LOBBY)
			return;

		if (CSteamID(pCallback->m_ulSteamIDGameServer).IsValid())
		{
			InitiateServerConnection(CSteamID(pCallback->m_ulSteamIDGameServer));
		}
	}
}