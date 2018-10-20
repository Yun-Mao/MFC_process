
// MFCApplication1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"


#include <iostream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
bool frun; 
BOOL fchild;
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCApplication1Dlg::IDD, pParent)
{
	guan=guandao();
	mail=mailslot();
	ni=niming();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, Content_Text);
	DDX_Control(pDX, IDC_EDIT3, cip);
	DDX_Control(pDX, IDC_EDIT1, cname);
	DDX_Control(pDX, IDC_CHECK1, runc);
	DDX_Control(pDX, IDC_LIST1, chistroy);
	DDX_Control(pDX, IDC_RADIO4, radio_niming);
}

void recmessage();
BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Send, &CMFCApplication1Dlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_Recive, &CMFCApplication1Dlg::OnBnClickedRecive)
	ON_BN_CLICKED(IDC_RADIO1, &CMFCApplication1Dlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CMFCApplication1Dlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_build, &CMFCApplication1Dlg::OnBnClickedbuild)
	ON_BN_CLICKED(IDC_connect, &CMFCApplication1Dlg::OnBnClickedconnect)
	ON_BN_CLICKED(IDC_del, &CMFCApplication1Dlg::OnBnClickeddel)
	ON_BN_CLICKED(IDC_CHECK1, &CMFCApplication1Dlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_RADIO3, &CMFCApplication1Dlg::OnBnClickedRadio3)
	//ON_MESSAGE_VOID(WM_COPYDATA,recmessage)
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
	ON_LBN_DBLCLK(IDC_LIST1, &CMFCApplication1Dlg::dclick)
	ON_BN_CLICKED(IDC_RADIO4, &CMFCApplication1Dlg::OnBnClickedRadio4)
	ON_EN_CHANGE(IDC_EDIT3, &CMFCApplication1Dlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT1, &CMFCApplication1Dlg::OnEnChangeEdit1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMFCApplication1Dlg::OnLbnSelchangeList1)
	ON_STN_CLICKED(IDC_STATIC2, &CMFCApplication1Dlg::OnStnClickedStatic2)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication1Dlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
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
	HANDLE m=GetStdHandle(STD_INPUT_HANDLE);//判断是不是生成的子进程
	fchild=(m!=0);
	if(fchild)//如果是就自动选择4模式
	{
		radio_niming.SetCheck(4);
		OnBnClickedRadio4();
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
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
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int now=0;
void CMFCApplication1Dlg::OnBnClickedSend()
{
	CString content,name;
	Content_Text.GetWindowTextA(content);
	int t=0;
	int s=_getpid();
	char pid[100];
	_itoa_s(s,pid,10);
	SYSTEMTIME tt;
	GetLocalTime(&tt);
	wsprintf(pid+strlen(pid)," %02d:%02d",tt.wHour,tt.wMinute);
	content="pid-"+CString(pid)+" :"+content;
	switch (now)
	{
		case 1:
			t=mail.send(content);
			break;
		case 2:
			t=guan.write(content);
			break;
		case 4:
			t=ni.send(content);
			break;
		default:
			MessageBox("请选择类型！","错误",MB_ICONERROR);
			return;
	}
	if(!t)return;
	chistroy.InsertString(-1,"send "+content);
	Content_Text.SetWindowTextA("");
}
//回车键发送
void CMFCApplication1Dlg::OnOK()
{
	OnBnClickedSend();
}

//按任意字母切换到输入框
BOOL CMFCApplication1Dlg::PreTranslateMessage(MSG *p)
{
	int m=p->wParam;
	if(m>'A'&&m<'Z')
	{
		if(GetFocus()!=Content_Text.GetHandle())//判断是否以有焦点，不然会卡死
			 Content_Text.SetFocus();
	}
	return CDialog::PreTranslateMessage(p);
}

void CMFCApplication1Dlg::OnBnClickedRecive()
{
	CString content;
	switch (now)
	{
		case 1:
			content=mail.read();break;
		case 2:
			content=guan.read();break;
		case 4:
			content=ni.recv();break;
		default:
			MessageBox("请选择类型！","错误",MB_ICONERROR);
			return;
	}
	if(content!="")chistroy.InsertString(-1,"Recv "+content);

}


//主动定时接受消息，避免窗口因为同名而接受不到消息
DWORD WINAPI read(LPVOID lpParameter)
{
	CMFCApplication1Dlg *p=(CMFCApplication1Dlg*)lpParameter;
	while(frun)
	{
		int hh=0;
		while(!hh)
		{
			if(!frun)return 0;
			Sleep(1000);
			if(now<3&&!(p->GetDlgItem(IDC_del)->IsWindowEnabled()))continue;
			switch (now)
			{
			case 1:  hh=p->mail.have();break;
			case 2: hh=p->guan.have();break;
			case 4: hh=p->ni.have();break;
			default:
			break;
			}
		}
	p->OnBnClickedRecive();
	}
	return 0;
}
//自动接受消息
void CMFCApplication1Dlg::OnBnClickedCheck1()
{
	frun=!frun;
	if(frun)CreateThread(NULL,0,read,this,0,NULL);
}

//创立服务端
void CMFCApplication1Dlg::OnBnClickedbuild()
{
	CString serve,name;
	cip.GetWindowTextA(serve);
	cname.GetWindowTextA(name);
	int t=0;
	switch (now)
	{
	case 1:
		t=mail.create(serve,name);
		break;
	case 2:
		t=guan.create(serve,name);break;
	case 4:
		t=ni.build();
		break;
	default:
		break;
	}
	if(t)
	{
		GetDlgItem(IDC_build)->EnableWindow(FALSE);
		GetDlgItem(IDC_connect)->EnableWindow(FALSE);
		GetDlgItem(IDC_del)->EnableWindow(TRUE);
		if(now==1)GetDlgItem(IDC_Send)->EnableWindow(FALSE);
	}
}
//创立发送端
void CMFCApplication1Dlg::OnBnClickedconnect()
{
	CString serve,name;
	cip.GetWindowTextA(serve);
	cname.GetWindowTextA(name);
	int t=0;
	switch (now)
	{
	case 1:
		t=mail.connect(serve,name);break;
	case 2:
		t=guan.connect(serve,name);break;
	default:
		ni.connect();
		t=1;
		break;
	}
	if(t)
	{
		GetDlgItem(IDC_connect)->EnableWindow(FALSE);
		GetDlgItem(IDC_build)->EnableWindow(FALSE);
		GetDlgItem(IDC_del)->EnableWindow(TRUE);
		if(now==1)GetDlgItem(IDC_Recive)->EnableWindow(FALSE);
	}
}
//断开链接
void CMFCApplication1Dlg::OnBnClickeddel()
{
	int t;
	switch (now)
	{
	case 1:
		t=mail.free();break;
	case 2:
		t=guan.free();break;
	default:
		break;
	}
	if(t)
	{
		GetDlgItem(IDC_build)->EnableWindow(TRUE);
		GetDlgItem(IDC_connect)->EnableWindow(TRUE);
		GetDlgItem(IDC_del)->EnableWindow(FALSE);
		GetDlgItem(IDC_Recive)->EnableWindow(TRUE);
		GetDlgItem(IDC_Send)->EnableWindow(TRUE);
	}
}


//重载消息定义
BOOL CMFCApplication1Dlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	if (now==3&&pCopyDataStruct != NULL)
	{
		LPCTSTR pszText = (LPCTSTR)(pCopyDataStruct->lpData);
		DWORD dwLength = (DWORD)(pCopyDataStruct->cbData);
		TCHAR szRecvText[1024] = {0};
		memcpy(szRecvText, pszText, dwLength);
		chistroy.InsertString(-1,"Recv "+CString(szRecvText));
	}
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}

//历史记录清除
void CMFCApplication1Dlg::OnBnClickedButton1()
{
	chistroy.ResetContent();
}

//双击获得消息
void CMFCApplication1Dlg::dclick()
{
	CString c,s;
	int nItem = chistroy.GetCurSel();;
	chistroy.GetText(nItem,s);
    Content_Text.GetWindowTextA(c);
	s=s.Right(s.GetLength()-1-s.ReverseFind(':'));
    Content_Text.SetWindowTextA(c+s);
   
}



//选择邮槽
void CMFCApplication1Dlg::OnBnClickedRadio1()
{
	GetDlgItem(IDC_EDIT_COPYCB)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_Send)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_LIST1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC4)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATICp)->ShowWindow(SW_SHOW);
	now=1;
	GetDlgItem(IDC_STATIC)->SetWindowTextA("server:");
	GetDlgItem(IDC_STATICp)->SetWindowTextA("邮槽名:");
	cip.ShowWindow(SW_SHOW);
	cip.SetWindowTextA(".");
	cname.ShowWindow(SW_SHOW);
	cname.SetWindowTextA("Mail");
	GetDlgItem(IDC_build)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_build)->SetWindowTextA("设为服务器（接收）");
	GetDlgItem(IDC_connect)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_connect)->SetWindowTextA("设为客户端（发送）");
	GetDlgItem(IDC_del)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_del)->SetWindowTextA("结束邮槽通信");
	GetDlgItem(IDC_Recive)->ShowWindow(SW_SHOW);
	runc.ShowWindow(SW_SHOW);
	frun=0;
	runc.SetCheck(false);
}
//选择管道
void CMFCApplication1Dlg::OnBnClickedRadio2()
{
	GetDlgItem(IDC_EDIT_COPYCB)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_Send)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_LIST1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATICp)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC4)->ShowWindow(SW_SHOW);
	now=2;
	GetDlgItem(IDC_STATIC)->SetWindowTextA("server:");
	GetDlgItem(IDC_STATICp)->SetWindowTextA("管道名:");
	cip.ShowWindow(SW_SHOW);
	cip.SetWindowTextA(".");
	cname.ShowWindow(SW_SHOW);
	cname.SetWindowTextA("MyPipe");
	GetDlgItem(IDC_build)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_build)->SetWindowTextA("创建管道");
	GetDlgItem(IDC_connect)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_connect)->SetWindowTextA("连接管道");
	GetDlgItem(IDC_del)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_del)->SetWindowTextA("断开链接");
	GetDlgItem(IDC_Recive)->ShowWindow(SW_SHOW);
	runc.ShowWindow(SW_SHOW);
	frun=0;
	runc.SetCheck(false);
}
//选择剪切板
void CMFCApplication1Dlg::OnBnClickedRadio3()
{
	GetDlgItem(IDC_EDIT_COPYCB)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT5)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_SHOW);
	now=3;
	GetDlgItem(IDC_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATICp)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT2)->ShowWindow(SW_HIDE);
	cip.ShowWindow(SW_HIDE);
	cname.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_build)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_connect)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_del)->ShowWindow(SW_HIDE);
	runc.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_Recive)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LIST1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE); 
	GetDlgItem(IDC_Send)->ShowWindow(SW_HIDE);
	frun=0;
}

//匿名管道
void CMFCApplication1Dlg::OnBnClickedRadio4()
{
	GetDlgItem(IDC_Send)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_LIST1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC4)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_COPYCB)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_HIDE);
	now=4;
	GetDlgItem(IDC_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATICp)->ShowWindow(SW_HIDE);
	cip.ShowWindow(SW_HIDE);
	cname.ShowWindow(SW_HIDE);
	if(fchild)
	{
		GetDlgItem(IDC_build)->ShowWindow(SW_HIDE);
		ni.connect();
	}
	else
	{
		GetDlgItem(IDC_build)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_build)->SetWindowTextA("创建子进程");
	}
	GetDlgItem(IDC_connect)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_del)->ShowWindow(SW_HIDE);
	runc.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_Recive)->ShowWindow(SW_SHOW);
	runc.ShowWindow(SW_SHOW);
	frun=0;
	runc.SetCheck(false);
}




void CMFCApplication1Dlg::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CMFCApplication1Dlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CMFCApplication1Dlg::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMFCApplication1Dlg::OnStnClickedStatic2()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMFCApplication1Dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!OpenClipboard())
	{
		AfxMessageBox(_T("Cannot open the Clipboard!"));
		return;
	}

	// 将剪贴板内容清空，释放数据资源，然后指定当前打开剪贴板的窗口为剪贴板的所有制
	if (!EmptyClipboard())
	{
		AfxMessageBox(_T("Cannot empty the Clipboard!"));
		return;
	}
	CEdit* pBoxOne;
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_COPYCB);
	// 取回当前控件的数据，hData开辟全局内存区域，存放数据
	CString str;
	pBoxOne->GetWindowText(str);
	size_t cbStr = (str.GetLength() + 1) * sizeof(TCHAR);
	HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE, cbStr);
	memcpy_s(GlobalLock(hData), cbStr, str.LockBuffer(), cbStr);
	GlobalUnlock(hData);
	str.UnlockBuffer();

	// 选择合适的格式
	UINT uiFormat = (sizeof(TCHAR) == sizeof(WCHAR)) ? CF_UNICODETEXT : CF_TEXT;
	if (::SetClipboardData(uiFormat, hData) == NULL)
	{
		AfxMessageBox(_T("Unable to set Clipboard data"));
		CloseClipboard();
		return;
	}
	CloseClipboard();
}


void CMFCApplication1Dlg::OnBnClickedButton3()
{
	if (!OpenClipboard())
	{
		AfxMessageBox(_T("Cannot open the Clipboard!"));
		return;
	}

	// 选择合适的格式（读取的时候不用获得剪贴板的拥有权）
	UINT uiFormat = (sizeof(TCHAR) == sizeof(WCHAR) ? CF_UNICODETEXT : CF_TEXT);
	HANDLE hData = ::GetClipboardData(uiFormat);
	if (NULL == hData)
	{
		AfxMessageBox(_T("Unable to get Clipboard data!"));
		CloseClipboard();
		return;
	}
	CString str;
	LPCTSTR lpdata = (LPCTSTR)GlobalLock(hData);
	CEdit* pBoxOne;
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT5);
	pBoxOne->SetWindowText(lpdata);
	GlobalUnlock(hData);
	CloseClipboard();
}
