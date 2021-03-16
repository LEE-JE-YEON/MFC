#pragma once


// CDlgRegistryDel 대화 상자

class CDlgRegistryDel : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRegistryDel)

public:
	CDlgRegistryDel(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgRegistryDel();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_REGISTRY_DEL };

private:
	CEdit m_edtRegistryName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRegistryDel();
};
