
// SeverDlg.h: 头文件
//

#pragma once
#include "My_SeverSocket.h"

// CSeverDlg 对话框
class CSeverDlg : public CDialogEx
{
// 构造
public:
	CSeverDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	My_SeverSocket m_SeverSocket;

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
	// 侦听并传输数据
	CListBox m_log;
	afx_msg void OnBnClickedStart();
};
