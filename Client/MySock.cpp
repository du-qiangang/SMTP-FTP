#include "stdafx.h"
#include "MySock.h"
#include "Client.h"
#include "ClientDlg.h"


MySock::MySock()
{
	m_nLength = 0;
	memset(m_szBuffer, 0, sizeof(m_szBuffer));
	m_bConnected = FALSE;
}


MySock::~MySock()
{
	if (m_hSocket != INVALID_SOCKET)
		Close();
}


void MySock::OnConnect(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (nErrorCode == 0) {
		m_bConnected = TRUE;
		//提请一个“读”的网络事件，准备接收
		AsyncSelect(FD_READ);
	}

	CAsyncSocket::OnConnect(nErrorCode);
}


void MySock::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_nLength = Receive(m_szBuffer, sizeof(m_szBuffer), 0);
	//下面两行代码用来获取对话框指针
	CClientDlg* pDlg = (CClientDlg*)AfxGetApp()->GetMainWnd();
	CString str(m_szBuffer);
	//str.Format(L"%s", m_szBuffer);
	pDlg ->m_Time.SetWindowTextW(str);
	memset(m_szBuffer, 0, sizeof(m_szBuffer));
	AsyncSelect(FD_READ);
	CAsyncSocket::OnReceive(nErrorCode);
}


void MySock::OnSend(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	Send(m_szBuffer, m_nLength, 0); 
	m_nLength = 0;
	memset(m_szBuffer, 0, sizeof(m_szBuffer));
	//继续提请一个“读”的网络事件，接收Server消息
	AsyncSelect(FD_READ);
	CAsyncSocket::OnSend(nErrorCode);
}
