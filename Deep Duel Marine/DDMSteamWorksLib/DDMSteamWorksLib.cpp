#include <Windows.h>

#include "DDMSteamWorksLib.h"



namespace DDMSteamWorksLib {
	bool SWNetworkingManager::s_isConnected_to_steam = false;

	DDMClient* SWNetworkingManager::s_pDDMClient = nullptr;
	DDMLobbyBrowser* SWNetworkingManager::s_pLobbyBrowser = nullptr;
	DDMServer* SWNetworkingManager::s_pServer = nullptr;

	ID3D11Device* SWNetworkingManager::s_pDevice = nullptr;
	std::map<CSteamID, AvatarTexture*> SWNetworkingManager::s_avatarTextures_map{};
	AvatarTexture* SWNetworkingManager::s_pNoInetConn_avatarTex = nullptr;
	AvatarTexture* SWNetworkingManager::s_pLoadingAvatar_avatarTex = nullptr;

	void SWNetworkingManager::TryToConnect(ID3D11Device* pDevice) {
		if (s_isConnected_to_steam) return;

		s_pDevice = pDevice;
		s_pNoInetConn_avatarTex = new AvatarTexture("AvatarNoConnection.png", s_pDevice);
		s_pLoadingAvatar_avatarTex = new AvatarTexture("AvatarLoading.png", s_pDevice);

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

		s_pDDMClient = new DDMClient();
		s_pLobbyBrowser = new DDMLobbyBrowser();
	}
	void SWNetworkingManager::Terminate() {
		delete s_pLobbyBrowser;
		delete s_pDDMClient;

		if (s_pServer) {
			delete s_pServer;
		}

		for (const auto& [id, pTex] : s_avatarTextures_map) {
			delete pTex;
		}
		delete s_pNoInetConn_avatarTex;
		delete s_pLoadingAvatar_avatarTex;

		SteamAPI_Shutdown();
	}
	void SWNetworkingManager::Update() {
		if (!s_isConnected_to_steam) return;

		SteamAPI_RunCallbacks();

		s_pDDMClient->Update();

		if (s_pServer) {
			s_pServer->Update();

			if (s_pServer->IsServerReadyToShut()) {
				SWNetworkingManager::TerminateServer();
			}
		}
	}

	void SWNetworkingManager::InitServer() {
		s_pServer = new DDMServer();
	}
	void SWNetworkingManager::TerminateServer() {
		if (s_pServer) {
			delete s_pServer;
			s_pServer = nullptr;
		}
	}

	bool SWNetworkingManager::IsConnectedToSteam() {
		return s_isConnected_to_steam;
	}

	DDMClient* SWNetworkingManager::GetDDMClient() {
		return s_pDDMClient;
	}
	DDMLobbyBrowser* SWNetworkingManager::GetLobbyBrowser() {
		return s_pLobbyBrowser;
	}
	DDMServer* SWNetworkingManager::GetServer() {
		return s_pServer;
	}

	AvatarTexture* SWNetworkingManager::GetSelfAvatarTex() {
		if (s_isConnected_to_steam) {
			return LoadAvatar(SteamUser()->GetSteamID());
		}
		else {
			return s_pNoInetConn_avatarTex;
		}
	}
	ID3D11ShaderResourceView* SWNetworkingManager::GetSelfAvatarResView() {
		auto pTex = GetSelfAvatarTex();
		if (pTex) return pTex->GetShaderResView();
		return nullptr;
	}
	AvatarTexture* SWNetworkingManager::GetAponentAvatarTex() {
		for (int i = 0; i < SteamMatchmaking()->GetNumLobbyMembers(GetDDMClient()->GetLobby()->GetID()); i++) {
			auto sID = SteamMatchmaking()->GetLobbyMemberByIndex(GetDDMClient()->GetLobby()->GetID(), i);
			if (sID != SteamUser()->GetSteamID()) {
				return LoadAvatar(sID);
			}
		}

		return nullptr;
	}
	ID3D11ShaderResourceView* SWNetworkingManager::GetAponentAvatarResView() {
		auto pTex = GetAponentAvatarTex();
		if (pTex) return pTex->GetShaderResView();
		return nullptr;
	}

	AvatarTexture* SWNetworkingManager::LoadAvatar(CSteamID id) {
		if (s_isConnected_to_steam) {
			if (s_avatarTextures_map.contains(id)) {
				if (s_avatarTextures_map[id] == s_pLoadingAvatar_avatarTex) {
					AvatarTexture* pTex = nullptr;

					int avatarId = SteamFriends()->GetLargeFriendAvatar(id);
					if (avatarId > 0) {
						unsigned int width;
						unsigned int height;
						SteamUtils()->GetImageSize(avatarId, &width, &height);
						unsigned char* imageBuffer = new unsigned char[width * height * 4] {};
						SteamUtils()->GetImageRGBA(avatarId, imageBuffer, width * height * 4);

						pTex = new AvatarTexture(width, height, imageBuffer, s_pDevice);

						delete[] imageBuffer;

						s_avatarTextures_map[id] = pTex;
					}
				}

				return s_avatarTextures_map[id];
			}
			else
			{
				AvatarTexture* pTex = nullptr;

				int avatarId = SteamFriends()->GetLargeFriendAvatar(id);
				if (avatarId > 0) {
					unsigned int width;
					unsigned int height;
					SteamUtils()->GetImageSize(avatarId, &width, &height);
					unsigned char* imageBuffer = new unsigned char[width * height * 4] {};
					SteamUtils()->GetImageRGBA(avatarId, imageBuffer, width * height * 4);

					pTex = new AvatarTexture(width, height, imageBuffer, s_pDevice);

					delete[] imageBuffer;
				}
				else {
					pTex = s_pLoadingAvatar_avatarTex;
				}

				s_avatarTextures_map[id] = pTex;

				return pTex;
			}
		}
		else
		{
			return s_pNoInetConn_avatarTex;
		}

	}

	const char* SWNetworkingManager::GetSelfNickname() {
		if (s_isConnected_to_steam) return SteamFriends()->GetPersonaName();
		return "";
	}
	const char* SWNetworkingManager::GetAponentNickname() {
		for (int i = 0; i < SteamMatchmaking()->GetNumLobbyMembers(GetDDMClient()->GetLobby()->GetID()); i++) {
			auto sID = SteamMatchmaking()->GetLobbyMemberByIndex(GetDDMClient()->GetLobby()->GetID(), i);
			if (sID != SteamUser()->GetSteamID()) {
				return GetUserNickname(sID);
			}
		}

		return "";
	}

	const char* SWNetworkingManager::GetUserNickname(CSteamID id) {
		if (s_isConnected_to_steam) return SteamFriends()->GetFriendPersonaName(id);
		return "";
	}

	bool SWNetworkingManager::GetSelfReadyOrNotStatus() {
		if (!s_isConnected_to_steam) return false;

		return SteamMatchmaking()->GetLobbyMemberData(GetDDMClient()->GetLobby()->GetID(), SteamUser()->GetSteamID(), "ready")[0] == '1';
	}
	bool SWNetworkingManager::GetAponentReadyOrNotStatus() {
		if (!s_isConnected_to_steam) return false;

		for (int i = 0; i < SteamMatchmaking()->GetNumLobbyMembers(GetDDMClient()->GetLobby()->GetID()); i++) {
			auto sID = SteamMatchmaking()->GetLobbyMemberByIndex(GetDDMClient()->GetLobby()->GetID(), i);
			if (sID != SteamUser()->GetSteamID()) {
				auto b = SteamMatchmaking()->GetLobbyMemberData(GetDDMClient()->GetLobby()->GetID(), sID, "ready");
				if (!b) return false;
				return SteamMatchmaking()->GetLobbyMemberData(GetDDMClient()->GetLobby()->GetID(), sID, "ready")[0] == '1';
			}
		}

		return false;
	}
}