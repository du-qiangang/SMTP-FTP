#pragma once
#include "afxwin.h"


// CFileListDialog 对话框

class CFileListDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CFileListDialog)

public:
	CFileListDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileListDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILEDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListBox ListBox;
	virtual BOOL OnInitDialog();
	BYTE* list;
	CString chooseFile;
	CString downloadPath;
	afx_msg void OnBnClickedOk();
	CString FicowGetDirectory();
	afx_msg void OnBnClickedChoose();
	CString Path;
};
