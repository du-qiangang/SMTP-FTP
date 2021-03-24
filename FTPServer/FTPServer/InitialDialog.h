#pragma once


// CInitialDialog 对话框

class CInitialDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CInitialDialog)

public:
	CInitialDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInitialDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INITIALDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedChooseaddr();
	CString FicowGetDirectory();
	CString path;
	CString Path;
	CString Port;
};
