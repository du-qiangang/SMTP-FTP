
// FTPClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FTPClient.h"
#include "FTPClientDlg.h"
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


// CFTPClientDlg �Ի���



CFTPClientDlg::CFTPClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FTPCLIENT_DIALOG, pParent)
	, FilePath(_T(""))
	, Port(_T(""))
	, CheckLink(_T(""))
	, Record(_T(""))
	, DownloadString(_T(""))
	, HostPort(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFTPClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, FilePath);
	DDX_Text(pDX, IDC_EDIT3, Port);
	DDX_Control(pDX, IDC_IPADDR, IPAddress);
	DDX_Text(pDX, IDC_CHECKLINK, CheckLink);
	DDX_Text(pDX, IDC_EDIT2, Record);
	DDX_Text(pDX, IDC_EDIT4, DownloadString);
	DDX_Text(pDX, IDC_NOWPORT, HostPort);
}

BEGIN_MESSAGE_MAP(CFTPClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHOOSEFILE, &CFTPClientDlg::OnBnClickedChoosefile)
	ON_BN_CLICKED(IDC_SENDFILE, &CFTPClientDlg::OnBnClickedSendfile)
	ON_BN_CLICKED(IDC_TESTCONNECT, &CFTPClientDlg::OnBnClickedTestconnect)
	ON_BN_CLICKED(IDC_CHOOSEDOWNLOADFILE, &CFTPClientDlg::OnBnClickedChoosedownloadfile)
	ON_BN_CLICKED(IDC_DOWNLOADFILE, &CFTPClientDlg::OnBnClickedDownloadfile)
END_MESSAGE_MAP()


// CFTPClientDlg ��Ϣ�������

BOOL CFTPClientDlg::OnInitDialog()
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

	hostPort = 110;
	CInitialDialog dialog;
	INT_PTR result = dialog.DoModal();
	if (result == IDOK)
	{
		hostPort = dialog.port;
		BOOL bFlag = MySock.Create(hostPort, SOCK_DGRAM, FD_READ);
		if (!bFlag)
		{
			MessageBox(L"FTP�ͻ��˿���ʧ�ܣ�\r\n���ܴ���ԭ�򣺶˿�δ�������ѱ�ռ�ã�", L"FTP�ͻ��˿���ʧ�ܣ�", MB_ICONERROR);
			exit(0);
		}
	}
	else
		exit(0);
	Record = L"**** FTP�ͻ���׼���� ****\r\n";
	Port = L"69";
	HostPort.Format(L"������ǰ�˿ںţ�%d", hostPort);
	IPAddress.SetAddress(127, 0, 0, 1);
	CheckLink = L"�����������ǰ��δ�������ӣ�";
	UpdateData(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CFTPClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFTPClientDlg::OnPaint()
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
HCURSOR CFTPClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFTPClientDlg::OnBnClickedChoosefile()
{
	BOOL isOpen = TRUE;
	CString defaultDir = L"C:";
	CString fileName = L"";
	CString filter = L"�����ļ� (*.*)|*.*||";
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = L"";
	INT_PTR result = openFileDlg.DoModal();
	if (result == IDOK)
	{
		FilePath = openFileDlg.GetPathName();
		FileName = openFileDlg.GetFileName();
		UpdateData(FALSE);
	}
}

void CFTPClientDlg::OnBnClickedSendfile()
{
	UpdateData(TRUE);
	MySock.port = _ttoi(Port);
	BYTE nf1, nf2, nf3, nf4;
	IPAddress.GetAddress(nf1, nf2, nf3, nf4);
	MySock.strIP.Format(L"%u.%u.%u.%u", nf1, nf2, nf3, nf4);
	MySock.upLoadFileName = FileName;
	MySock.filePath = FilePath;
	MySock.sendFile = true;
	MySock.AsyncSelect(FD_WRITE);
}

void CFTPClientDlg::OnBnClickedTestconnect()
{
	UpdateData(TRUE);
	MySock.port = _ttoi(Port);
	BYTE nf1, nf2, nf3, nf4;
	IPAddress.GetAddress(nf1, nf2, nf3, nf4);
	MySock.strIP.Format(L"%u.%u.%u.%u", nf1, nf2, nf3, nf4);
	MySock.checkLink = true;
	MySock.AsyncSelect(FD_WRITE);
}

void CFTPClientDlg::OnBnClickedChoosedownloadfile()
{
	UpdateData(TRUE);
	MySock.port = _ttoi(Port);
	BYTE nf1, nf2, nf3, nf4;
	IPAddress.GetAddress(nf1, nf2, nf3, nf4);
	MySock.strIP.Format(L"%u.%u.%u.%u", nf1, nf2, nf3, nf4);
	MySock.findFile = true;
	MySock.AsyncSelect(FD_WRITE);
}

void CFTPClientDlg::OnBnClickedDownloadfile()
{
	UpdateData(TRUE);
	MySock.port = _ttoi(Port);
	BYTE nf1, nf2, nf3, nf4;
	IPAddress.GetAddress(nf1, nf2, nf3, nf4);
	MySock.strIP.Format(L"%u.%u.%u.%u", nf1, nf2, nf3, nf4);
	MySock.downloadFileName = DownloadString;
	MySock.downloadFile = true;
	MySock.AsyncSelect(FD_WRITE);
}
