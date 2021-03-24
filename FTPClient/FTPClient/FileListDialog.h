#pragma once
#include "afxwin.h"


// CFileListDialog �Ի���

class CFileListDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CFileListDialog)

public:
	CFileListDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFileListDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILEDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
