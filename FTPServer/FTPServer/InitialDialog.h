#pragma once


// CInitialDialog �Ի���

class CInitialDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CInitialDialog)

public:
	CInitialDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInitialDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INITIALDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedChooseaddr();
	CString FicowGetDirectory();
	CString path;
	CString Path;
	CString Port;
};
