#pragma once


// CDlgExplorerBrowser 대화 상자
class CExplorerCtrl;
class CDlgExplorerBrowser : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgExplorerBrowser)

public:
	CDlgExplorerBrowser(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgExplorerBrowser();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_BROWSER_EXPLORER };

private:
	CEdit m_editReturn;
	CStatic m_stcBrowser;
	CExplorerCtrl *m_pExplorerCtrl;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnEventToJavascript();
	afx_msg LRESULT OnEventFromJavascript(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
