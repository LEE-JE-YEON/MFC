#pragma once


// CDlgRegistryAdd 대화 상자

class CDlgRegistryAdd : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRegistryAdd)

public:
	CDlgRegistryAdd(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgRegistryAdd();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_REGISTRY_ADD };

private:
	CEdit m_edtRegistryName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedRegistryAdd();
};
