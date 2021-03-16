#pragma once

class CLoginInfo
{
public:
	CLoginInfo();
	virtual ~CLoginInfo();
	void SetCurLangID(const LANGID wdValue);
	LANGID GetCurLangID();

private:
	LANGID m_wdCurLangID;
};