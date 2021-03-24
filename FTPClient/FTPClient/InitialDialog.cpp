// InitialDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FTPClient.h"
#include "InitialDialog.h"
#include "afxdialogex.h"


// CInitialDialog �Ի���

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


// CInitialDialog ��Ϣ�������


void CInitialDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	port = _ttoi(Port);
	CDialogEx::OnOK();
}
