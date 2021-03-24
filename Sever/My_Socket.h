#pragma once
#include <afxsock.h>
class My_Socket :
	public CAsyncSocket
{
public:
	My_Socket();
	~My_Socket();
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	UINT m_Length;
	char m_szBuffer[4096];
	CString m_log;
	void strcmp(CString str,char *point);
};

