#include "SteamNetworking.h"
#include "../Graphics/Renderer/Renderer.h"


extern bool G_isShould_close_window;
bool SteamNetworkingManager::s_isConnected_to_steam = false;
std::map<CSteamID, ID3D11ShaderResourceView*> SteamNetworkingManager::s_avatars{};

unsigned char* SteamNetworkingManager::s_pDefault_avatar_image = nullptr;

//Lobby SteamNetworkingManager::s_lobby{};
GameClient* SteamNetworkingManager::s_pGame_client = nullptr;
LobbyBrowser* SteamNetworkingManager::s_pLobbyBrowser = nullptr;


void SteamNetworkingManager::TryToConnect() noexcept {
	if (s_isConnected_to_steam) return;

	s_pDefault_avatar_image = new unsigned char[DEFAULT_AVATAR_IMAGE_WIDTH_HEIGHT * DEFAULT_AVATAR_IMAGE_WIDTH_HEIGHT * 4];
	for (int i = 0; i < DEFAULT_AVATAR_IMAGE_WIDTH_HEIGHT * DEFAULT_AVATAR_IMAGE_WIDTH_HEIGHT; i++) {
		s_pDefault_avatar_image[i * 4] = 255;
		s_pDefault_avatar_image[i * 4 + 1] = 0;
		s_pDefault_avatar_image[i * 4 + 2] = 0;
		s_pDefault_avatar_image[i * 4 + 3] = 255;
	}

	s_avatars[{}] = Renderer::LoadImageToGPU(s_pDefault_avatar_image, DEFAULT_AVATAR_IMAGE_WIDTH_HEIGHT, DEFAULT_AVATAR_IMAGE_WIDTH_HEIGHT);

	SteamErrMsg errMsg = { 0 };
	if (SteamAPI_InitEx(&errMsg) != k_ESteamAPIInitResult_OK)
	{
		s_isConnected_to_steam = false;
		MessageBoxA(0, "SteamAPI_Init() failed", errMsg, 0);
		return;
	}

	if (!SteamUser()->BLoggedOn())
	{
		s_isConnected_to_steam = false;
		return;
	}

	SteamNetworkingUtils()->InitRelayNetworkAccess();

	s_isConnected_to_steam = true;

	s_pGame_client = new GameClient();
	s_pLobbyBrowser = new LobbyBrowser();
}
void SteamNetworkingManager::Terminate() noexcept {
	delete s_pLobbyBrowser;
	delete s_pGame_client;
	delete[] s_pDefault_avatar_image;

	for (const auto& [id, shaderView] : s_avatars) {
		shaderView->Release();
	}

	SteamAPI_Shutdown();
}
void SteamNetworkingManager::Update() noexcept {
	if (!s_isConnected_to_steam) return;

	SteamAPI_RunCallbacks();

	s_pGame_client->Update();
}

void SteamNetworkingManager::ClearLobbyStructData() noexcept {
	if (!s_pGame_client || !SteamNetworkingManager::IsSteamConnected()) return;
	if (s_pGame_client->m_lobby.m_id != 0) return;

	s_pGame_client->m_lobby.m_isPublic = true;
	memset(s_pGame_client->m_lobby.m_name, 0, 64);
	strncpy(s_pGame_client->m_lobby.m_name, SteamNetworkingManager::GetSelfUserNickName(), 63);

	if (strlen(s_pGame_client->m_lobby.m_name) + 7 < 63) {
		memcpy(s_pGame_client->m_lobby.m_name + strlen(s_pGame_client->m_lobby.m_name), " lobby", 7);
	}
}
Lobby* SteamNetworkingManager::GetLobbyStruct() noexcept {
	return &s_pGame_client->m_lobby;
}

LobbyBrowser* SteamNetworkingManager::GetLobbyBrowser() noexcept {
	return s_pLobbyBrowser;
}

int SteamNetworkingManager::GetLobbyMemberCount() noexcept {
	return s_pGame_client->GetLobbyMemberCount();
}
bool SteamNetworkingManager::IsThisClientLobbyOwner() noexcept {
	return s_pGame_client->is_client_owns_lobby;
}

void SteamNetworkingManager::SetReadyOrNotStatus(bool isReady) noexcept {
	//SteamMatchmaking()->GetLobbyMemberData(m_steamIDLobby, SteamUser()->GetSteamID(), "ready")

	s_pGame_client->SetReadyOrNotStatus(isReady);
}
bool SteamNetworkingManager::GetAponentReadyOrNotStatus() noexcept {
	//if (s_pGame_client->m_lobby_members_count != 2) return false;

	for (int i = 0; i < SteamMatchmaking()->GetNumLobbyMembers(s_pGame_client->m_lobby.m_id); i++) {
		auto sID = SteamMatchmaking()->GetLobbyMemberByIndex(s_pGame_client->m_lobby.m_id, i);
		if (sID != SteamUser()->GetSteamID()) {
			//MessageBoxA(0, "A", SteamMatchmaking()->GetLobbyMemberData(s_pGame_client->m_lobby.m_id, sID, "ready"), 0);
			//OutputDebugStringA(SteamMatchmaking()->GetLobbyMemberData(s_pGame_client->m_lobby.m_id, sID, "ready")[0] == '1');
			return SteamMatchmaking()->GetLobbyMemberData(s_pGame_client->m_lobby.m_id, sID, "ready")[0] == '1';
		}
	}
}

bool SteamNetworkingManager::StartServerRetarder(bool isBoth_plaers_ready, int& seconds_remaining) noexcept {
	static bool is_sync_both_ready = false;
	static int ssecs = 0;

	if (isBoth_plaers_ready && !is_sync_both_ready) {
		is_sync_both_ready = true;
		ssecs = 6;
	}
	if (!isBoth_plaers_ready) {
		ssecs = 6;
		is_sync_both_ready = false;
		return false;
	}

	static int now_time = 0;
	if (now_time != std::time(0)) {
		now_time = std::time(0);
		ssecs--;
	}

	seconds_remaining = ssecs;

	if (ssecs < 0) {
		//MessageBoxA(0, "Bruh", "Server should start init now", 0);
		///TODO starting server logic
		return true;
	}
	return false;
}

void SteamNetworkingManager::CreateLobby() noexcept {
	s_pGame_client->CreateLobby();
}
void SteamNetworkingManager::JoinLobby(CSteamID id) noexcept {
	s_pGame_client->JoinLobby(id);
}
void SteamNetworkingManager::LeaveLobby() noexcept {
	s_pGame_client->LeaveLobby();
}

void* SteamNetworkingManager::LoadSelfAvatar() noexcept {
	if (s_isConnected_to_steam) {
		return LoadAvatar(SteamUser()->GetSteamID());
	}
	else {
		return s_avatars[{}];
	}
}
void* SteamNetworkingManager::LoadAponentAvatar() noexcept {
	if (!s_pGame_client || s_pGame_client->m_lobby.m_id == 0) return s_avatars[{}];

	for (int i = 0; i < SteamMatchmaking()->GetNumLobbyMembers(s_pGame_client->m_lobby.m_id); i++) {
		auto sID = SteamMatchmaking()->GetLobbyMemberByIndex(s_pGame_client->m_lobby.m_id, i);
		if (sID != SteamUser()->GetSteamID()) {
			return LoadAvatar(sID);
		}
	}

	return s_avatars[{}];
}
void* SteamNetworkingManager::LoadAvatar(CSteamID id) noexcept {
	if (s_isConnected_to_steam) {
		if (s_avatars.contains(id)) {
			return s_avatars[id];
		}
		else {
			int avatarId = SteamFriends()->GetLargeFriendAvatar(id);
			if (avatarId) {
				unsigned int width;
				unsigned int height;
				SteamUtils()->GetImageSize(avatarId, &width, &height);
				unsigned char* imageBuffer = new unsigned char[width * height * 4] {};
				SteamUtils()->GetImageRGBA(avatarId, imageBuffer, width * height * 4);
			
				s_avatars[id] = Renderer::LoadImageToGPU(imageBuffer, width, height);

				delete[] imageBuffer;

				return s_avatars[id];
			}
			else {
				return s_avatars[{}];
			}
		}
	}
	else {
		return s_avatars[{}];
	}

}

const char* SteamNetworkingManager::GetSelfUserNickName() noexcept {
	if (s_isConnected_to_steam) {
		return SteamFriends()->GetPersonaName();
	}
	else {
		return "Unknown";
	}
}
const char* SteamNetworkingManager::GetAponentUserNickName() noexcept {
	if (!s_pGame_client || s_pGame_client->m_lobby.m_id == 0) return "Unknown player";

	for (int i = 0; i < SteamMatchmaking()->GetNumLobbyMembers(s_pGame_client->m_lobby.m_id); i++) {
		auto sID = SteamMatchmaking()->GetLobbyMemberByIndex(s_pGame_client->m_lobby.m_id, i);
		if (sID != SteamUser()->GetSteamID()) {
			return GetUserNickName(sID);
		}
	}

	return "Unknown player";
}
const char* SteamNetworkingManager::GetUserNickName(CSteamID id) noexcept {
	if (s_isConnected_to_steam) {
		return SteamFriends()->GetFriendPersonaName(id);
	}
	else {
		return "Unknown player";
	}
}