#include "stdafx.h"
#include "My_Socket.h"
#include "SeverDlg.h"

My_Socket::My_Socket()
{
	m_Length = 0;
	memset(m_szBuffer, 0, sizeof(m_szBuffer));
	m_log = _T("");
}


My_Socket::~My_Socket()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		Close();
	}
}


void My_Socket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CSeverDlg *dlg = (CSeverDlg*)AfxGetApp()->GetMainWnd();//主窗口指针对象
	memset(m_szBuffer, 0, sizeof(m_szBuffer));  //每次receive的之前需要把缓冲区清零
	m_Length = Receive(m_szBuffer, sizeof(m_szBuffer), 0); //接收数据
	CString msg(m_szBuffer);
	AsyncSelect(FD_WRITE); //触发OnSend函数 
	CAsyncSocket::OnReceive(nErrorCode);
}


void My_Socket::OnSend(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CSeverDlg *dlg = (CSeverDlg*)AfxGetApp()->GetMainWnd();//主窗口指针对象
	char m_sendBuf[100];   //消息缓冲区 
	memset(m_sendBuf, 0, sizeof(m_sendBuf));
	CTime time = CTime::GetCurrentTime();
	CString str =L"";
	CString msg(m_szBuffer);
	if (msg.Left(4) == "Date" || msg.Left(4) == "date") {
		str = time.Format("%x");
		strcmp(str,m_sendBuf);
	}
	else if (msg.Left(4) == "Time" || msg.Left(4) == "time") {
		str = time.Format("%X");
		//WideCharToMultiByte(CP_ACP, 0, str, -1, m_sendBuf, str.GetLength(), NULL, NULL);
		strcmp(str, m_sendBuf);
	}
	else {
		str = time.Format("错误命令");
		//WideCharToMultiByte(CP_ACP, 0, str, -1, m_sendBuf, str.GetLength(), NULL, NULL);
		strcmp(str, m_sendBuf);
	}
	Send(m_sendBuf, strlen(m_sendBuf));
	str =L"响应客户端:" + str;
	dlg->m_log.AddString(str);
	//触发OnReceive函数  
	AsyncSelect(FD_READ);
	CAsyncSocket::OnSend(nErrorCode);
}


void My_Socket::strcmp(CString str, char *point)
{
	// TODO: 在此处添加实现代码.
	for (int i = 0; i < str.GetLength(); i++) {
		point[i] = str.GetAt(i);
	}
	return;
}
