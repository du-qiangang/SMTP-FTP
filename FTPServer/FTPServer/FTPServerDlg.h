
// FTPServerDlg.h : ͷ�ļ�
//
#include "UDPClient.h"

#pragma once


// CFTPServerDlg �Ի���
class CFTPServerDlg : public CDialogEx
{
	// ����
public:
	CFTPServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FTPSERVER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonsleep();
	afx_msg void OnBnClickedButtonleft();
	CUDPClient MySock;
	CString Record;
	int hostPort;
	CString HostPort;
};
