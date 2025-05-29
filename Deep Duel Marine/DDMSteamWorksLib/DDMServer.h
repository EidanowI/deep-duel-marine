#pragma once
#include <Windows.h>

#include "Dependencies/steam/public/steam/steam_api.h"
#include "Dependencies/steam/public/steam/steam_gameserver.h"
#include "DDMMessages.h"



#define MAX_PLAYERS_ON_SERVER_COUNT 2

///server init
#define unIP INADDR_ANY 
// Current game server version
#define SERVER_VERSION "1.0.0.0"
// UDP port for the spacewar server to listen on
#define SERVER_PORT 27015
// UDP port for the master server updater to listen on
#define MASTER_SERVER_UPDATER_PORT 27016
#define SERVER_MODE eServerModeAuthenticationAndSecure


///params
#define GAMEDIR "testserver"


namespace DDMSteamWorksLib {
	enum SERVER_GAME_STATE {
		ACTIVE,
		SUMMARIZING
	};

	struct ClientConnectionData_t
	{
		bool m_bActive;					// Is this slot in use? Or is it available for new connections?
		CSteamID m_SteamIDUser;			// What is the steamid of the player?
		HSteamNetConnection m_hConn;	// The handle for the connection to the player

		ClientConnectionData_t() {
			m_bActive = false;
			m_hConn = 0;
			m_SteamIDUser = {};
		}
	};

	enum EDisconnectReason
	{
		k_EDRClientDisconnect = k_ESteamNetConnectionEnd_App_Min + 1,
		k_EDRServerClosed = k_ESteamNetConnectionEnd_App_Min + 2,
		k_EDRServerReject = k_ESteamNetConnectionEnd_App_Min + 3,
		k_EDRServerFull = k_ESteamNetConnectionEnd_App_Min + 4,
		k_EDRClientKicked = k_ESteamNetConnectionEnd_App_Min + 5
	};

	bool ValidateShipPositing(MsgShip* ships);
	void UnitTest();

	enum SHIP_CELL_STATE {
		EMPTY,
		ALIVE,
		DEAD
	};

	class DDMServer {
	public:
		DDMServer();
		~DDMServer();

		__declspec(dllexport) void Update();

	public:
		__declspec(dllexport) bool IsConnectedToSteam();
		__declspec(dllexport) CSteamID GetSteamID();

		__declspec(dllexport) bool IsServerReadyToShut();

	private:
		void ReceiveNetworkData();
		bool BSendDataToClient(int index, char* pData, uint32 nSizeOfData);

		void OnClientBeginAuthentication(CSteamID steamIDClient, HSteamNetConnection connectionID, void* pToken, uint32 uTokenLen);
		void OnAuthCompleted(bool bAuthSuccessful, uint32 iPendingAuthIndex);

	private:
		void RemovePlayerFromServer(int index, EDisconnectReason reason);

	private:
		STEAM_GAMESERVER_CALLBACK(DDMServer, OnSteamServersConnected, SteamServersConnected_t);
		STEAM_GAMESERVER_CALLBACK(DDMServer, OnNetConnectionStatusChanged, SteamNetConnectionStatusChangedCallback_t);

	private:
		bool m_isConnected_to_steam = false;
		int m_connectedPlayers_count = 0;

		SERVER_GAME_STATE m_gameState;

		// Socket to listen for new connections on 
		HSteamListenSocket m_hListenSocket;

		// Vector to keep track of client connections
		ClientConnectionData_t m_rgClientData[MAX_PLAYERS_ON_SERVER_COUNT]{};
		ClientConnectionData_t m_rgPendingClientData[MAX_PLAYERS_ON_SERVER_COUNT]{};

		//Poll group used to receive messages from all clients at once
		HSteamNetPollGroup m_hNetPollGroup;

		SHIP_CELL_STATE m_first_player_cells[100]{};
		bool m_first_player_set_ships = false;

		SHIP_CELL_STATE m_seccond_player_cells[100]{};
		bool m_second_player_set_ships = false;

		unsigned int turn_counter;
	};
}