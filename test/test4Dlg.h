
// test4Dlg.h : ��� ����
//

#pragma once


// Ctest4Dlg ��ȭ ����
class Ctest4Dlg : public CDialogEx
{
// �����Դϴ�.
public:
	Ctest4Dlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_TEST4_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
