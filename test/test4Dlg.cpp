
// test4Dlg.cpp : ���� ����
//

#include "stdafx.h"
#include "test4.h"
#include "test4Dlg.h"
#include "afxdialogex.h"
#include <afxwin.h>
#include <iostream>
#include <string>

using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ctest4Dlg ��ȭ ����


#define WM_USER_UPDATE_UI (WM_USER + 1)

Ctest4Dlg::Ctest4Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Ctest4Dlg::IDD, pParent)
	, m_strName(_T(""))
	, m_strInfo(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Ctest4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT_NAME,m_strName);
	DDX_Text(pDX,IDC_EDIT_INFO,m_strInfo);

}

BEGIN_MESSAGE_MAP(Ctest4Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ENLIST, &Ctest4Dlg::OnBnClickedButtonEnlist)
END_MESSAGE_MAP()


// Ctest4Dlg �޽��� ó����

BOOL Ctest4Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RegisterWindowMessage(_T("WM_USER_UPDATE_UI"));
	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	m_hMapFile = OpenFileMapping(FILE_MAP_READ || FILE_MAP_WRITE, FALSE, L"Global\\MySharedMemory");
    if (m_hMapFile == NULL) {
        cerr << "OpenFileMapping failed: " << GetLastError() << endl;
		m_strName = "OpenFileMapping failed";
		m_strInfo.Append(m_strName);
		m_strInfo.Append(_T("\r\n"));
		m_strName.Empty();
		UpdateData(false);
    }

    m_pBuf = (LPWSTR)MapViewOfFile(m_hMapFile, FILE_MAP_READ, 0, 0, 256);
    if (m_pBuf == NULL) {
		m_strName = "MapViewOfFile failed";
		m_strInfo.Append(m_strName);
		m_strInfo.Append(_T("\r\n"));
		m_strName.Empty();
        CloseHandle(m_hMapFile);
		UpdateData(false);
    }
	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void Ctest4Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR Ctest4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Ctest4Dlg::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void Ctest4Dlg::OnBnClickedButtonEnlist()
{
    UpdateData();

    // ������ ��� �Լ��� ���� ȣ���Ͽ� ���� �޸𸮿��� �����͸� ����
	if(m_pBuf == nullptr){
		m_strName = "";
	}else{
		m_strName = LPWSTRToCString(m_pBuf);
	}
	m_strInfo.Append(m_strName);
	m_strInfo.Append(_T("\r\n"));
	m_strName.Empty();

    // ������ ������Ʈ
    UpdateData(FALSE);
}

CString Ctest4Dlg::LPWSTRToCString(LPWSTR lpwstr) {
    CString str(lpwstr); // CString �����ڸ� �̿��Ͽ� LPWSTR�κ��� CString ����
    return str;
}
//void Ctest4Dlg::ReadFromSharedMemory(LPWSTR pBuf) {
    // �����尡 ���鼭 ���� �޸𸮿��� �����͸� �д� ����

	//m_strName = LPWSTRToCString(pBuf);
	//m_strInfo.Append(m_strName);
	//m_strInfo.Append(_T("\r\n"));
	//m_strName.Empty();
//}