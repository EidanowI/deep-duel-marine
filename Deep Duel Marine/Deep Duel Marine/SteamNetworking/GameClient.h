#pragma once
#include "../Dependencies/steam/public/steam/steam_api.h"

#include "Lobby.h"

#define MAX_LOBBY_MEMBERS 2

enum EClientConnectionState
{
    k_EClientNotConnected,							// Initial state, not connected to a server
    k_EClientConnectedPendingAuthentication,		// We've established communication with the server, but it hasn't authed us yet
    k_EClientConnectedAndAuthenticated,				// Final phase, server has authed us, we are actually able to play on it
};

enum GAME_STATE {
    GAME_STATE_IN_MENU,
    GAME_STATE_IN_LOBBY,
    GAME_STATE_CONNECTING,
    GAME_STATE_CONNECTED
};

class GameClient {
    friend class SteamNetworkingManager;
public:
    GameClient() noexcept;
    ~GameClient() noexcept;

    void CreateLobby() noexcept;


private:
    void OnLobbyCreated(LobbyCreated_t* pCallback, bool bIOFailure);
    CCallResult<GameClient, LobbyCreated_t> m_steamCallResultLobbyCreated;

private:
    CSteamID m_client_steamID;
    EClientConnectionState m_client_connectionState;
    GAME_STATE m_client_gameState;

    Lobby m_lobby;
};