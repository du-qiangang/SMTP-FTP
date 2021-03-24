
// FTPServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FTPServer.h"
#include "FTPServerDlg.h"
#include "afxdialogex.h"
#include "InitialDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFTPServerDlg �Ի���



CFTPServerDlg::CFTPServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FTPSERVER_DIALOG, pParent)
	, Record(_T(""))
	, HostPort(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFTPServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, Record);
	DDX_Text(pDX, IDC_SHOWPORT, HostPort);
}

BEGIN_MESSAGE_MAP(CFTPServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONSLEEP, &CFTPServerDlg::OnBnClickedButtonsleep)
	ON_BN_CLICKED(IDC_BUTTONLEFT, &CFTPServerDlg::OnBnClickedButtonleft)
END_MESSAGE_MAP()


// CFTPServerDlg ��Ϣ�������

BOOL CFTPServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	CInitialDialog dialog;
	INT_PTR result = dialog.DoModal();
	if (result == IDOK)
	{
		hostPort = _ttoi(dialog.Port);
		MySock.path = dialog.path;
		MySock.findWild = MySock.path + L"*.*";
		BOOL bFlag = MySock.Create(69, SOCK_DGRAM, FD_READ);
		if (!bFlag)
		{
			MessageBox(L"FTP����������ʧ�ܣ�", L"����ʧ�ܣ�", MB_ICONERROR);
			exit(0);
		}

	}
	else
		exit(0);

	HostPort.Format(L"���������˿ںţ�%d", hostPort);
	Record = L"**** FTP������׼���� ****\r\n";
	UpdateData(FALSE);

	MySock.AsyncSelect(FD_READ);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CFTPServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFTPServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFTPServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFTPServerDlg::OnBnClickedButtonsleep()
{
	Sleep(4000);
}


void CFTPServerDlg::OnBnClickedButtonleft()
{
	MySock.testPack = !MySock.testPack;
}
