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

    void Update() noexcept;

    void SetReadyOrNotStatus(bool isReady) noexcept;

    int GetLobbyMemberCount() noexcept;

    void CreateLobby() noexcept;
    void JoinLobby(CSteamID id) noexcept;
    void LeaveLobby() noexcept;


private:
    void OnLobbyCreated(LobbyCreated_t* pCallback, bool bIOFailure);
    CCallResult<GameClient, LobbyCreated_t> m_steamCallResultLobbyCreated;

    void OnLobbyEntered(LobbyEnter_t* pCallback, bool bIOFailure);
    CCallResult<GameClient, LobbyEnter_t> m_SteamCallResultLobbyEntered;
private:
    CSteamID m_client_steamID;
    EClientConnectionState m_client_connectionState;
    GAME_STATE m_client_gameState;

    Lobby m_lobby;
    bool is_client_owns_lobby;
    int m_lobby_members_count;
};