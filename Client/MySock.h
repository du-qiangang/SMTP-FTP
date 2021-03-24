#pragma once
#include <afxsock.h>
class MySock :
	public CAsyncSocket
{
public:
	MySock();
	~MySock();
public: BOOL m_bConnected;
		//是否连接 
		UINT m_nLength;
		//消息长度 
		char m_szBuffer[1024];
		//消息缓冲区
		virtual void OnConnect(int nErrorCode);
		virtual void OnReceive(int nErrorCode);
		virtual void OnSend(int nErrorCode);
};

