
// serverDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "server.h"
#include "serverDlg.h"
#include "afxdialogex.h"

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <atlstr.h>
#include <ctime>
#include <string>

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


// CserverDlg dialog



CserverDlg::CserverDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CserverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST5, log);
	DDX_Control(pDX, IDC_LIST6, state);
	DDX_Control(pDX, IDC_EDIT1, ipbox);
	DDX_Control(pDX, IDC_EDIT2, portbox);
	DDX_Control(pDX, IDC_BUTTON1, startBtn);
}

BEGIN_MESSAGE_MAP(CserverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CserverDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CserverDlg message handlers

BOOL CserverDlg::OnInitDialog()
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
	state.SetExtendedStyle(LVS_EX_GRIDLINES);
	log.SetExtendedStyle(LVS_EX_GRIDLINES);

	CRect rect;
	state.GetClientRect(rect);
	log.GetClientRect(rect);

	state.InsertColumn(0, _T("IP"), 0, rect.Width() / 2);
	state.InsertColumn(1, _T("Port"), 0, rect.Width() / 2);

	log.InsertColumn(0, _T("Time"), 0, rect.Width() / 2);
	log.InsertColumn(1, _T("Activity"), 0, rect.Width() / 2);
	
	// Init winsock
	int i;
	if ((i = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		AfxMessageBox(_T("Unable to initialize winsock!!"), MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	result = NULL;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CserverDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CserverDlg::OnPaint()
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
HCURSOR CserverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Get current time
CString CserverDlg::GetTime() {
	// Get current time
	time_t now = time(0);
	struct tm timeinfo;
	localtime_s(&timeinfo, &now);

	// Format time as a string
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);

	// Convert to CString
	CString currentTime(buffer);

	return currentTime;
}
void CserverDlg::HandleClient(SOCKET clientSocket) {
	char buffer[2048];
	int i, index = log.GetItemCount();

	do {
		// Receive data from client
		i = recv(clientSocket, buffer, 2048, 0);
		if (i > 0) {
			buffer[i] = '\0';

			// Echo back to client
			i = send(clientSocket, buffer, i, 0);
			if (i == SOCKET_ERROR)				
				break;			
		}
		else if (i == 0) {
			CString time = GetTime();
			log.InsertItem(index, time);
			log.SetItemText(index, 1, _T("Client disconnected"));
		}
		else {			
			break;
		}

		++index;
	} while (i > 0);
}

void CserverDlg::OnBnClickedButton1()
{
	CString IP, port;
	int i;
	ipbox.GetWindowTextW(IP);
	portbox.GetWindowText(port);

	// Somehow I cannot add CString port to socket functions
	// hence the hard-coded port number 8081 :v
	if ((i = getaddrinfo(NULL, "8081", &hints, &result)) != 0) {
		AfxMessageBox(_T("getaddrinfo failed!!"), MB_OK | MB_ICONERROR);
		WSACleanup();
		return;
	}

	SOCKET listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET) {		
		AfxMessageBox(_T("Error at socket!!"), MB_OK | MB_ICONERROR);
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	// Listen for connection request
	if ((i = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen)) == SOCKET_ERROR) {
		AfxMessageBox(_T("Failed to bind!!"), MB_OK | MB_ICONERROR);
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return;
	}

	freeaddrinfo(result);

	if ((i = listen(listenSocket, SOMAXCONN)) == SOCKET_ERROR) {
		AfxMessageBox(_T("Listen failed!!"), MB_OK | MB_ICONERROR);
		closesocket(listenSocket);
		WSACleanup();
		return;
	}

	startBtn.SetWindowTextW(_T("STOP"));

	SOCKET clientSocket;
	while ((clientSocket = accept(listenSocket, NULL, NULL)) != INVALID_SOCKET) {
		HandleClient(clientSocket);
	}

	startBtn.SetWindowTextW(_T("START"));
	closesocket(listenSocket);
	WSACleanup();
}
