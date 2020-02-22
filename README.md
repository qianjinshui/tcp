# tcp
C++ tcp socket

1.创建MFC应用, 基于对话框, 点选套接字, 生成的类dlg.
2. APP类视图 InitInstance()  自动生成 AfxSocketInti();

3.画窗口,
4.添加socket变量.  //在dlg设计界面右击.
  在Dlg类 头文件 (类视图打开)中添加:
  public:
	SOCKET s;
	SOCKET s1;
	int n;
	sockaddr_in addr;
	sockaddr_in addr1;
  
 5.添加消息, 自定义型的.
 
 TCP :server: 1,创建套接字
              2.绑定本地地址和端口
              3.监听端口请求
              4.接受请求
              5.通信
              6.释放套接字
       client:1.创建套接字
              2.向服务器发送请求
              3.连接进行通信
              4.释放套接字
              
TODO,// TODO: 在此添加额外的初始化代码
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
 类向导添加WM_SOCKET自定义消息及方法.
 
 解决方案一：如错误提示中所说的，用新函数替换旧函数。
 解决方案二：define _WINSOCK_DEPRECATED_NO_WARNINGS to disable deprecated API warnings
 在 “stdafx.h” 文件中添加上下面这句，即可。#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
 解决方案三：取消编译器的SDL检查，便不会再报该错误了
 

 connect(s, (sockaddr *)&addr1, sizeof(addr1))
 accept(s, (sockaddr*)&addr1, &length);
 send(s1, str.GetBuffer(1), str.GetLength(), 0)
 recv(
    _In_ SOCKET s,
    _Out_writes_bytes_to_(len, return) __out_data_source(NETWORK) char FAR * buf,
    _In_ int len,
    _In_ int flags
    );
              
