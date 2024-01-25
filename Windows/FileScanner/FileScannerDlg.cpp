#include "pch.h"
#include "framework.h"
#include "FileScanner.h"
#include "FileScannerDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <filesystem>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// using namespace std;
namespace fs = std::filesystem;

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


// CFileScannerDlg dialog



CFileScannerDlg::CFileScannerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILESCANNER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileScannerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, editBox1);
	DDX_Control(pDX, IDC_EDIT2, editBox2);
	DDX_Control(pDX, IDC_LIST1, listCtrl);
}

BEGIN_MESSAGE_MAP(CFileScannerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CFileScannerDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDOK, &CFileScannerDlg::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CFileScannerDlg::OnLvnItemchangedList1)
END_MESSAGE_MAP()


// CFileScannerDlg message handlers

BOOL CFileScannerDlg::OnInitDialog()
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
	
	// Set listCtrl in grid mode
	listCtrl.SetExtendedStyle(LVS_EX_GRIDLINES);

	CRect rect;
	listCtrl.GetClientRect(rect);
	listCtrl.InsertColumn(0, _T("-"), 0, rect.Width() / 2);
	listCtrl.InsertColumn(1, _T("-"), 0, rect.Width() / 2);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFileScannerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFileScannerDlg::OnPaint()
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
HCURSOR CFileScannerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFileScannerDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here	
}

void CFileScannerDlg::OnBnClickedOk()
{
	CString path, fileName, fullFilePath;

	// Get edit box content
	editBox1.GetWindowTextW(path);
	editBox2.GetWindowTextW(fileName);

	// Absolute file path
	fullFilePath.Append(path);
	fullFilePath.Append(_T("/"));
	fullFilePath.Append(fileName);
	
	fs::path fileIn(fullFilePath.GetString());
	// int fSize = fs::file_size(fullFilePath.GetString());

	if (!fs::exists(path.GetString())) 
	{
		AfxMessageBox(_T("Path not exist."), MB_OK | MB_ICONERROR);
		return;
	}

	if (!fs::is_directory(path.GetString())) 
	{
		AfxMessageBox(_T("Path provided is not a directory."), MB_OK | MB_ICONERROR);
		return;
	}

	// fileSize.Format(_T("%d"), fSize);
	// listCtrl.InsertItem(0, fileSize);

	const int KB = 1024;
	const int MB = KB * 1024;
	const int GB = MB * 1024;

	// Display file info to listControl
	try 
	{		
		// Get file attributes
		WIN32_FILE_ATTRIBUTE_DATA fileAttributes;
		if (GetFileAttributesEx(fullFilePath, GetFileExInfoStandard, &fileAttributes))
		{			
			UpdateData();
			int nIndex = listCtrl.GetItemCount();
			// Insert file name
			listCtrl.InsertItem(nIndex, _T("File Name"));
			listCtrl.SetItemText(nIndex, 1, fileIn.filename().c_str());

			// Insert file size
			listCtrl.InsertItem(++nIndex, _T("File Size"));
			CString fileSizeWithUnit;			

			if (fileAttributes.nFileSizeLow < KB)
				fileSizeWithUnit.Format(_T("%d B"), fileAttributes.nFileSizeLow);
			else if (fileAttributes.nFileSizeLow < MB)
				fileSizeWithUnit.Format(_T("%.2f KB"), static_cast<double>(fileAttributes.nFileSizeLow) / KB);
			else if (fileAttributes.nFileSizeLow < GB)
				fileSizeWithUnit.Format(_T("%.2f MB"), static_cast<double>(fileAttributes.nFileSizeLow) / MB);
			else
				fileSizeWithUnit.Format(_T("%.2f GB"), static_cast<double>(fileAttributes.nFileSizeLow) / GB);

			listCtrl.SetItemText(nIndex, 1, fileSizeWithUnit);

			// Insert full path
			listCtrl.InsertItem(++nIndex, _T("Full Path"));
			listCtrl.SetItemText(nIndex, 1, fileIn.c_str());

			// Insert date modified
			SYSTEMTIME st;
			FileTimeToSystemTime(&fileAttributes.ftLastWriteTime, &st);
			CString formattedDate;
			formattedDate.Format(_T("%02d/%02d/%04d %02d:%02d:%02d"),
				st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
			listCtrl.InsertItem(++nIndex, _T("Date Modified"));
			listCtrl.SetItemText(nIndex, 1, formattedDate);

			// Insert date created
			SYSTEMTIME stCreated;
			FileTimeToSystemTime(&fileAttributes.ftCreationTime, &stCreated);
			CString formattedDateCreated;
			formattedDateCreated.Format(_T("%02d/%02d/%04d %02d:%02d:%02d"),
				stCreated.wDay, stCreated.wMonth, stCreated.wYear,
				stCreated.wHour, stCreated.wMinute, stCreated.wSecond);
			listCtrl.InsertItem(++nIndex, _T("Date Created"));
			listCtrl.SetItemText(nIndex, 1, formattedDateCreated);
		}
	} 
	catch (const std::filesystem::filesystem_error& e) 
	{
		AfxMessageBox(_T("Error occured :P"), MB_OK | MB_ICONERROR);
	}
}

void CFileScannerDlg::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{

}
