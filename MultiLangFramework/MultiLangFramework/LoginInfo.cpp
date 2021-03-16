#include "pch.h"
#include "LoginInfo.h"


CLoginInfo::CLoginInfo()
{
	m_wdCurLangID = LANG_KOREAN;
}

CLoginInfo::~CLoginInfo()
{
}

void CLoginInfo::SetCurLangID(const LANGID wdValue)
{
	m_wdCurLangID = wdValue;
}

LANGID CLoginInfo::GetCurLangID()
{
	return m_wdCurLangID;
}