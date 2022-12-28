
// AntRanGUIDlg.h : header file
//

#pragma once

#include "FilterClient.h"

#define WM_TRAY_NOTIFYICACTION WM_APP + 10
#define TRAY_BALLOON 0

// CAntRanGUIDlg dialog
class CAntRanGUIDlg : public CDialogEx
{
// Construction
public:
	CAntRanGUIDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANTRANGUI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	FilterClient client;


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
