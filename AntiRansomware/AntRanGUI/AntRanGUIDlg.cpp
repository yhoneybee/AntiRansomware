
// AntRanGUIDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "AntRanGUI.h"
#include "AntRanGUIDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CAntRanGUIDlg dialog



CAntRanGUIDlg::CAntRanGUIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ANTRANGUI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAntRanGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, is_running_background);
}

LRESULT CAntRanGUIDlg::OnTrayIconNotification(WPARAM wparam, LPARAM lparam)
{
	tray_icon_controller.OnTrayNotification(wparam, lparam);
	return 0;
}

BEGIN_MESSAGE_MAP(CAntRanGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CAntRanGUIDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAntRanGUIDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON2, &CAntRanGUIDlg::OnBnClickedShowBallon)
	ON_MESSAGE(WM_TRAY_NOTIFYICACTION, &CAntRanGUIDlg::OnTrayIconNotification)
	ON_BN_CLICKED(IDC_BUTTON4, &CAntRanGUIDlg::OnBnClickedInstall)
	ON_BN_CLICKED(IDC_BUTTON5, &CAntRanGUIDlg::OnBnClickedUninstall)
	ON_BN_CLICKED(IDC_BUTTON6, &CAntRanGUIDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_BUTTON7, &CAntRanGUIDlg::OnBnClickedStop)
	ON_EN_CHANGE(IDC_EDIT1, &CAntRanGUIDlg::OnEnChangeFilter)
END_MESSAGE_MAP()


// CAntRanGUIDlg message handlers

BOOL CAntRanGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	tray_icon_controller.SetHWND(GetSafeHwnd());

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAntRanGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if (is_running_background.GetCheck())
	{
		if (nID == SC_CLOSE || nID == SC_MINIMIZE)
		{
			tray_icon_controller.CreateTrayIcon();
			AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAntRanGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAntRanGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



LRESULT CAntRanGUIDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class



	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CAntRanGUIDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CAntRanGUIDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CAntRanGUIDlg::OnBnClickedShowBallon()
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_EDIT2, str);
	tray_icon_controller.CreateBalloon(NIIF_NONE, _T("TITLE TEST"), str);
}

void CAntRanGUIDlg::OnBnClickedInstall()
{
	// TODO: Add your control notification handler code here
	service.InstallFromINF();
}


void CAntRanGUIDlg::OnBnClickedUninstall()
{
	// TODO: Add your control notification handler code here
	service.Uninstall();
}


void CAntRanGUIDlg::OnBnClickedStart()
{
	// TODO: Add your control notification handler code here
	service.Start();
	client.Connect();
}


void CAntRanGUIDlg::OnBnClickedStop()
{
	// TODO: Add your control notification handler code here

	client.Disconnect();
	service.Stop();
}


void CAntRanGUIDlg::OnEnChangeFilter()
{
	CString str;
	GetDlgItemText(IDC_EDIT1, str);
	client.SetBlockedExtend(str);
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

