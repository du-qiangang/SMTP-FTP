#pragma once

// CUDPClient ÃüÁîÄ¿±ê

class CUDPClient : public CAsyncSocket
{
public:
	CUDPClient();
	virtual ~CUDPClient();
	bool checkLink;
	bool sendFile;
	bool sendContent;
	bool endFile;
	bool findFile;
	bool downloadFile;
	bool checkReSend;
	int port;
	int reSend;
	int nowPosition;
	char readBuffer[513];
	UINT recvPort;
	CString recvIP;
	CString upLoadFileName;
	CString downloadFileName;
	CString filePath;
	CString strIP;
	CFile file;
	CString downloadPath;
	virtual void OnSend(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	int openReadFile(CString filePath);
	int openWriteFile(CString filePath);
	void closeFile();
	static VOID CALLBACK CheckReceive(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
};
#pragma pack(1)
struct Pack
{
	WORD Operation;
	BYTE Content[1000];
};
struct OpeartionPack
{
	WORD Operation;
};
struct TitlePack
{
	WORD Operation;
	BYTE Content[200];
};
struct ContentPack
{
	WORD Operation;
	WORD BlockNumber;
	WORD Length;
	BYTE Content[512];
};
struct FilePack
{
	WORD Operation;
	BYTE Content[1000];
};
struct ACKPack
{
	WORD Operation;
	WORD BlockNumber;
};
#pragma pack()

