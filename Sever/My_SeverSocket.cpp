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
	// TODO: �ڴ����ר�ô����/����û���
	My_Socket *pSocket = new My_Socket();
	if (Accept(*pSocket))
	{
		pSocket->AsyncSelect(FD_READ); //����ͨ��socket��Read����������  
		m_pSocket = pSocket;
	}
	else
	{
		delete pSocket;
	}
	CAsyncSocket::OnAccept(nErrorCode);
}
