#pragma once
#include <afxsock.h>
#include "My_Socket.h"
class My_SeverSocket :
	public CAsyncSocket
{
public:
	My_SeverSocket();
	~My_SeverSocket();
	virtual void OnAccept(int nErrorCode);
	My_Socket *m_pSocket;
	CString m_msg;
};

