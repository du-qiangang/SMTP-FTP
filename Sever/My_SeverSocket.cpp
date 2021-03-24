#include "stdafx.h"
#include "My_SeverSocket.h"


My_SeverSocket::My_SeverSocket()
{
	m_msg = _T("");
}


My_SeverSocket::~My_SeverSocket()
{
}


void My_SeverSocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	My_Socket *pSocket = new My_Socket();
	if (Accept(*pSocket))
	{
		pSocket->AsyncSelect(FD_READ); //触发通信socket的Read函数读数据  
		m_pSocket = pSocket;
	}
	else
	{
		delete pSocket;
	}
	CAsyncSocket::OnAccept(nErrorCode);
}
