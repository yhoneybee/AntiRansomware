
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
	ON_BN_CLICKED(IDC_BUTTON1, &CAntRanGUIDlg::OnBnClickedAddTray)
	ON_BN_CLICKED(IDC_BUTTON2, &CAntRanGUIDlg::OnBnClickedShowBallon)
	ON_BN_CLICKED(IDC_BUTTON3, &CAntRanGUIDlg::OnBnClickedSubTray)
	ON_MESSAGE(WM_TRAY_NOTIFYICACTION, &CAntRanGUIDlg::OnTrayIconNotification)
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


void CAntRanGUIDlg::OnBnClickedAddTray()
{
	tray_icon_controller.CreateTrayIcon();
	// TODO: Add your control notification handler code here
}

void CAntRanGUIDlg::OnBnClickedShowBallon()
{
	// TODO: Add your control notification handler code here
}

void CAntRanGUIDlg::OnBnClickedSubTray()
{
	tray_icon_controller.DestroyTrayIcon();
	// TODO: Add your control notification handler code here
}
