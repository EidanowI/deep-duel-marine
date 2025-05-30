#include "DDMServer.h"
#include "DDMMessages.h"
#include <assert.h>
#include <ctime>



namespace DDMSteamWorksLib {
	bool ValidateShipPositing(MsgShip* ships) {
		for (int i = 0; i < 10; i++) {
			for(int j = 0; j < 10; j++) {
				if (j == i) continue;


				for (int k = 0; k < 4; k++) {
					int x = ships[j].cells[k].x;
					int y = ships[j].cells[k].y;

					if (ships[i].cells[k].x == -1 || ships[i].cells[k].y == -1 ||
						ships[j].cells[k].x == -1 || ships[j].cells[k].y == -1) continue;

					/*
						#0 #1 #2
						#3 #4 #5
						#6 #7 #8
					*/

					if (ships[i].cells[k].x - 1 == x && ships[i].cells[k].y - 1 == y) return false;//#0
					if (ships[i].cells[k].x == x && ships[i].cells[k].y - 1 == y) return false;//#1
					if (ships[i].cells[k].x + 1 == x && ships[i].cells[k].y - 1 == y) return false;//#2
					if (ships[i].cells[k].x - 1 == x && ships[i].cells[k].y == y) return false;//#3
					if (ships[i].cells[k].x == x && ships[i].cells[k].y == y) return false;//#4
					if (ships[i].cells[k].x + 1 == x && ships[i].cells[k].y == y) return false;//#5
					if (ships[i].cells[k].x - 1 == x && ships[i].cells[k].y + 1 == y) return false;//#6
					if (ships[i].cells[k].x == x && ships[i].cells[k].y + 1 == y) return false;//#7
					if (ships[i].cells[k].x + 1 == x && ships[i].cells[k].y + 1 == y) return false;//#8
				}
			}
		}
		
		return true;
	}


	void UnitTest() {
		/*
		0 0 1 1 0 0 0 0 0 0
		0 1 0 0 0 0 0 0 0 0
		0 1 0 0 0 0 0 0 0 0
		0 1 0 0 0 0 0 0 0 0
		0 1 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0 --- bad
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0
		*/

		MsgShip test1[10];
		test1[0].cells[0].x = 1;
		test1[0].cells[0].y = 1;
		test1[0].cells[1].x = 1;
		test1[0].cells[1].y = 2;
		test1[0].cells[2].x = 1;
		test1[0].cells[2].y = 3;
		test1[0].cells[3].x = 1;
		test1[0].cells[3].y = 4;

		test1[1].cells[0].x = 2;
		test1[1].cells[0].y = 0;
		test1[1].cells[1].x = 3;
		test1[1].cells[1].y = 0;

		assert(ValidateShipPositing(test1) == false);

		/*
		0 0 0 1 1 0 0 0 0 0
		0 1 0 0 0 0 0 0 0 0
		0 1 0 0 0 0 0 0 0 0
		0 1 0 0 0 0 0 0 0 0
		0 1 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0 --- good
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0
		*/

		MsgShip test2[10];
		test2[0].cells[0].x = 1;
		test2[0].cells[0].y = 1;
		test2[0].cells[1].x = 1;
		test2[0].cells[1].y = 2;
		test2[0].cells[2].x = 1;
		test2[0].cells[2].y = 3;
		test2[0].cells[3].x = 1;
		test2[0].cells[3].y = 4;

		test2[1].cells[0].x = 3;
		test2[1].cells[0].y = 0;
		test2[1].cells[1].x = 4;
		test2[1].cells[1].y = 0;

		assert(ValidateShipPositing(test2) == true);

		/*
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0 --- bad
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 1 0
		0 0 0 0 0 0 0 0 0 1
		*/

		MsgShip test3[10];
		test3[0].cells[0].x = 8;
		test3[0].cells[0].y = 8;
		

		test3[1].cells[0].x = 9;
		test3[1].cells[0].y = 9;


		assert(ValidateShipPositing(test3) == false);

		/*
		0 0 0 0 0 0 0 0 0 1
		0 0 0 0 0 0 0 0 1 0
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0 --- bad
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0
		*/

		MsgShip test4[10];
		test4[0].cells[0].x = 9;
		test4[0].cells[0].y = 0;


		test4[1].cells[0].x = 8;
		test4[1].cells[0].y = 1;


		assert(ValidateShipPositing(test4) == false);

		/*
		0 0 0 0 0 0 0 0 0 1
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 1 0
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0 --- good
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0
		*/

		MsgShip test5[10];
		test5[0].cells[0].x = 9;
		test5[0].cells[0].y = 0;


		test5[1].cells[0].x = 8;
		test5[1].cells[0].y = 2;


		assert(ValidateShipPositing(test5) == true);

	}


	DDMServer::DDMServer() : m_isConnected_to_steam(false), m_connectedPlayers_count(0), m_first_player_set_ships(false), m_second_player_set_ships(false){
		UnitTest();

		srand(std::time(0));
		turn_counter = rand() % 100;

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
			case k_EMsgClientReadyAndGiveShips:
			{
				if (message->GetSize() != sizeof(MsgClientReadyAndGiveShips_t))
				{
					message->Release();
					message = nullptr;
					continue;
				}
				MsgClientReadyAndGiveShips_t* pMsg = (MsgClientReadyAndGiveShips_t*)message->GetData();

				if (ValidateShipPositing(pMsg->ships)) {
					for (int i = 0; i < 2; i++) {
						if (steamIDRemote == m_rgClientData[i].m_SteamIDUser) {
							if (i == 0) {
								m_first_player_set_ships = true;

								for (int ship = 0; ship < 10; ship++) {
									for (int k = 0; k < 4; k++) {
										if(pMsg->ships[ship].cells[k].y != -1 && pMsg->ships[ship].cells[k].x != -1)
											m_first_player_cells[10 * pMsg->ships[ship].cells[k].y + pMsg->ships[ship].cells[k].x] = ALIVE;
									}
								}
							}
							else {
								m_second_player_set_ships = true;

								for (int ship = 0; ship < 10; ship++) {
									for (int k = 0; k < 4; k++) {
										if (pMsg->ships[ship].cells[k].y != -1 && pMsg->ships[ship].cells[k].x != -1)
											m_seccond_player_cells[10 * pMsg->ships[ship].cells[k].y + pMsg->ships[ship].cells[k].x] = ALIVE;
									}
								}
							}
						}			
					}

					if (m_first_player_set_ships && m_second_player_set_ships) {
						MsgTwoClientsAreReadySoStart_t msg = MsgTwoClientsAreReadySoStart_t();
						BSendDataToClient(1, (char*)&msg, sizeof(MsgTwoClientsAreReadySoStart_t));
						BSendDataToClient(0, (char*)&msg, sizeof(MsgTwoClientsAreReadySoStart_t));
						
						

						MsgTurn_t turn_msg = MsgTurn_t();
						if (turn_counter % 2 == 1) {
							BSendDataToClient(1, (char*)&turn_msg, sizeof(MsgTurn_t));
						}
						else {
							BSendDataToClient(0, (char*)&turn_msg, sizeof(MsgTurn_t));
						}
					}
				}
			}
			break;
			case k_EMsgShot:
			{
				if (message->GetSize() != sizeof(MsgShot_t))
				{
					message->Release();
					message = nullptr;
					continue;
				}
				MsgShot_t* pMsg = (MsgShot_t*)message->GetData();

				for (int i = 0; i < 2; i++) {///TODO add turn validation
					if (steamIDRemote == m_rgClientData[i].m_SteamIDUser) {
						if (i == 1) {	
							if (m_first_player_cells[pMsg->m_y * 10 + pMsg->m_x] == ALIVE) {
								m_first_player_cells[pMsg->m_y * 10 + pMsg->m_x] = DEAD;

								MsgShotResult_t msg_res_shot = MsgShotResult_t();
								msg_res_shot.m_x = pMsg->m_x;
								msg_res_shot.m_y = pMsg->m_y;
								msg_res_shot.is_dead = true;

								BSendDataToClient(1, (char*)&msg_res_shot, sizeof(MsgShotResult_t));


								MsgGotDead_t dead_msg = MsgGotDead_t();
								dead_msg.m_x = pMsg->m_x;
								dead_msg.m_y = pMsg->m_y;

								BSendDataToClient(0, (char*)&dead_msg, sizeof(MsgGotDead_t));
							}
							else {
								turn_counter++;

								MsgShotResult_t msg_res_shot = MsgShotResult_t();
								msg_res_shot.m_x = pMsg->m_x;
								msg_res_shot.m_y = pMsg->m_y;
								msg_res_shot.is_dead = false;

								BSendDataToClient(1, (char*)&msg_res_shot, sizeof(MsgShotResult_t));

								MsgTurn_t turn_msg = MsgTurn_t();
								BSendDataToClient(0, (char*)&turn_msg, sizeof(MsgTurn_t));
							}
						}
						else {
							if (m_seccond_player_cells[pMsg->m_y * 10 + pMsg->m_x] == ALIVE) {
								m_seccond_player_cells[pMsg->m_y * 10 + pMsg->m_x] = DEAD;

								MsgShotResult_t msg_res_shot = MsgShotResult_t();
								msg_res_shot.m_x = pMsg->m_x;
								msg_res_shot.m_y = pMsg->m_y;
								msg_res_shot.is_dead = true;

								BSendDataToClient(0, (char*)&msg_res_shot, sizeof(MsgShotResult_t));

								MsgGotDead_t dead_msg = MsgGotDead_t();
								dead_msg.m_x = pMsg->m_x;
								dead_msg.m_y = pMsg->m_y;

								BSendDataToClient(1, (char*)&dead_msg, sizeof(MsgGotDead_t));
							}
							else {
								turn_counter++;

								MsgShotResult_t msg_res_shot = MsgShotResult_t();
								msg_res_shot.m_x = pMsg->m_x;
								msg_res_shot.m_y = pMsg->m_y;
								msg_res_shot.is_dead = false;

								BSendDataToClient(0, (char*)&msg_res_shot, sizeof(MsgShotResult_t));

								MsgTurn_t turn_msg = MsgTurn_t();
								BSendDataToClient(1, (char*)&turn_msg, sizeof(MsgTurn_t));
							}
						}
					}
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