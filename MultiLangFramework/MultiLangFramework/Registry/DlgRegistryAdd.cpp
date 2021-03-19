// DlgRegistryAdd.cpp: 구현 파일
//

#include "pch.h"
#include "DlgRegistryAdd.h"
#include "Registry.h"


// CDlgRegistryAdd 대화 상자

IMPLEMENT_DYNAMIC(CDlgRegistryAdd, CDialogEx)

CDlgRegistryAdd::CDlgRegistryAdd(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CDlgRegistryAdd::IDD, pParent)
{

}

CDlgRegistryAdd::~CDlgRegistryAdd()
{
}

void CDlgRegistryAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edtRegistryName);
}


BEGIN_MESSAGE_MAP(CDlgRegistryAdd, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgRegistryAdd::OnBnClickedRegistryAdd)
END_MESSAGE_MAP()


// CDlgRegistryAdd 메시지 처리기

void CDlgRegistryAdd::OnBnClickedRegistryAdd()
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
		AfxMessageBox(g_JsonString.GetString(_T("REGISTRY_ALREADY_REG")));
	}
	else
	{
		reg.SetValue(strAbsPathName, strExeName);
		AfxMessageBox(g_JsonString.GetString(_T("REGISTRY_SUCCESS")));
	}
	reg.CloseKey();
}