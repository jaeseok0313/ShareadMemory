
// test4Dlg.cpp : 구현 파일
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


// Ctest4Dlg 대화 상자


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


// Ctest4Dlg 메시지 처리기

BOOL Ctest4Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RegisterWindowMessage(_T("WM_USER_UPDATE_UI"));
	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

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
	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void Ctest4Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR Ctest4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Ctest4Dlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Ctest4Dlg::OnBnClickedButtonEnlist()
{
    UpdateData();

    // 쓰레드 대신 함수를 직접 호출하여 공유 메모리에서 데이터를 읽음
	if(m_pBuf == nullptr){
		m_strName = "";
	}else{
		m_strName = LPWSTRToCString(m_pBuf);
	}
	m_strInfo.Append(m_strName);
	m_strInfo.Append(_T("\r\n"));
	m_strName.Empty();

    // 데이터 업데이트
    UpdateData(FALSE);
}

CString Ctest4Dlg::LPWSTRToCString(LPWSTR lpwstr) {
    CString str(lpwstr); // CString 생성자를 이용하여 LPWSTR로부터 CString 생성
    return str;
}
//void Ctest4Dlg::ReadFromSharedMemory(LPWSTR pBuf) {
    // 쓰레드가 돌면서 공유 메모리에서 데이터를 읽는 예제

	//m_strName = LPWSTRToCString(pBuf);
	//m_strInfo.Append(m_strName);
	//m_strInfo.Append(_T("\r\n"));
	//m_strName.Empty();
//}