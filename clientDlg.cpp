
// clientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "client.h"
#include "clientDlg.h"
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


// CclientDlg 对话框



CclientDlg::CclientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CclientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_MESSAGE(WM_SOCKET, &CclientDlg::OnSocket)
	ON_BN_CLICKED(IDC_BUTTON1, &CclientDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CclientDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CclientDlg 消息处理程序

BOOL CclientDlg::OnInitDialog()
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

GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
s = socket(AF_INET, SOCK_STREAM, 0);
WSAAsyncSelect(s, this->m_hWnd, WM_SOCKET, FD_READ);


return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CclientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CclientDlg::OnPaint()
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
HCURSOR CclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




afx_msg LRESULT CclientDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	char cs[100] = "";
	if (lParam == FD_READ) {
		CString num = "";
		recv(s, cs, 100, 0);
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(num);
		num += "\r\n服务器:";
		num += (LPTSTR)cs;
		GetDlgItem(IDC_EDIT3)->SetWindowTextA(num);
	}
	return 0;
}


void CclientDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str, str1;
	int port;
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(str);
	GetDlgItem(IDC_EDIT2)->GetWindowText(str1);
	port = atoi(str1.GetBuffer(1));
	addr1.sin_family = AF_INET;
	addr1.sin_port = ntohs(port);
	addr1.sin_addr.S_un.S_addr = inet_addr(str.GetBuffer(1));
	if ( connect(s, (sockaddr *)&addr1, sizeof(addr1)) ){
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(str);
		str += "连接成功";
		GetDlgItem(IDC_EDIT3)->SetWindowTextA(str);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(0);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(1);
		GetDlgItem(IDC_EDIT1)->EnableWindow(0);
		GetDlgItem(IDC_EDIT2)->EnableWindow(0);
	}
	else {
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(str);
		str += "连接失败";
		GetDlgItem(IDC_EDIT1)->SetWindowTextA(str);
	}


}


void CclientDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str, str1;
	int port;
	GetDlgItem(IDC_EDIT4)->GetWindowTextA(str);
	send(s, str.GetBuffer(1), str.GetLength(), 0);

	GetDlgItem(IDC_EDIT3)->GetWindowTextA(str1);
	str1 += "\r\n";
	str1 += str;
	GetDlgItem(IDC_EDIT3)->SetWindowTextA(str1);
}
