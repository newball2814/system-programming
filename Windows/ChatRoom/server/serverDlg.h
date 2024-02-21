
// serverDlg.h : header file
//

#pragma once


// CserverDlg dialog
class CserverDlg : public CDialogEx
{
// Construction
public:
	CserverDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl log;	
	CListCtrl state;
	WSADATA wsaData;
	SOCKET clientSocket;
	struct addrinfo* result, hints;
	afx_msg void OnBnClickedButton1();
	void HandleClient(SOCKET clientSocket);
	CString GetTime();
	CEdit ipbox;
	CEdit portbox;
	CButton startBtn;
};
