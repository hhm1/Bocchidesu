// BocchidesuDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Bocchidesu.h"
#include "BocchidesuDlg.h"
#include "afxdialogex.h"
#include <windows.h>
#include "psapi.h"
#include"stdio.h"
#include <tlhelp32.h>

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

// CBocchidesuDlg 对话框

BEGIN_DHTML_EVENT_MAP(CBocchidesuDlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()

CBocchidesuDlg::CBocchidesuDlg(CWnd* pParent /*=nullptr*/)
	: CDHtmlDialog(IDD_BOCCHIDESU_DIALOG, IDR_HTML_BOCCHIDESU_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBocchidesuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBocchidesuDlg, CDHtmlDialog)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_BUTTON1, &CBocchidesuDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

// CBocchidesuDlg 消息处理程序

BOOL CBocchidesuDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

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
	::SetWindowPos(this->m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREPOSITION);

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBocchidesuDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDHtmlDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBocchidesuDlg::OnPaint()
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
		CDHtmlDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBocchidesuDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HRESULT CBocchidesuDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CBocchidesuDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}

void CBocchidesuDlg::OnBnClickedButton1()
{
	int i = 0;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		i += 0;
	}
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while (bMore)
	{
		//printf(" 进程名称：%s \n", pe32.szExeFile);
		if (_stricmp("nginx.exe", pe32.szExeFile) == 0)
		{
			//printf("进程运行中");
			i += 1;
		}
		bMore = ::Process32Next(hProcessSnap, &pe32);
	}
	if (i > 1) {           //大于1，弹窗提示正在运行，否则不在运行
		WinExec("killnginx.cmd", SW_HIDE);
		WinExec("hosts.exe", SW_HIDE);
		MessageBox(_T("关闭成功"), _T("提示"));
	}
	else 
	{
		WinExec("nginx.exe", SW_HIDE);
		if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1))     //IDC_CHECK1是CheckBox控件。
		{
			WinExec("cmd.exe /k cd comands && ao3.cmd", SW_HIDE);
		}
		if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK2))
		{
			WinExec("cmd.exe /k cd comands && exhentai.cmd", SW_HIDE);
		}
		if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK3))
		{
			WinExec("cmd.exe /k cd comands && pixiv.cmd", SW_HIDE);
		}
		if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK4))
		{
			WinExec("cmd.exe /k cd comands && discord.cmd", SW_HIDE);
		}
		if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK5))
		{
			WinExec("cmd.exe /k cd comands && github.cmd", SW_HIDE);
		}
		if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK6))
		{
			WinExec("cmd.exe /k cd comands && steam.cmd", SW_HIDE);
		}
		if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK7))
		{
			WinExec("cmd.exe /k cd comands && duckduckgo.cmd", SW_HIDE);
		}
		if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK8))
		{
			WinExec("cmd.exe /k cd comands && nyaa.cmd", SW_HIDE);
		}
		MessageBox("开启成功（不知道是不是成功了，如果不能访问就提交issues）");
	}
}