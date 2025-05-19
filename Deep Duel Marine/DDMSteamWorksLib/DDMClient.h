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


	enum EClientConnectionState
	{
		k_EClientNotConnected,
		k_EClientConnected
	};

	enum GAME_STATE {
		GAME_STATE_IN_MENU,
		GAME_STATE_IN_LOBBY,
		GAME_STATE_CONNECTING,
		GAME_STATE_CONNECTED
	};

	class DDMClient {
	public:
		DDMClient();
		~DDMClient();
		
		void Update();

		__declspec(dllexport) void CreateLobby();
		__declspec(dllexport) void JoinLobby(CSteamID id);
		__declspec(dllexport) void LeaveLobby();

		__declspec(dllexport) void SetReadyOrNotStatus(bool isReady);

		__declspec(dllexport) bool IsOwnsLobby();

		__declspec(dllexport) DDMLobby* GetLobby();

	private:
		void OnLobbyCreated(LobbyCreated_t* pCallback, bool bIOFailure);
		CCallResult<DDMClient, LobbyCreated_t> m_steamCallResultLobbyCreated;

		void OnLobbyEntered(LobbyEnter_t* pCallback, bool bIOFailure);
		CCallResult<DDMClient, LobbyEnter_t> m_SteamCallResultLobbyEntered;

	private:
		CSteamID m_steamID;
		EClientConnectionState m_connectionState;
		GAME_STATE m_gameState;

		DDMLobby m_lobby;
	};
}