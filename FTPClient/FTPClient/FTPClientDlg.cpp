
// FTPClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FTPClient.h"
#include "FTPClientDlg.h"
#include "afxdialogex.h"
#include "InitialDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CFTPClientDlg 对话框



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


// CFTPClientDlg 消息处理程序

BOOL CFTPClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	hostPort = 110;
	CInitialDialog dialog;
	INT_PTR result = dialog.DoModal();
	if (result == IDOK)
	{
		hostPort = dialog.port;
		BOOL bFlag = MySock.Create(hostPort, SOCK_DGRAM, FD_READ);
		if (!bFlag)
		{
			MessageBox(L"FTP客户端开启失败！\r\n可能错误原因：端口未开启或已被占用！", L"FTP客户端开启失败！", MB_ICONERROR);
			exit(0);
		}
	}
	else
		exit(0);
	Record = L"**** FTP客户端准备好 ****\r\n";
	Port = L"69";
	HostPort.Format(L"本机当前端口号：%d", hostPort);
	IPAddress.SetAddress(127, 0, 0, 1);
	CheckLink = L"连接情况：当前尚未测试连接！";
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFTPClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFTPClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFTPClientDlg::OnBnClickedChoosefile()
{
	BOOL isOpen = TRUE;
	CString defaultDir = L"C:";
	CString fileName = L"";
	CString filter = L"所有文件 (*.*)|*.*||";
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
