
// tcpServerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "tcpServer.h"
#include "tcpServerDlg.h"
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


// CtcpServerDlg 对话框



CtcpServerDlg::CtcpServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TCPSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtcpServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
}

BEGIN_MESSAGE_MAP(CtcpServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET, &CtcpServerDlg::OnSocket)
	ON_BN_CLICKED(IDC_BUTTON1, &CtcpServerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CtcpServerDlg 消息处理程序

BOOL CtcpServerDlg::OnInitDialog()
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
	//1.初始化套接字库.  勾选了,已经初始化了
	//2.创建套接字
	s = socket(AF_INET, SOCK_STREAM, 0);
	//3.处理地址---->>网络自己顺序
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	//4.绑定端口和ip
	bind(s, (sockaddr*)&addr, sizeof(addr));
	listen(s, 5);
	//设置套接字为异步套接字;
	WSAAsyncSelect(s, this->m_hWnd, WM_SOCKET, FD_ACCEPT | FD_READ);
	
	//GetDlgItem(IDC_EDIT1)->EnableWindow(false);

	GetDlgItem(IDC_STATIC)->SetWindowTextA(CString("监听启动"));

	//
	//修改服务器状态

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CtcpServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CtcpServerDlg::OnPaint()
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
HCURSOR CtcpServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



afx_msg LRESULT CtcpServerDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	CString str;
	char cs[100] = "";
	switch (lParam)
	{
	case FD_ACCEPT:
	{
		int length = sizeof(addr1);
		s1 = accept(s, (sockaddr*)&addr1, &length);
		n = n + 1;
		str.Format("有%d个客户端已经连接", n);
		str += inet_ntoa(addr1.sin_addr);
		GetDlgItem(IDC_EDIT1)->SetWindowTextA(str);


	}break;
	case FD_READ:
	{
		CString num = "";
		recv(s1, cs, 100, 0);

		GetDlgItem(IDC_EDIT1)->GetWindowTextA(num);
		num += "\r\n客户端:";
		num += (LPTSTR)cs;
		GetDlgItem(IDC_EDIT1)->SetWindowTextA(num);


	}break;
	default:
		break;
	}
	return 0;
}


void CtcpServerDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str,str1;
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(str);
	if (str == "") {
		MessageBox("消息不能为空");
	}
	else {
		//发送消息给客户端
		//发送成功
		if (send(s1, str.GetBuffer(1), str.GetLength(), 0) != SOCKET_ERROR) {
			GetDlgItem(IDC_EDIT1)->GetWindowTextA(str1);
			str1 += "\r\nsent:";
			str1 += str;
			GetDlgItem(IDC_EDIT1)->SetWindowTextA(str1);

		}
		else {//发送失败
			GetDlgItem(IDC_EDIT1)->SetWindowTextA("send message fail\r\n");
		}
	}
	

}
