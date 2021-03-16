// DlgChangeLanguage.cpp: 구현 파일
//

#include "pch.h"
#include "DlgChangeLanguage.h"
#include "afxdialogex.h"
#include <fstream>


// CDlgChangeLanguage 대화 상자

IMPLEMENT_DYNAMIC(CDlgChangeLanguage, CDialogEx)

CDlgChangeLanguage::CDlgChangeLanguage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CDlgChangeLanguage::IDD, pParent)
{
}

CDlgChangeLanguage::~CDlgChangeLanguage()
{
}

void CDlgChangeLanguage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cboLanguage);
}


BEGIN_MESSAGE_MAP(CDlgChangeLanguage, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgChangeLanguage::OnBnClickedChange)
END_MESSAGE_MAP()


// CDlgChangeLanguage 메시지 처리기

BOOL CDlgChangeLanguage::OnInitDialog()
{
	BOOL bRet = CDialogEx::OnInitDialog();

	m_cboLanguage.AddString(g_JsonString.GetString(_T("PROGRAM_LANGUAGE_KO")));
	m_cboLanguage.AddString(g_JsonString.GetString(_T("PROGRAM_LANGUAGE_EN")));
	m_cboLanguage.AddString(g_JsonString.GetString(_T("PROGRAM_LANGUAGE_JP")));
	m_cboLanguage.SetCurSel(0);

	return bRet;
}

void CDlgChangeLanguage::OnBnClickedChange()
{
	BOOL bRet = FALSE;

	// 값 가져오기
	CString strCurLang;
	m_cboLanguage.GetLBText(m_cboLanguage.GetCurSel(), strCurLang);

	if (strCurLang.IsEmpty())
		return;

	TCHAR *szText;
	if (strCurLang == g_JsonString.GetString(_T("PROGRAM_LANGUAGE_KO")))
		szText = _T("[DLL]\nLANGUAGE=ko");
	else if (strCurLang == g_JsonString.GetString(_T("PROGRAM_LANGUAGE_EN")))
		szText = _T("[DLL]\nLANGUAGE=en");
	else if (strCurLang == g_JsonString.GetString(_T("PROGRAM_LANGUAGE_JP")))
		szText = _T("[DLL]\nLANGUAGE=jp");
	else
		return;
	
	// 경로 취득
	TCHAR szPath[MAX_PATH];
	memset(szPath, 0, MAX_PATH);
	GetModuleFileName(AfxGetInstanceHandle(), szPath, _MAX_PATH);
	PathRemoveFileSpec(szPath);
	_tcscat_s(szPath, _T("\\LanguageDll.ini"));

	if (_waccess(szPath, 0) != -1)		// 관제
	{
		std::wofstream ost(szPath, std::ofstream::out | std::ofstream::trunc);
		if (ost.is_open())
		{
			ost.write(szText, _tcslen(szText));
			bRet = TRUE;
		}
		ost.close();
	}

	if (bRet)
	{
		AfxMessageBox(g_JsonString.GetString(_T("LANGUAGE_CHANGE_SUCCESS")));
		EndDialog(IDOK);
	}
}