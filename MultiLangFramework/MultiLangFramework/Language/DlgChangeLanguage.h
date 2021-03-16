#pragma once


// CDlgChangeLanguage 대화 상자

class CDlgChangeLanguage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgChangeLanguage)

public:
	CDlgChangeLanguage(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgChangeLanguage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LANGUAGE_CHANGE };

private:
	CComboBox m_cboLanguage;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedChange();
};
