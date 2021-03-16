#include "pch.h"
#include "StringTokenizerEx.h"


CStringTokenizerEx::CStringTokenizerEx()
{

}

CStringTokenizerEx::CStringTokenizerEx(LPCTSTR lpszData, LPCTSTR lpszToken)
{
	Split(lpszData, lpszToken);
}

CStringTokenizerEx::~CStringTokenizerEx(void)
{
}

void CStringTokenizerEx::Split(LPCTSTR lpszData, LPCTSTR lpszToken)
{
	ASSERT(lpszData && ::AfxIsValidString(lpszData));
	ASSERT(lpszToken && ::AfxIsValidString(lpszToken));

	CString strData;
	CString strToken;
	int		nStart;
	int		nEnd;
	int		nCount;
	int		nTokenLen;
	
	m_sl.RemoveAll();

	strData = CString(lpszData) + lpszToken;
	nTokenLen = ::_tcslen(lpszToken);
	nStart = 0;

	while (1)
	{
		nEnd = strData.Find(lpszToken, nStart);
		if (-1 == nEnd)
			break;

		nCount = nEnd - nStart;
		strToken = strData.Mid(nStart, nCount);
		nStart = nEnd + nTokenLen;

		m_sl.AddTail(strToken);
	}
}

BOOL CStringTokenizerEx::GetItem(int nIdx, CString &rItem)
{
	POSITION pos = m_sl.FindIndex(nIdx);
	if (pos)
	{
		rItem = m_sl.GetAt(pos);
	}
	return (pos != NULL);
}

CString CStringTokenizerEx::GetItem(int nIdx)
{
	CString strItem;
	POSITION pos;
	
	pos = m_sl.FindIndex(nIdx);
	if (pos)
	{
		strItem = m_sl.GetAt(pos);
	}

	return strItem;
}