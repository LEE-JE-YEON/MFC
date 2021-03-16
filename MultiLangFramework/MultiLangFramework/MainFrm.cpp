
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MultiLangFramework.h"
#include "MainFrm.h"
#include "Registry/DlgRegistryAdd.h"
#include "Registry/DlgRegistryDel.h"
#include "Language/DlgChangeLanguage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_APP_ABOUT, &CMainFrame::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CMainFrame::OnFileNew)
	ON_COMMAND(ID_REGISTRY_ADD, &CMainFrame::OnRegistryAdd)
	ON_COMMAND(ID_REGISTRY_DELETE, &CMainFrame::OnRegistryDel)
	ON_COMMAND(ID_LANGUAGE_CHANGE, &CMainFrame::OnLanguageChange)
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SetWindowText(g_JsonString.GetString(_T("PROGRAM_TITLE")));

	return CMDIFrameWnd::OnCreate(lpCreateStruct);
}

void CMainFrame::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CMainFrame::OnFileNew()
{
	AfxMessageBox(g_JsonString.GetString(_T("PROGRAM_TITLE")));
}

void CMainFrame::OnRegistryAdd()
{
	CDlgRegistryAdd dlg;
	dlg.DoModal();
}

void CMainFrame::OnRegistryDel()
{
	CDlgRegistryDel dlg;
	dlg.DoModal();
}

void CMainFrame::OnLanguageChange()
{
	CDlgChangeLanguage dlg;
	dlg.DoModal();
}