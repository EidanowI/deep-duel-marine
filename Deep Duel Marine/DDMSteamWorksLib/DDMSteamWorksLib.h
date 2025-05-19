#pragma once
#include <map>
#include <d3d11.h>

#include "Dependencies/steam/public/steam/steam_api.h"

#include "DDMClient.h"
#include "DDMLobbyBrowser.h"
#include "DDMAvatarTexture.h"



namespace DDMSteamWorksLib {
	class SWNetworkingManager {
	public:
		static __declspec(dllexport) void TryToConnect(ID3D11Device* pDevice);
		static __declspec(dllexport) void Update();
		static __declspec(dllexport) void Terminate();

	public:
		static __declspec(dllexport) bool IsConnectedToSteam();

		static __declspec(dllexport) DDMClient* GetDDMClient();
		static __declspec(dllexport) LobbyBrowser* GetLobbyBrowser();

	public:
		static __declspec(dllexport) AvatarTexture* GetSelfAvatarTex();
		static __declspec(dllexport) ID3D11ShaderResourceView* GetSelfAvatarResView();
		static __declspec(dllexport) AvatarTexture* GetAponentAvatarTex();
		static __declspec(dllexport) ID3D11ShaderResourceView* GetAponentAvatarResView();

	public:
		static __declspec(dllexport) bool GetSelfReadyOrNotStatus();
		static __declspec(dllexport) bool GetAponentReadyOrNotStatus();

	private:
		static __declspec(dllexport) AvatarTexture* LoadAvatar(CSteamID id);

	public:
		static __declspec(dllexport) const char* GetSelfNickname();
		static __declspec(dllexport) const char* GetAponentNickname();

	private:
		static __declspec(dllexport) const char* GetUserNickname(CSteamID id);

	private:
		static __declspec(dllexport) bool s_isConnected_to_steam;

	private:
		static __declspec(dllexport) DDMClient* s_pDDMClient;
		static __declspec(dllexport) LobbyBrowser* s_pLobbyBrowser;

	private:
		/*Need this shit cause my colostomy bag is domestic violent
		anyway don`t try to delete this shi cause it aggregated here*/
		static __declspec(dllexport) ID3D11Device* s_pDevice;
		static __declspec(dllexport) std::map<CSteamID, AvatarTexture*> s_avatarTextures_map;
		static __declspec(dllexport) AvatarTexture* s_pNoInetConn_avatarTex;
		static __declspec(dllexport) AvatarTexture* s_pLoadingAvatar_avatarTex;
	};
}