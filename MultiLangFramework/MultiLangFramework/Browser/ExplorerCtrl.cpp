#include "pch.h"
#include "ExplorerCtrl.h"


CExplorerCtrl::CExplorerCtrl(CWnd* pParent)
{
	EnableAutomation();
	m_hParent = pParent->GetSafeHwnd();
}

CExplorerCtrl::~CExplorerCtrl(void)
{
}

BEGIN_MESSAGE_MAP(CExplorerCtrl, CHtmlView)
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()

int CExplorerCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default

	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

HRESULT CExplorerCtrl::OnGetExternal(LPDISPATCH *lppDispatch)
{
	*lppDispatch = GetIDispatch(TRUE);
	return S_OK;
}

BOOL CExplorerCtrl::OnEventToJavascript(const CString &strJson)
{
	BOOL bRet = FALSE;
	IDispatch *spDocIDispatch = NULL;
	m_pBrowserApp->get_Document(&spDocIDispatch);
	if (!spDocIDispatch)
	{
		return bRet;
	}
	IHTMLDocument2* pDoc2 = NULL;
	spDocIDispatch->QueryInterface(IID_IHTMLDocument2, (LPVOID*)&pDoc2);

	if (!pDoc2)
	{
		return bRet;
	}
	CComQIPtr<IHTMLDocument> pDoc(pDoc2);
	CComPtr<IDispatch> pDisp;
	HRESULT hr = pDoc->get_Script(&pDisp);
	if (FAILED(hr))
	{
		return bRet;
	}
	OLECHAR FAR* szFunctionName = _T("OnEventToJavascript");
	DISPID dispid;
	hr = pDisp->GetIDsOfNames(IID_NULL, &szFunctionName, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(hr))
	{
		return bRet;
	}
	DISPPARAMS dispParams;
	memset(&dispParams, 0, sizeof dispParams);
	dispParams.cArgs = 1;
	dispParams.rgvarg = new VARIANT[1];
	dispParams.rgdispidNamedArgs = NULL;
	dispParams.cNamedArgs = 0;

	CComBSTR bstr = strJson;
	bstr.CopyTo(&dispParams.rgvarg[0].bstrVal);
	dispParams.rgvarg[0].vt = VT_BSTR;

	CComVariant vResult;
	hr = pDisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispParams, &vResult, NULL, NULL);

	if (vResult.vt == VT_BSTR)
	{
		bRet = (CString)vResult.bstrVal == _T("1");
	}

	delete dispParams.rgvarg;
	pDoc.Release();
	pDisp.Release();
	bstr.Empty();
	vResult.Clear();

	return bRet;
}

void CExplorerCtrl::OnSize(const CStringA &strJson)
{
	// MFC 다이얼로그 사이즈에 따라서 브라우저 사이즈도 같이 변화
	IDispatch *spDocIDispatch = NULL;
	m_pBrowserApp->get_Document(&spDocIDispatch);
	if (!spDocIDispatch)
		return;

	IHTMLDocument2* pDoc2 = NULL;
	spDocIDispatch->QueryInterface(IID_IHTMLDocument2, (LPVOID*)&pDoc2);
	if (!pDoc2)
		return;

	CComQIPtr<IHTMLDocument> pDoc(pDoc2);
	CComPtr<IDispatch> pDisp;
	HRESULT hr = pDoc->get_Script(&pDisp);
	if (FAILED(hr))
		return;

	OLECHAR FAR* szFunctionName = _T("OnSize");
	DISPID dispid;
	hr = pDisp->GetIDsOfNames(IID_NULL, &szFunctionName, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(hr))
		return;

	DISPPARAMS dispParams;
	memset(&dispParams, 0, sizeof dispParams);
	dispParams.cArgs = 1;
	dispParams.rgvarg = new VARIANT[1];
	dispParams.rgdispidNamedArgs = NULL;
	dispParams.cNamedArgs = 0;

	CComBSTR bstr = strJson;
	bstr.CopyTo(&dispParams.rgvarg[0].bstrVal);
	dispParams.rgvarg[0].vt = VT_BSTR;

	hr = pDisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispParams, NULL, NULL, NULL);

	delete dispParams.rgvarg;
	pDoc.Release();
	pDisp.Release();
	bstr.Empty();
}



BEGIN_DISPATCH_MAP(CExplorerCtrl, CHtmlView)
	DISP_FUNCTION(CExplorerCtrl, "OnEventFromJavascript", OnEventFromJavascript, VT_I4, VTS_VARIANT)
END_DISPATCH_MAP()

void CExplorerCtrl::OnEventFromJavascript(const VARIANT FAR& vParentElement)
{
	CString strParam;
	switch (vParentElement.vt)
	{
	case VT_BSTR:
		strParam = vParentElement;
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	if (!strParam.IsEmpty())
	{
		::SendMessage(m_hParent, UWM_EVENT_FROM_JAVASCRIPT_EXPOLRER, NULL, (LPARAM)&strParam);
	}
}