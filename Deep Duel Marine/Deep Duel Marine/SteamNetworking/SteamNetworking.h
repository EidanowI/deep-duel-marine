#pragma once
#include "d3d11.h"
#include "Windows.h"
#include "../Dependencies/steam/public/steam/steam_api.h"
#include <map>
#include <string>

#include "Lobby.h"
#include "GameClient.h"
#include "LobbyBrowser.h"



class SteamNetworkingManager {
public:
	static void TryToConnect() noexcept;
	static void Terminate() noexcept;
	static void Update() noexcept;

	static bool IsSteamConnected() noexcept { return s_isConnected_to_steam; }

	static void ClearLobbyStructData() noexcept;
	static Lobby* GetLobbyStruct() noexcept;

	static LobbyBrowser* GetLobbyBrowser() noexcept;

	static void CreateLobby() noexcept;

	static void* LoadSelfAvatar() noexcept;
	static void* LoadAvatar(CSteamID id) noexcept;

	static const char* GetSelfUserNickName() noexcept;
	static const char* GetUserNickName(CSteamID id) noexcept;

private:
	static bool s_isConnected_to_steam;
	static std::map<CSteamID, ID3D11ShaderResourceView*> s_avatars;

	static unsigned char* s_pDefault_avatar_image;
#define DEFAULT_AVATAR_IMAGE_WIDTH_HEIGHT 184

	//static Lobby s_lobby;
	static GameClient* s_pGame_client;
	static LobbyBrowser* s_pLobbyBrowser;
};