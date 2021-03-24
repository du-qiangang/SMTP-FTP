
// FTPClientDlg.h : ͷ�ļ�
//
#include "UDPClient.h"
#pragma once


// CFTPClientDlg �Ի���
class CFTPClientDlg : public CDialogEx
{
	// ����
public:
	CFTPClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FTPCLIENT_DIALOG };
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
	afx_msg void OnBnClickedChoosefile();
	afx_msg void OnBnClickedSendfile();
	afx_msg void OnBnClickedTestconnect();
	CString FilePath;
	CString CheckLink;
	CString Record;
	CString FileName;
	CString Port;
	CString DownloadString;
	int hostPort;
	CIPAddressCtrl IPAddress;
	CUDPClient MySock;
	afx_msg void OnBnClickedChoosedownloadfile();
	afx_msg void OnBnClickedDownloadfile();
	CString HostPort;
};
