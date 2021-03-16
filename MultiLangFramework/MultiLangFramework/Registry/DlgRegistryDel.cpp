// DlgRegistryDel.cpp: 구현 파일
//

#include "pch.h"
#include "DlgRegistryDel.h"
#include "afxdialogex.h"
#include "Registry.h"


// CDlgRegistryDel 대화 상자

IMPLEMENT_DYNAMIC(CDlgRegistryDel, CDialogEx)

CDlgRegistryDel::CDlgRegistryDel(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CDlgRegistryDel::IDD, pParent)
{

}

CDlgRegistryDel::~CDlgRegistryDel()
{
}

void CDlgRegistryDel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edtRegistryName);
}


BEGIN_MESSAGE_MAP(CDlgRegistryDel, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgRegistryDel::OnBnClickedRegistryDel)
END_MESSAGE_MAP()


// CDlgRegistryDel 메시지 처리기

BOOL CDlgRegistryDel::OnInitDialog()
{
	BOOL bRet = CDialogEx::OnInitDialog();

	CRegistry reg;

	// 경로 취득
	TCHAR szTemp[MAX_PATH];
	memset(szTemp, 0, MAX_PATH);
	GetModuleFileName(AfxGetInstanceHandle(), szTemp, _MAX_PATH);
	CString strPathName;
	strPathName.Format(_T("%s.FriendlyAppName"), szTemp);

	memset(szTemp, 0, MAX_PATH);
	_tfullpath(szTemp, strPathName, MAX_PATH);
	CString strAbsPathName = szTemp;

	// 존재 여부 확인 후, 등록
	CString strValue;

	// 레지스트리키 확인
	if (reg.OpenKey(HKEY_CURRENT_USER, _T("Software\\Classes\\Local Settings\\Software\\Microsoft\\Windows\\Shell\\MuiCache")))
	{
		reg.GetValue(strAbsPathName, strValue);
	}

	m_edtRegistryName.SetWindowText(strValue);

	reg.CloseKey();

	return bRet;
}

void CDlgRegistryDel::OnBnClickedRegistryDel()
{
	// 레지스트명 확인
	CString strExeName;
	m_edtRegistryName.GetWindowText(strExeName);
	if (strExeName.IsEmpty())
	{
		AfxMessageBox(g_JsonString.GetString(_T("REGISTRY_FAIL")));
		return;
	}

	CRegistry reg;

	// 경로 취득
	TCHAR szTemp[MAX_PATH];
	memset(szTemp, 0, MAX_PATH);
	GetModuleFileName(AfxGetInstanceHandle(), szTemp, _MAX_PATH);
	CString strPathName;
	strPathName.Format(_T("%s.FriendlyAppName"), szTemp);

	memset(szTemp, 0, MAX_PATH);
	_tfullpath(szTemp, strPathName, MAX_PATH);
	CString strAbsPathName = szTemp;

	// 레지스트리키 확인
	if (!reg.OpenKey(HKEY_CURRENT_USER, _T("Software\\Classes\\Local Settings\\Software\\Microsoft\\Windows\\Shell\\MuiCache")))
	{
		AfxMessageBox(g_JsonString.GetString(_T("REGISTRY_FAIL")));
		reg.CloseKey();
		return;
	}

	// 존재 여부 확인 후, 등록
	CString strValue;
	reg.GetValue(strAbsPathName, strValue);
	if (strValue == strExeName)
	{
		if (reg.DeleteValue(strExeName))
			AfxMessageBox(g_JsonString.GetString(_T("REGISTRY_SUCCESS")));
		else
			AfxMessageBox(g_JsonString.GetString(_T("REGISTRY_FAIL")));
	}
	else
	{
		AfxMessageBox(g_JsonString.GetString(_T("REGISTRY_FAIL")));
	}
	reg.CloseKey();
}