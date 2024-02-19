
// ProcessViewerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ProcessViewer.h"
#include "ProcessViewerDlg.h"
#include "afxdialogex.h"

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <string>

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


// CProcessViewerDlg dialog



CProcessViewerDlg::CProcessViewerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROCESSVIEWER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProcessViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, refreshBtn);
	DDX_Control(pDX, IDC_LIST1, processList);
}

BEGIN_MESSAGE_MAP(CProcessViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CProcessViewerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CProcessViewerDlg message handlers

BOOL CProcessViewerDlg::OnInitDialog()
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
	processList.SetExtendedStyle(LVS_EX_GRIDLINES);

	CRect rect;
	processList.GetClientRect(rect);
	processList.InsertColumn(0, _T("PID"), 0, rect.Width() / 4);
	processList.InsertColumn(1, _T("Process Name"), 0, rect.Width() / 4);
	processList.InsertColumn(2, _T("Command Line"), 0, rect.Width() / 4);
	processList.InsertColumn(3, _T("Full Path"), 0, rect.Width() / 4);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CProcessViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CProcessViewerDlg::OnPaint()
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
HCURSOR CProcessViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CProcessViewerDlg::OnBnClickedButton1()
{
	processList.DeleteAllItems(); // Clear existing items from the list

	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		return;
	}

	// Initialize process entry structure
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32)) {
		CloseHandle(hProcessSnap);
		return;
	}

	int index = 0; // Index to insert items into the list

	do {
		CString pid, processName, commandLine, fullPath;

		pid.Format(_T("%d"), pe32.th32ProcessID);
		processName = pe32.szExeFile;

		// Get a handle to the process
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
		if (hProcess != NULL) {
			// Get the full path of the process
			char szProcessPath[MAX_PATH];
			DWORD dwSize = sizeof(szProcessPath);
			if (QueryFullProcessImageNameA(hProcess, 0, szProcessPath, &dwSize)) {
				fullPath = szProcessPath;
			}
			else {
				fullPath = _T("Failed to retrieve full path.");
			}

			// Retrieve the command line of the process
			commandLine = GetCommandLineA();

			// Close the process handle
			CloseHandle(hProcess);
		}
		else {
			fullPath = _T("Failed to open process.");
			commandLine = _T("Failed to open process.");
		}

		// Insert the process information into the list
		processList.InsertItem(index, pid);
		processList.SetItemText(index, 1, processName);
		processList.SetItemText(index, 2, commandLine);
		processList.SetItemText(index, 3, fullPath);

		index++;
	} while (Process32Next(hProcessSnap, &pe32));

	// Close the snapshot handle
	CloseHandle(hProcessSnap);
}
