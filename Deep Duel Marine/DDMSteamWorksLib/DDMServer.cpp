#include "DDMServer.h"
#include "DDMMessages.h"



namespace DDMSteamWorksLib {
	DDMServer::DDMServer() : m_isConnected_to_steam(false), m_connectedPlayers_count(0){
		SteamErrMsg errMsg = { 0 };

		if (SteamGameServer_InitEx(INADDR_ANY, SERVER_PORT, MASTER_SERVER_UPDATER_PORT, SERVER_MODE, SERVER_VERSION, &errMsg) != k_ESteamAPIInitResult_OK)
		{
			MessageBoxA(0, errMsg, "SteamGameServer_Init call failed", 0);
		}

		if (SteamGameServer())
		{
			//SteamGameServer()->SetModDir(GAMEDIR);

			SteamGameServer()->SetDedicatedServer(false);

			//SteamGameServer()->SetGameData("official");//for filters

			//SteamGameServer()->SetGameDescription("Steamworks Example");

			//SteamGameServer()->SetGameTags("classic");

			//SteamGameServer()->SetMapName("procedural");

			SteamGameServer()->SetMaxPlayerCount(2);

			SteamGameServer()->SetPasswordProtected(false);

			//SteamGameServer()->SetProduct("SteamworksExample");

			//SteamGameServer()->SetRegion("world");

			SteamGameServer()->SetServerName("DDM server");



			SteamGameServer()->LogOnAnonymous();

			//SteamNetworkingUtils()->InitRelayNetworkAccess();

			SteamGameServer()->SetAdvertiseServerActive(true);
		}
		// create the listen socket for listening for players connecting
		m_hListenSocket = SteamGameServerNetworkingSockets()->CreateListenSocketP2P(0, 0, nullptr);

		// create the poll group
		m_hNetPollGroup = SteamGameServerNetworkingSockets()->CreatePollGroup();
	}
	DDMServer::~DDMServer() {
		SteamGameServerNetworkingSockets()->CloseListenSocket(m_hListenSocket);
		SteamGameServerNetworkingSockets()->DestroyPollGroup(m_hNetPollGroup);

		SteamGameServer()->LogOff();

		// release our reference to the steam client library
		SteamGameServer_Shutdown();
	}

	void DDMServer::Update() {
		SteamGameServer_RunCallbacks();

		ReceiveNetworkData();
	}

	void DDMServer::OnSteamServersConnected(SteamServersConnected_t* pLogonSuccess)
	{
		m_gameState = ACTIVE;
		m_isConnected_to_steam = true;
	}
	void DDMServer::OnNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pCallback) {
		HSteamNetConnection hConn = pCallback->m_hConn;
		SteamNetConnectionInfo_t info = pCallback->m_info;
		ESteamNetworkingConnectionState eOldState = pCallback->m_eOldState;

		if (info.m_hListenSocket &&
			eOldState == k_ESteamNetworkingConnectionState_None &&
			info.m_eState == k_ESteamNetworkingConnectionState_Connecting)
		{
			// Connection from a new client
			// Search for an available slot
			for (uint32 i = 0; i < MAX_PLAYERS_ON_SERVER_COUNT; ++i)
			{
				if (!m_rgClientData[i].m_bActive && !m_rgPendingClientData[i].m_hConn)
				{

					// Found one.  "Accept" the connection.
					EResult res = SteamGameServerNetworkingSockets()->AcceptConnection(hConn);
					if (res != k_EResultOK)
					{
						char msg[256];
						sprintf(msg, "AcceptConnection returned %d", res);
						MessageBoxA(0, "Accept connection error", msg, MB_OK);
						//OutputDebugString(msg);
						SteamGameServerNetworkingSockets()->CloseConnection(hConn, k_ESteamNetConnectionEnd_AppException_Generic, "Failed to accept connection", false);
						return;
					}

					m_rgPendingClientData[i].m_hConn = hConn;
					//m_rgClientData[i].m_bActive = true;
					//m_rgClientData[i].m_SteamIDUser = pCallback->m_info.

					// add the user to the poll group
					SteamGameServerNetworkingSockets()->SetConnectionPollGroup(hConn, m_hNetPollGroup);

					MsgServerSendInfo_t msg;
					msg.SetSteamIDServer(SteamGameServer()->GetSteamID().ConvertToUint64());

					msg.SetSecure(false);
					SteamGameServerNetworkingSockets()->SendMessageToConnection(hConn, &msg, sizeof(MsgServerSendInfo_t), k_nSteamNetworkingSend_Reliable, nullptr);

					return;
				}
			}

			// No empty slots.  Server full!
			MessageBoxA(0, "Connection error", "Server full", MB_OK);
			SteamGameServerNetworkingSockets()->CloseConnection(hConn, k_ESteamNetConnectionEnd_AppException_Generic, "Server full!", false);
		}
		// Check if a client has disconnected
		else if ((eOldState == k_ESteamNetworkingConnectionState_Connecting || eOldState == k_ESteamNetworkingConnectionState_Connected) && info.m_eState == k_ESteamNetworkingConnectionState_ClosedByPeer)
		{
			// Handle disconnecting a client
			for (uint32 i = 0; i < MAX_PLAYERS_ON_SERVER_COUNT; ++i)
			{
				// If there is no ship, skip
				if (!m_rgClientData[i].m_bActive)
					continue;

				if (m_rgClientData[i].m_SteamIDUser == info.m_identityRemote.GetSteamID())//pCallback->m_steamIDRemote)
				{
					RemovePlayerFromServer(i,k_EDRClientDisconnect);
					break;
				}
			}
		}
	}

	void DDMServer::RemovePlayerFromServer(int index, EDisconnectReason reason) {
		if (index >= MAX_PLAYERS_ON_SERVER_COUNT)
		{
			return;
		}

		// close the hNet connection
		SteamGameServerNetworkingSockets()->CloseConnection(m_rgClientData[index].m_hConn, reason, nullptr, false);

		m_rgClientData[index] = ClientConnectionData_t();
	}

	void DDMServer::ReceiveNetworkData() {
		SteamNetworkingMessage_t* msgs[128];
		int numMessages = SteamGameServerNetworkingSockets()->ReceiveMessagesOnPollGroup(m_hNetPollGroup, msgs, 128);
		for (int idxMsg = 0; idxMsg < numMessages; idxMsg++)
		{
			SteamNetworkingMessage_t* message = msgs[idxMsg];
			CSteamID steamIDRemote = message->m_identityPeer.GetSteamID();
			HSteamNetConnection connection = message->m_conn;

			if (message->GetSize() < sizeof(DWORD))
			{
				//OutputDebugString("Got garbage on server socket, too short\n");
				message->Release();
				message = nullptr;
				continue;
			}

			EMessage eMsg = (EMessage)*(DWORD*)message->GetData();

			switch (eMsg)
			{
			case k_EMsgClientBeginAuth:
			{
				if (message->GetSize() != sizeof(MsgClientBeginAuthentication_t))
				{
					message->Release();
					message = nullptr;
					continue;
				}
				MsgClientBeginAuthentication_t* pMsg = (MsgClientBeginAuthentication_t*)message->GetData();

				OnClientBeginAuthentication(steamIDRemote, connection, 0, 0);
			}
			break;

			case k_EMsgRequestWin:
			{
				m_gameState = SUMMARIZING;
				MsgServerSendWinToClient_t winMsg = MsgServerSendWinToClient_t();
				MsgServerSendLoseToClient_t loseMsg = MsgServerSendLoseToClient_t();

				if (m_rgClientData[0].m_SteamIDUser == steamIDRemote) {		
					BSendDataToClient(0, (char*)&winMsg, sizeof(MsgServerSendWinToClient_t));
					BSendDataToClient(1, (char*)&loseMsg, sizeof(MsgServerSendLoseToClient_t));
				}
				else {
					BSendDataToClient(1, (char*)&winMsg, sizeof(MsgServerSendWinToClient_t));
					BSendDataToClient(0, (char*)&loseMsg, sizeof(MsgServerSendLoseToClient_t));
				}
			}
			break;
			case k_EMsgRequestLose:
			{
				m_gameState = SUMMARIZING;
				MsgServerSendWinToClient_t winMsg = MsgServerSendWinToClient_t();
				MsgServerSendLoseToClient_t loseMsg = MsgServerSendLoseToClient_t();

				if (m_rgClientData[0].m_SteamIDUser != steamIDRemote) {
					BSendDataToClient(0, (char*)&winMsg, sizeof(MsgServerSendWinToClient_t));
					BSendDataToClient(1, (char*)&loseMsg, sizeof(MsgServerSendLoseToClient_t));
				}
				else {
					BSendDataToClient(1, (char*)&winMsg, sizeof(MsgServerSendWinToClient_t));
					BSendDataToClient(0, (char*)&loseMsg, sizeof(MsgServerSendLoseToClient_t));
				}
			}
			break;

			default:
				break;
			}

			message->Release();
			message = nullptr;
		}
	}
	bool DDMServer::BSendDataToClient(int index, char* pData, uint32 nSizeOfData) {
		if (index >= MAX_PLAYERS_ON_SERVER_COUNT)
			return false;

		int64 messageOut;
		if (!SteamGameServerNetworkingSockets()->SendMessageToConnection(m_rgClientData[index].m_hConn, pData, nSizeOfData, k_nSteamNetworkingSend_Unreliable, &messageOut))
		{
			//OutputDebugString("Failed sending data to a client\n");
			return false;
		}
		return true;
	}

	bool DDMServer::IsConnectedToSteam() {
		return m_isConnected_to_steam;
	}
	CSteamID DDMServer::GetSteamID() {
		return SteamGameServer()->GetSteamID();
	}

	bool DDMServer::IsServerReadyToShut() {
		if (m_gameState == SUMMARIZING) {
			for (int i = 0; i < MAX_PLAYERS_ON_SERVER_COUNT; i++) {
				if (m_rgClientData[i].m_bActive) {
					return false;
				}
			}
			return true;
		}
		return false;
	}

	void DDMServer::OnClientBeginAuthentication(CSteamID steamIDClient, HSteamNetConnection connectionID, void* pToken, uint32 uTokenLen) {
		// First, check this isn't a duplicate and we already have a user logged on from the same steamid
		for (uint32 i = 0; i < MAX_PLAYERS_ON_SERVER_COUNT; ++i)
		{
			if (m_rgClientData[i].m_hConn == connectionID)
			{
				return;
			}
		}

		// Second, do we have room?
		uint32 nPendingOrActivePlayerCount = 0;
		for (uint32 i = 0; i < MAX_PLAYERS_ON_SERVER_COUNT; ++i)
		{
			if (m_rgPendingClientData[i].m_bActive)
				++nPendingOrActivePlayerCount;

			if (m_rgClientData[i].m_bActive)
				++nPendingOrActivePlayerCount;
		}

		// We are full (or will be if the pending players auth), deny new login
		if (nPendingOrActivePlayerCount >= MAX_PLAYERS_ON_SERVER_COUNT)
		{
			SteamGameServerNetworkingSockets()->CloseConnection(connectionID, k_EDRServerFull, "Server full", false);
		}

		// If we get here there is room, add the player as pending
		for (uint32 i = 0; i < MAX_PLAYERS_ON_SERVER_COUNT; ++i)
		{
			if (!m_rgPendingClientData[i].m_bActive)
			{
				m_rgPendingClientData[i].m_bActive = true;
				// we need to tell the server our Steam id in the non-auth case, so we stashed it in the login message, pull it back out
				//m_rgPendingClientData[i].m_SteamIDUser = *(CSteamID*)pToken;
				m_rgPendingClientData[i].m_hConn = connectionID;
				m_rgPendingClientData[i].m_SteamIDUser = steamIDClient;
				// You would typically do your own authentication method here and later call OnAuthCompleted
				// In this sample we just automatically auth anyone who connects
				OnAuthCompleted(true, i);
				break;
			}
		}
	}
	void DDMServer::OnAuthCompleted(bool bAuthSuccessful, uint32 iPendingAuthIndex) {
		if (!m_rgPendingClientData[iPendingAuthIndex].m_bActive)
		{
			//OutputDebugString("Got auth completed callback for client who is not pending\n");
			return;
		}

		if (!bAuthSuccessful)
		{
			// Send a deny for the client, and zero out the pending data
			MsgServerFailAuthentication_t msg;
			int64 outMessage;
			SteamGameServerNetworkingSockets()->SendMessageToConnection(m_rgPendingClientData[iPendingAuthIndex].m_hConn, &msg, sizeof(msg), k_nSteamNetworkingSend_Reliable, &outMessage);
			m_rgPendingClientData[iPendingAuthIndex] = ClientConnectionData_t();
			return;
		}

		bool bAddedOk = false;
		for (uint32 i = 0; i < MAX_PLAYERS_ON_SERVER_COUNT; ++i)
		{
			if (!m_rgClientData[i].m_bActive)
			{
				// copy over the data from the pending array
				memcpy(&m_rgClientData[i], &m_rgPendingClientData[iPendingAuthIndex], sizeof(ClientConnectionData_t));
				m_rgPendingClientData[iPendingAuthIndex] = ClientConnectionData_t();
				//m_rgClientData[i].m_ulTickCountLastData = m_pGameEngine->GetGameTickCount();

				// Add a new ship, make it dead immediately
				//AddPlayerShip(i);
				//m_rgpShips[i]->SetDisabled(true);

				MsgServerPassAuthentication_t msg;
				BSendDataToClient(i, (char*)&msg, sizeof(msg));

				bAddedOk = true;

				break;
			}
		}
	}
}