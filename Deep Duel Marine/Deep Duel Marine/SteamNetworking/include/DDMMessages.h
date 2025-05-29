#pragma once



enum EMessage
{
	// Server messages
	k_EMsgServerBegin = 0,

	k_EMsgClientWin = k_EMsgServerBegin + 1,
	k_EMsgClientLose = k_EMsgServerBegin + 2,

	k_EMsgRequestWin = k_EMsgServerBegin + 3,
	k_EMsgRequestLose = k_EMsgServerBegin + 4,

	k_EMsgServerSendInfo = k_EMsgServerBegin + 5,
	k_EMsgClientBeginAuth = k_EMsgServerBegin + 6,
	k_EMsgClientPassAuth = k_EMsgServerBegin + 7,
	k_EMsgClientFailAuth = k_EMsgServerBegin + 8,

	k_EMsgClientReadyAndGiveShips = k_EMsgServerBegin + 9,
	k_EMsgTwoClientsReadySoStart = k_EMsgServerBegin + 10,
	//k_EMsgServerSendInfo = k_EMsgServerBegin + 1,
	//k_EMsgServerFailAuthentication = k_EMsgServerBegin + 2,
	//k_EMsgServerPassAuthentication = k_EMsgServerBegin + 3,
	//k_EMsgServerUpdateWorld = k_EMsgServerBegin + 4,
	//k_EMsgServerExiting = k_EMsgServerBegin + 5,
	//k_EMsgServerPingResponse = k_EMsgServerBegin + 6,
	//k_EMsgServerPlayerHitSun = k_EMsgServerBegin + 7,

	//// Client messages
	//k_EMsgClientBegin = 500,
	//k_EMsgClientBeginAuthentication = k_EMsgClientBegin + 2,
	//k_EMsgClientSendLocalUpdate = k_EMsgClientBegin + 3,

	//// P2P authentication messages
	//k_EMsgP2PBegin = 600,
	//k_EMsgP2PSendingTicket = k_EMsgP2PBegin + 1,

	//// voice chat messages
	//k_EMsgVoiceChatBegin = 700,
	////k_EMsgVoiceChatPing = k_EMsgVoiceChatBegin+1,	// deprecated keep alive message
	//k_EMsgVoiceChatData = k_EMsgVoiceChatBegin + 2,	// voice data from another player



	// force 32-bit size enum so the wire protocol doesn't get outgrown later
	k_EForceDWORD = 0x7fffffff,
};

class BaseMessage {
public:
	__declspec(dllexport) BaseMessage(int message_type) : m_message_type(message_type) {

	}
	__declspec(dllexport) int GetMessageType() { return m_message_type; }
private:
	const int m_message_type;
};

class MsgServerSendWinToClient_t : public BaseMessage
{
public:
	__declspec(dllexport) MsgServerSendWinToClient_t() : BaseMessage(k_EMsgClientWin) {}
};

class MsgServerSendLoseToClient_t : public BaseMessage
{
public:
	__declspec(dllexport) MsgServerSendLoseToClient_t() : BaseMessage(k_EMsgClientLose) {}
};

class MsgClientRequestWin_t : public BaseMessage
{
public:
	__declspec(dllexport) MsgClientRequestWin_t() : BaseMessage(k_EMsgRequestWin) {}
};

class MsgClientRequestLose_t : public BaseMessage
{
public:
	__declspec(dllexport) MsgClientRequestLose_t() : BaseMessage(k_EMsgRequestLose) {}
};

struct MsgServerSendInfo_t : public BaseMessage
{
	__declspec(dllexport) MsgServerSendInfo_t() : BaseMessage(k_EMsgServerSendInfo), m_ulSteamIDServer(0), m_bIsVACSecure(false) {}

	__declspec(dllexport) void SetSteamIDServer(unsigned int SteamID) { m_ulSteamIDServer = SteamID; }
	__declspec(dllexport) unsigned int GetSteamIDServer() { return m_ulSteamIDServer; }

	__declspec(dllexport) void SetSecure(bool bSecure) { m_bIsVACSecure = bSecure; }
	__declspec(dllexport) bool GetSecure() { return m_bIsVACSecure; }

	__declspec(dllexport) void SetServerName(const char* pchName) { strncpy(m_rgchServerName, pchName, sizeof(m_rgchServerName)); }
	__declspec(dllexport) const char* GetServerName() { return m_rgchServerName; }

private:
	unsigned int m_ulSteamIDServer;
	bool m_bIsVACSecure;
	char m_rgchServerName[128]{};
};

// Msg from client to server when initiating authentication
struct MsgClientBeginAuthentication_t : public BaseMessage
{
	__declspec(dllexport) MsgClientBeginAuthentication_t() : BaseMessage(k_EMsgClientBeginAuth) {}

	__declspec(dllexport) void SetSteamID(unsigned int ulSteamID) { m_ulSteamID = ulSteamID; }
	__declspec(dllexport) unsigned int GetSteamID() { return m_ulSteamID; }

private:
	unsigned int m_ulSteamID;
};

// Msg from the server to the client when refusing a connection
struct MsgServerFailAuthentication_t : public BaseMessage
{
	__declspec(dllexport) MsgServerFailAuthentication_t() : BaseMessage(k_EMsgClientFailAuth) {}
};

// Msg from the server to client when accepting a pending connection
struct MsgServerPassAuthentication_t : public BaseMessage
{
	__declspec(dllexport) MsgServerPassAuthentication_t() : BaseMessage(k_EMsgClientPassAuth) {}
};



struct MsgShipPos {
	MsgShipPos() {
		x = -1;
		y = -1;
	}
	int x = -1;
	int y = -1;
};
struct MsgShip {
	MsgShip() {

	}

	MsgShipPos cells[4]{ MsgShipPos(), MsgShipPos(), MsgShipPos(), MsgShipPos() };
};


struct MsgClientReadyAndGiveShips_t : public BaseMessage
{
	__declspec(dllexport) MsgClientReadyAndGiveShips_t() : BaseMessage(k_EMsgClientReadyAndGiveShips) {}

	__declspec(dllexport) void SetShip(int index, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
		ships[index].cells[0].x = x1;
		ships[index].cells[0].y = y1;

		ships[index].cells[1].x = x2;
		ships[index].cells[1].y = y2;

		ships[index].cells[2].x = x3;
		ships[index].cells[2].y = y3;

		ships[index].cells[3].x = x4;
		ships[index].cells[3].y = y4;
	}

	MsgShip ships[10];
};

struct MsgTwoClientsAreReadySoStart_t : public BaseMessage
{
	__declspec(dllexport) MsgTwoClientsAreReadySoStart_t() : BaseMessage(k_EMsgTwoClientsReadySoStart) {}
};