
// StudyMFCDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "StudyMFC.h"
#include "StudyMFCDlg.h"
#include "afxdialogex.h"
#include <winioctl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEVICE_SEND CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_WRITE_DATA)
#define DEVICE_READ CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_READ_DATA)

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


// CStudyMFCDlg dialog



CStudyMFCDlg::CStudyMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STUDYMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStudyMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStudyMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CStudyMFCDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CStudyMFCDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CStudyMFCDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CStudyMFCDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CStudyMFCDlg message handlers

BOOL CStudyMFCDlg::OnInitDialog()
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CStudyMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CStudyMFCDlg::OnPaint()
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
HCURSOR CStudyMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HANDLE device_handle = nullptr;

void CStudyMFCDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	device_handle = CreateFile(_T("\\\\.\\mysymbol"), GENERIC_ALL, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);
	if (device_handle == INVALID_HANDLE_VALUE)
	{
		TCHAR msg[1024];
		StringCchPrintf(msg, 1024, _T("not valid value: %d"), GetLastError());
		MessageBox(msg, 0, 0);
		return;
	}

	MessageBox(_T("valid value"), 0, 0);
}


void CStudyMFCDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	if (device_handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(device_handle);
	}
}


void CStudyMFCDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here

	LPTSTR message = _T("send sample from mfc");
	DWORD return_length = 0;

	if (device_handle == nullptr || device_handle == INVALID_HANDLE_VALUE)
	{
		return;
	}

	if (DeviceIoControl(device_handle, DEVICE_SEND, message, 21 * sizeof(WCHAR), nullptr, 0, &return_length, 0) == false)
	{
		MessageBox(_T("DeviceIoControl failed"), 0, 0);
	}
	else
	{
		MessageBox(message, 0, 0);
	}
}


void CStudyMFCDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	TCHAR message[1024] = { 0, };
	DWORD return_length = 0;

	if (device_handle == nullptr || device_handle == INVALID_HANDLE_VALUE)
	{
		return;
	}

	if (DeviceIoControl(device_handle, DEVICE_READ, nullptr, 0, message, 1024, &return_length, 0) == false)
	{
		MessageBox(_T("DeviceIoControl failed"), 0, 0);
	}
	else
	{
		MessageBox(message, 0, 0);
	}
}
