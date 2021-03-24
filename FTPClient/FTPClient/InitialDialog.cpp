// InitialDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "FTPClient.h"
#include "InitialDialog.h"
#include "afxdialogex.h"


// CInitialDialog 对话框

IMPLEMENT_DYNAMIC(CInitialDialog, CDialogEx)

CInitialDialog::CInitialDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_INITIALDIALOG, pParent)
	, Port(_T(""))
{

}

CInitialDialog::~CInitialDialog()
{
}

void CInitialDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, Port);
}


BEGIN_MESSAGE_MAP(CInitialDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInitialDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CInitialDialog 消息处理程序


void CInitialDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	port = _ttoi(Port);
	CDialogEx::OnOK();
}
