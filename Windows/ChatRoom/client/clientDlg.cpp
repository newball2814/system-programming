
// clientDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "client.h"
#include "clientDlg.h"
#include "afxdialogex.h"

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

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


// CclientDlg dialog



CclientDlg::CclientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT3, sendBtn);
	DDX_Control(pDX, IDC_BUTTON2, sendBtn);
	DDX_Control(pDX, IDC_EDIT3, chatbox);
	DDX_Control(pDX, IDC_EDIT1, ipbox);
	DDX_Control(pDX, IDC_EDIT2, portbox);
	DDX_Control(pDX, IDC_BUTTON1, connectBtn);
}

BEGIN_MESSAGE_MAP(CclientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CclientDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CclientDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CclientDlg message handlers

BOOL CclientDlg::OnInitDialog()
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

	int i;
	if ((i = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		AfxMessageBox(_T("WSAStartup failed."), MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	result = NULL, ptr = NULL;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CclientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CclientDlg::OnPaint()
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
HCURSOR CclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CclientDlg::OnBnClickedButton1()
{
	CString ip, port;
	int i;

	ipbox.GetWindowTextW(ip);
	portbox.GetWindowTextW(port);

	if ((i = getaddrinfo("localhost", "8081", &hints, &result)) != 0) {
		AfxMessageBox(_T("getaddrinfo failed"), MB_OK | MB_ICONERROR);
		WSACleanup();
		return;
	}

	// Try to connect til succeeds
	SOCKET connectSocket = INVALID_SOCKET;
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		// Create a SOCKET for connecting to server
		connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (connectSocket == INVALID_SOCKET) {
			AfxMessageBox(_T("Socket failed"), MB_OK | MB_ICONERROR);
			WSACleanup();
			return;
		}

		// Connect to server
		if ((i = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen)) == SOCKET_ERROR) {
			closesocket(connectSocket);
			connectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (connectSocket == INVALID_SOCKET) {
		AfxMessageBox(_T("Failed to connect to server"), MB_OK | MB_ICONERROR);
		WSACleanup();
		return;
	}

	// Now we're connected, we can move on to communicating with server
}


void CclientDlg::OnBnClickedButton2()
{
	char recv_msg[4096];
	int bytesReceived, i;
	CString message;

	chatbox.GetWindowTextW(message);

	CStringA m(message);
	// Send message to server		
	if ((i = send(connectSocket, m.GetString(), message.GetLength(), 0)) == SOCKET_ERROR) {
		AfxMessageBox(_T("Send failed"), MB_OK | MB_ICONERROR);
		closesocket(connectSocket);
		WSACleanup();
		return;
	}

	// Receive response
	bytesReceived = recv(connectSocket, recv_msg, 4096, 0);

	if (bytesReceived > 0) {
		recv_msg[bytesReceived] = '\0';
	}
	else if (bytesReceived == 0) {
		std::cout << "Connection closed by server" << std::endl;
		AfxMessageBox(_T("Connection close by server"), MB_OK | MB_ICONINFORMATION);
		return;
	}
	else {
		AfxMessageBox(_T("Receive failed"), MB_OK | MB_ICONERROR);
		return;
	}	
}
