
// test4Dlg.h : 헤더 파일
//

#pragma once


// Ctest4Dlg 대화 상자
class Ctest4Dlg : public CDialogEx
{
// 생성입니다.
public:
	Ctest4Dlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TEST4_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	HANDLE m_hThread;
public:
	HANDLE m_hMapFile;
	LPWSTR m_pBuf;
	afx_msg void OnBnClickedButton1();
	CString m_strName;
	CString m_strInfo;
	afx_msg void OnBnClickedButtonEnlist();
	CString LPWSTRToCString(LPWSTR lpwstr);
};
