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
	// TODO: �ڴ����ר�ô����/����û���
	if (nErrorCode == 0) {
		m_bConnected = TRUE;
		//����һ���������������¼���׼������
		AsyncSelect(FD_READ);
	}

	CAsyncSocket::OnConnect(nErrorCode);
}


void MySock::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	m_nLength = Receive(m_szBuffer, sizeof(m_szBuffer), 0);
	//�������д���������ȡ�Ի���ָ��
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
	// TODO: �ڴ����ר�ô����/����û���
	Send(m_szBuffer, m_nLength, 0); 
	m_nLength = 0;
	memset(m_szBuffer, 0, sizeof(m_szBuffer));
	//��������һ���������������¼�������Server��Ϣ
	AsyncSelect(FD_READ);
	CAsyncSocket::OnSend(nErrorCode);
}
