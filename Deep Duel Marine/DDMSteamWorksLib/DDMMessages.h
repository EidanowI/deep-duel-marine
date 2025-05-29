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

	k_EMsgTurn = k_EMsgServerBegin + 11,
	k_EMsgShot = k_EMsgServerBegin + 12,
	k_EMsgShotResult = k_EMsgServerBegin + 13,
	

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

	__declspec(dllexport) void SetSteamIDServer(unsigned long long SteamID) { m_ulSteamIDServer = SteamID; }
	__declspec(dllexport) unsigned long long GetSteamIDServer() { return m_ulSteamIDServer; }

	__declspec(dllexport) void SetSecure(bool bSecure) { m_bIsVACSecure = bSecure; }
	__declspec(dllexport) bool GetSecure() { return m_bIsVACSecure; }

	__declspec(dllexport) void SetServerName(const char* pchName) { strncpy(m_rgchServerName, pchName, sizeof(m_rgchServerName)); }
	__declspec(dllexport) const char* GetServerName() { return m_rgchServerName; }

private:
	unsigned long long m_ulSteamIDServer;
	bool m_bIsVACSecure;
	char m_rgchServerName[128]{};
};

// Msg from client to server when initiating authentication
struct MsgClientBeginAuthentication_t : public BaseMessage
{
	__declspec(dllexport) MsgClientBeginAuthentication_t() : BaseMessage(k_EMsgClientBeginAuth) {}

	__declspec(dllexport) void SetSteamID(unsigned long long ulSteamID) { m_ulSteamID = ulSteamID; }
	__declspec(dllexport) unsigned long long GetSteamID() { return m_ulSteamID; }

private:
	unsigned long long m_ulSteamID;
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

struct MsgTurn_t : public BaseMessage
{
	__declspec(dllexport) MsgTurn_t() : BaseMessage(k_EMsgTurn) {}
};

struct MsgShot_t : public BaseMessage
{
	__declspec(dllexport) MsgShot_t() : BaseMessage(k_EMsgShot) {}

	__declspec(dllexport) void SetPos(int x, int y) {
		m_x = x;
		m_y = y;
	}

	int m_x;
	int m_y;
};

struct MsgShotResult_t : public BaseMessage
{
	__declspec(dllexport) MsgShotResult_t() : BaseMessage(k_EMsgShotResult) {}

	int m_x;
	int m_y;
	bool is_dead;
};