
// MultiLangFramework.cpp: 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "MultiLangFramework.h"
#include "MainFrm.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()



// CMyFrameWork2App

BEGIN_MESSAGE_MAP(CMyFrameWork2App, CWinApp)
END_MESSAGE_MAP()


// CMyFrameWork2App 생성

CMyFrameWork2App::CMyFrameWork2App() noexcept
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 응용 프로그램을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 응용 프로그램 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MultiLangFramework.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CMyFrameWork2App 개체입니다.

CMyFrameWork2App theApp;


// CMyFrameWork2App 초기화

BOOL CMyFrameWork2App::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면 AfxInitRichEdit2()가 있어야 합니다.
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));

	// 언어별 DLL 적용
	if (!_initLanguage())
		return FALSE;

	// 주 창을 만들기 위해 이 코드에서는 새 프레임 창 개체를
	// 만든 다음 이를 응용 프로그램의 주 창 개체로 설정합니다.
	CMDIFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// 주 MDI 프레임 창을 만듭니다.
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	// 주 창이 초기화되었으므로 이를 표시하고 업데이트합니다.
	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();

	return TRUE;
}

BOOL CMyFrameWork2App::_initLanguage()
{
	// 언어별 리소스 DLL 설정
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	PathRemoveFileSpec(szPath);

	CString strPath(szPath);

	TCHAR szDll[MAX_PATH] = { 0, };
	if (GetPrivateProfileString(_T("DLL"), _T("LANGUAGE"), _T("0"), szDll, MAX_PATH, strPath + _T("\\LanguageDll.ini")) < 1)
	{
		return FALSE;
	}

	LANGID CurLangID = PRIMARYLANGID(GetUserDefaultUILanguage());
	HINSTANCE hInstance;

	if (_tccmp(szDll, _T("ko")) == 0)
		CurLangID = LANG_KOREAN;
	else if (_tccmp(szDll, _T("jp")) == 0)
		CurLangID = LANG_JAPANESE;
	else if (_tccmp(szDll, _T("en")) == 0)
		CurLangID = LANG_ENGLISH;
	else
		CurLangID = LANG_KOREAN;

	g_LoginInfo.SetCurLangID(CurLangID);

	switch (CurLangID)
	{
	case LANG_KOREAN:
		strPath += _T("\\MultiLangFrameworkRes_ko.dll");
		break;
	case LANG_JAPANESE:
		strPath += _T("\\MultiLangFrameworkRes_jp.dll");
		break;
	case LANG_ENGLISH:
		strPath += _T("\\MultiLangFrameworkRes_en.dll");
		break;
	default:
		strPath += _T("\\MultiLangFrameworkRes_ko.dll");
		break;
	}
	hInstance = LoadLibrary(strPath);
	if (hInstance != NULL)
		AfxSetResourceHandle(hInstance);
	else
		return FALSE;

	// 설정한 리소스 DLL에서 JSON string table 로드
	return g_JsonString.Load();
}

int CMyFrameWork2App::ExitInstance()
{
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}