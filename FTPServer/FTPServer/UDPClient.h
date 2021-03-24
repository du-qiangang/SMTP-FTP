#pragma once

// CUDPClient ÃüÁîÄ¿±ê



class CUDPClient : public CAsyncSocket
{
public:
	CUDPClient();
	virtual ~CUDPClient();
	virtual void OnSend(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	int checkUser(CString IP, UINT port);
	int checkDownload(CString file);
	int openWriteFile(int userID, CString filePath);
	int openReadFile(int userID, CString filePath);
	void closeFile(int userID);
	CString findAllFile();
	bool checkReSend;
	bool testPack;
	bool sendFileList;
	bool sendFile;
	UINT tempPort;
	CString tempIP;
	CFileFind finder;
	CString findWild;
	int nowUserNumber;
	int nowUser;
	int nowDownloadNumber;
	CString path;
	struct Download
	{
		CString downloadFile;
		CFile file2;
	}download[100];
	struct User
	{
		UINT port;
		CString IP;
		CString sendFileName;
		int nowSendBlock;
		CString receiveFileName;
		int nowReceiveBlock;
		CFile file;
		CString downloadFile;
		int endFile;
		int nowDownload;
	}user[100];
	static VOID CALLBACK CheckSend(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
};

#pragma pack(1)
struct Pack
{
	WORD Operation;
	BYTE Content[1000];
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