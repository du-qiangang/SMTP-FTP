
// ClientDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"


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


// CClientDlg 对话框



CClientDlg::CClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Port = 69;
	m_Addr ="127.0.0.1";
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MSG, m_Msg);
	DDX_Control(pDX, IDC_TIME, m_Time);
	m_Port = GetDlgItemInt(IDC_PORT);
	DDX_Text(pDX, IDC_ADDR, m_Addr);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, &CClientDlg::OnBnClickedConnect)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SEND, &CClientDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_CLOSE, &CClientDlg::OnBnClickedClose)
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClientDlg::OnPaint()
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClientDlg::OnBnClickedConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	//默认端口69
	m_clientSocket.m_hSocket = INVALID_SOCKET;
	m_clientSocket.m_bConnected = FALSE;
	if(m_clientSocket.m_bConnected)
	{
		AfxMessageBox(_T("当前已经与服务器建立连接"));
		return;
	}
	UpdateData(TRUE);
	if (!m_Addr.IsEmpty()) {
		 //建立计时器，每1秒尝试连接一次，直到连上或
		SetTimer(1,1000,NULL);
		TryCount = 0;
	}
}


void CClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_clientSocket.m_hSocket == INVALID_SOCKET) {
		//AfxMessageBox(_T("fuck it"));
		BOOL bFlag = m_clientSocket.Create(0, SOCK_STREAM, FD_CONNECT);
		if (!bFlag) {
			AfxMessageBox(L"Socket Error!");
			m_clientSocket.Close();
			PostQuitMessage(0);
			return;
		}
	}
	m_clientSocket.Connect(m_Addr,UINT(m_Port));
	if (TryCount >= 10)
	{
		KillTimer(1);
		AfxMessageBox(_T("连接失败!"));
		return;
	}
	else if (m_clientSocket.m_bConnected)
	{
		AfxMessageBox(_T("连接成功!"));
		KillTimer(1);
		return;
	}
	TryCount++;
	CDialogEx::OnTimer(nIDEvent);
}


void CClientDlg::OnBnClickedSend()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_clientSocket.m_bConnected)
	{
		UpdateData(TRUE);
		m_clientSocket.m_nLength = m_Msg.GetLength();
		int len = WideCharToMultiByte(CP_ACP, 0, m_Msg, -1, NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, m_Msg, -1, m_clientSocket.m_szBuffer, len, NULL, NULL);
		m_clientSocket.m_nLength = strlen(m_clientSocket.m_szBuffer);
		m_clientSocket.AsyncSelect(FD_WRITE);
		SetDlgItemText(IDC_MSG,L"");
		UpdateData(FALSE);
	}
}


void CClientDlg::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	//关闭
	m_clientSocket.ShutDown(); //关闭对话框
	EndDialog(0); 
}
