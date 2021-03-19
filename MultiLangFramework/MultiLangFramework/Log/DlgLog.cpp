// DlgRegistryAdd.cpp: 구현 파일
//

#include "pch.h"
#include "Log/DlgLog.h"


// CDlgLog 대화 상자

IMPLEMENT_DYNAMIC(CDlgLog, CDialogEx)

CDlgLog::CDlgLog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CDlgLog::IDD, pParent)
{

}

CDlgLog::~CDlgLog()
{
}

void CDlgLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgLog::OnBnClickedLogOpen)
END_MESSAGE_MAP()


// CDlgLog 메시지 처리기

void CDlgLog::OnBnClickedLogOpen()
{
}