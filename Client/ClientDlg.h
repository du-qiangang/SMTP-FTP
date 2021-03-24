
// ClientDlg.h: 头文件
//

#pragma once
#include"MySock.h"

// CClientDlg 对话框
class CClientDlg : public CDialogEx
{
// 构造
public:
	CClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConnect();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedClose();
	// 发送内容
	CString m_Msg;
	CEdit m_Time;
protected:
	int TryCount;
	MySock m_clientSocket;
public:
	int m_Port;
	CString m_Addr;
};
