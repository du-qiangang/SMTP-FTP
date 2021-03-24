// FileListDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FTPClient.h"
#include "FileListDialog.h"
#include "afxdialogex.h"


// CFileListDialog �Ի���

IMPLEMENT_DYNAMIC(CFileListDialog, CDialogEx)

CFileListDialog::CFileListDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FILEDIALOG, pParent)
	, Path(_T(""))
{
	chooseFile = L"";
	downloadPath = L"";
}

CFileListDialog::~CFileListDialog()
{
}

void CFileListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, ListBox);
	DDX_Text(pDX, IDC_EDIT1, Path);
}


BEGIN_MESSAGE_MAP(CFileListDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CFileListDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHOOSE, &CFileListDialog::OnBnClickedChoose)
END_MESSAGE_MAP()


// CFileListDialog ��Ϣ�������


BOOL CFileListDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString temp = L"";
	int i = 0;
	for (;list[i] || list[i + 1];i++)
	{
		if (list[i] != 0)
			temp += list[i];
		else
		{
			ListBox.AddString(temp);
			temp = L"";
		}
	}
	if (temp != L"")
		ListBox.AddString(temp);
	Path = downloadPath;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CFileListDialog::OnBnClickedOk()
{
	int now = ListBox.GetCurSel();
	if (now > -1)
		ListBox.GetText(now, chooseFile);
	CDialogEx::OnOK();
}

CString CFileListDialog::FicowGetDirectory()
{
	TCHAR           szFolderPath[MAX_PATH] = { 0 };
	CString         strFolderPath = TEXT("");
	BROWSEINFO      sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = L"��ѡ���ļ�����·��";
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

void CFileListDialog::OnBnClickedChoose()
{
	downloadPath = FicowGetDirectory() + L"\\";
	Path = downloadPath;
	UpdateData(FALSE);
}
