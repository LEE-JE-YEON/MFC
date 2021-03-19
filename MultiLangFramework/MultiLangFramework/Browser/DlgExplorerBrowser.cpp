// DlgExplorerBrowser.cpp: 구현 파일
//

#include "pch.h"
#include "Browser/DlgExplorerBrowser.h"
#include "ExplorerCtrl.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using namespace rapidjson;


// CDlgExplorerBrowser 대화 상자

IMPLEMENT_DYNAMIC(CDlgExplorerBrowser, CDialogEx)

CDlgExplorerBrowser::CDlgExplorerBrowser(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CDlgExplorerBrowser::IDD, pParent)
{
}

CDlgExplorerBrowser::~CDlgExplorerBrowser()
{
}

void CDlgExplorerBrowser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_editReturn);
	DDX_Control(pDX, IDC_STATIC1, m_stcBrowser);
}


BEGIN_MESSAGE_MAP(CDlgExplorerBrowser, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgExplorerBrowser::OnBnEventToJavascript)
	ON_MESSAGE(UWM_EVENT_FROM_JAVASCRIPT_EXPOLRER, &CDlgExplorerBrowser::OnEventFromJavascript)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgExplorerBrowser 메시지 처리기

BOOL CDlgExplorerBrowser::OnInitDialog()
{
	BOOL bRet = CDialogEx::OnInitDialog();

	CRect rect;
	m_stcBrowser.GetWindowRect(rect);
	ScreenToClient(rect);

	m_pExplorerCtrl = new CExplorerCtrl(this);
	m_pExplorerCtrl->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, this, m_stcBrowser.GetDlgCtrlID());
	m_pExplorerCtrl->LoadFromResource(IDR_HTML1);

	m_stcBrowser.DestroyWindow();

	return bRet;
}

void CDlgExplorerBrowser::OnBnEventToJavascript()
{
	// TODO javascript로 넘겨줄 JSON 데이터 생성
	CString strJson;

	// html의 javascript 함수 호출
	m_pExplorerCtrl->OnEventToJavascript(strJson);
}

LRESULT CDlgExplorerBrowser::OnEventFromJavascript(WPARAM wParam, LPARAM lParam)
{
	// javascript에서 호출받은 함수
	CString *pstrParam = (CString *)lParam;
	if (pstrParam && !pstrParam->IsEmpty())
	{
		m_editReturn.SetWindowText(*pstrParam);
	}
	return 0;
}

void CDlgExplorerBrowser::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_pExplorerCtrl)
	{
		// 사이즈 변화시, 가로 세로 사이즈 브라우저에 전달
		CRect rect;
		m_pExplorerCtrl->GetWindowRect(&rect);
		ScreenToClient(&rect);

		m_pExplorerCtrl->MoveWindow(rect);

		Document jsonDoc;
		jsonDoc.SetObject();

		// create a rapidjson array type with similar syntax to std::vector
		Value array(kArrayType);
		// must pass an allocator when the object may need to allocate memory
		Document::AllocatorType& allocator = jsonDoc.GetAllocator();

		jsonDoc.AddMember("width", rect.Width() - 16, allocator);
		jsonDoc.AddMember("height", rect.Height() - 16, allocator);

		StringBuffer strbuf;
		Writer<StringBuffer> writer(strbuf);
		jsonDoc.Accept(writer);

		m_pExplorerCtrl->OnSize(strbuf.GetString());

		/*
		chain methods as rapidjson provides a fluent interface when modifying its objects
		array.PushBack("hello", allocator).PushBack("world", allocator);//"array":["hello","world"]

		
		document.AddMember("array", array, allocator);

		// create a rapidjson object type
		rapidjson::Value object(rapidjson::kObjectType);
		object.AddMember("Math", "50", allocator);
		object.AddMember("Science", "70", allocator);
		object.AddMember("English", "50", allocator);
		object.AddMember("Social Science", "70", allocator);
		document.AddMember("Marks", object, allocator);
		//	fromScratch["object"]["hello"] = "Yourname";

		StringBuffer strbuf;
		Writer<StringBuffer> writer(strbuf);
		document.Accept(writer);

		std::cout << strbuf.GetString() << std::endl;
		json.AddMember(_T("width"), rect.Width() - 16, allocator);
		json.AddMember(_T("height"), rect.Height() - 16, allocator);
		*/
	}
}