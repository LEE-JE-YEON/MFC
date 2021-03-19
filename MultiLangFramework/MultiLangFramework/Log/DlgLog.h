#pragma once


// CDlgRegistryAdd 대화 상자

class CDlgLog : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLog)

public:
	CDlgLog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgLog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOG };

private:
	CEdit m_edtRegistryName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedLogOpen();
};
