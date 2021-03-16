#include "pch.h"
#include "JsonStringTable.h"

CJsonStringTable::CJsonStringTable()
{
	m_IsbLoad = FALSE;
}

CJsonStringTable::~CJsonStringTable()
{
}

BOOL CJsonStringTable::Load()
{
	HRSRC hRsrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE((INT)IDR_JSON_STRING_TABLE), _T("JSON"));
	if (!hRsrc)
		return FALSE;
	HGLOBAL hData = LoadResource(AfxGetResourceHandle(), hRsrc);
	if (!hData)
		return FALSE;

	char *pData = (char*)LockResource(hData);
	if (hData)
		FreeResource(hData);
	if (!pData)
		return FALSE;

	int nLen = MultiByteToWideChar(CP_UTF8, 0, pData, strlen(pData), NULL, NULL);
	wchar_t *pUTF8 = new wchar_t[nLen + 1];
	memset(pUTF8, 0x00, sizeof(wchar_t) * (nLen + 1));
	MultiByteToWideChar(CP_UTF8, 0, pData, strlen(pData), pUTF8, nLen);

	m_JsonDocument.Parse(pUTF8);
	if (m_JsonDocument.GetParseError() == ParseErrorCode::kParseErrorNone)
		m_IsbLoad = TRUE;

	delete[] pUTF8;

	return m_IsbLoad;
}

CString CJsonStringTable::GetString(const CString &strKey)
{
	if (!m_IsbLoad)
		return _T("");
	if (!m_JsonDocument[strKey.GetString()].IsString())
		return _T("");

	return m_JsonDocument[strKey.GetString()].GetString();
}