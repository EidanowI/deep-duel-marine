#pragma once
#include "Dependencies/steam/public/steam/steam_api.h"



#define MAX_LOBBY_MEMBERS 2

namespace DDMSteamWorksLib {
	class DDMLobby {
	public:
		__declspec(dllexport) DDMLobby();
		__declspec(dllexport) ~DDMLobby();

		__declspec(dllexport) void SetID(unsigned long long id);
		__declspec(dllexport) unsigned long long GetID();
		__declspec(dllexport) char* GetName();

		__declspec(dllexport) int GetLobbyMemberCount();

		__declspec(dllexport) void ClearLobbyData();
		__declspec(dllexport) void LeaveLobby();

	private:
		unsigned long long m_id = 0;
		char m_name[64]{};
	};


	enum GAME_STATE {
		GAME_STATE_IN_MENU,
		GAME_STATE_IN_LOBBY,
		GAME_STATE_CONNECTED
	};

	class DDMClient {
	public:
		DDMClient();
		~DDMClient();
		
		void Update();

		__declspec(dllexport) GAME_STATE GetGameState();

		__declspec(dllexport) void CreateLobby();
		__declspec(dllexport) void JoinLobby(CSteamID id);
		__declspec(dllexport) void LeaveLobby();

		__declspec(dllexport) void SetReadyOrNotStatus(bool isReady);

		__declspec(dllexport) bool IsOwnsLobby();

		__declspec(dllexport) DDMLobby* GetLobby();
		__declspec(dllexport) CSteamID GetID();

	public:
		__declspec(dllexport) void SetGameResultCalbacks(void (*pWin_func_calback)(), void (*pLose_func_claback)());

	public:
		__declspec(dllexport) bool SendNetworkData(const void* pData, unsigned int nSizeOfData, int nSendFlags);
	
	private:
		void ReceiveNetworkData();
		void OnReceiveServerInfo(CSteamID steamIDGameServer, bool bVACSecure, const char* pchServerName);
		void OnReceiveServerAuthenticationResponse(bool bSuccess);

	private:
		void InitiateServerConnection(CSteamID steamIDGameServer);

		void DisconnectFromServer();

	private:
		void OnLobbyCreated(LobbyCreated_t* pCallback, bool bIOFailure);
		CCallResult<DDMClient, LobbyCreated_t> m_steamCallResultLobbyCreated;

		void OnLobbyEntered(LobbyEnter_t* pCallback, bool bIOFailure);
		CCallResult<DDMClient, LobbyEnter_t> m_SteamCallResultLobbyEntered;

		STEAM_CALLBACK(DDMClient, OnLobbyGameCreated, LobbyGameCreated_t);

	private:
		CSteamID m_steamID;
		GAME_STATE m_gameState;

		DDMLobby m_lobby;

		HSteamNetConnection m_hConnServer = 0;

		void (*m_pWin_func_calback)() = 0;
		void (*m_pLose_func_claback)() = 0;

		CSteamID m_steamIDGameServer = CSteamID();
		uint32 m_unServerIP = 0;
		uint16 m_usServerPort = 0;
	};
}