#pragma once

class CExplorerCtrl : public CHtmlView
{
public:
	CExplorerCtrl(CWnd* pParent);
	~CExplorerCtrl(void);

	DECLARE_DISPATCH_MAP()

private:
	HWND m_hParent;

public:
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	void OnEventFromJavascript(const VARIANT FAR& vParentElement);
	BOOL OnEventToJavascript(const CString &strJson);
	void OnSize(const CStringA &strJson);

protected:
	virtual HRESULT OnGetExternal(LPDISPATCH *lppDispatch);
	DECLARE_MESSAGE_MAP()
};
