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
	// TODO: �ڴ����ר�ô����/����û���
	CSeverDlg *dlg = (CSeverDlg*)AfxGetApp()->GetMainWnd();//������ָ�����
	memset(m_szBuffer, 0, sizeof(m_szBuffer));  //ÿ��receive��֮ǰ��Ҫ�ѻ���������
	m_Length = Receive(m_szBuffer, sizeof(m_szBuffer), 0); //��������
	CString msg(m_szBuffer);
	AsyncSelect(FD_WRITE); //����OnSend���� 
	CAsyncSocket::OnReceive(nErrorCode);
}


void My_Socket::OnSend(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	CSeverDlg *dlg = (CSeverDlg*)AfxGetApp()->GetMainWnd();//������ָ�����
	char m_sendBuf[100];   //��Ϣ������ 
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
		str = time.Format("��������");
		//WideCharToMultiByte(CP_ACP, 0, str, -1, m_sendBuf, str.GetLength(), NULL, NULL);
		strcmp(str, m_sendBuf);
	}
	Send(m_sendBuf, strlen(m_sendBuf));
	str =L"��Ӧ�ͻ���:" + str;
	dlg->m_log.AddString(str);
	//����OnReceive����  
	AsyncSelect(FD_READ);
	CAsyncSocket::OnSend(nErrorCode);
}


void My_Socket::strcmp(CString str, char *point)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	for (int i = 0; i < str.GetLength(); i++) {
		point[i] = str.GetAt(i);
	}
	return;
}
