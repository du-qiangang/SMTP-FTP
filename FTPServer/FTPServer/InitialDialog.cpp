// InitialDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "FTPServer.h"
#include "InitialDialog.h"
#include "afxdialogex.h"


// CInitialDialog 对话框

IMPLEMENT_DYNAMIC(CInitialDialog, CDialogEx)

CInitialDialog::CInitialDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_INITIALDIALOG, pParent)
	, Path(_T("D:\\FTP Files\\"))
	, Port(_T("69"))
{
	path = L"D:\\FTP Files\\";
}

CInitialDialog::~CInitialDialog()
{
}

void CInitialDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, Path);
	DDX_Text(pDX, IDC_EDIT2, Port);
}


BEGIN_MESSAGE_MAP(CInitialDialog, CDialogEx)
	ON_BN_CLICKED(IDC_CHOOSEADDR, &CInitialDialog::OnBnClickedChooseaddr)
END_MESSAGE_MAP()


// CInitialDialog 消息处理程序


void CInitialDialog::OnBnClickedChooseaddr()
{
	path = FicowGetDirectory() + L"\\";
	Path = path;
	UpdateData(FALSE);
}

CString CInitialDialog::FicowGetDirectory()
{
	TCHAR           szFolderPath[MAX_PATH] = { 0 };
	CString         strFolderPath = TEXT("");
	BROWSEINFO      sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = L"请选择文件下载路径";
	sInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX | BIF_DONTGOBELOWDOMAIN;
	sInfo.lpfn = NULL;
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != NULL)
		if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
			strFolderPath = szFolderPath;
	if (lpidlBrowse != NULL)
		::CoTaskMemFree(lpidlBrowse);
	return strFolderPath;
}