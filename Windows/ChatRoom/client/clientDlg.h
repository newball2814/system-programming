
// clientDlg.h : header file
//

#pragma once


// CclientDlg dialog
class CclientDlg : public CDialogEx
{
// Construction
public:
	CclientDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
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
	CButton sendBtn;
	CEdit chatbox;
	CEdit ipbox;
	CEdit portbox;
	CButton connectBtn;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();	
	WSADATA wsaData;
	struct addrinfo* result, *ptr, hints;
	SOCKET connectSocket;
};
