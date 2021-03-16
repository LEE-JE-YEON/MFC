#pragma once

class CStringTokenizerEx
{
public:
	CStringTokenizerEx();
	CStringTokenizerEx(LPCTSTR lpszData, LPCTSTR lpszToken);
	~CStringTokenizerEx(void);

	BOOL GetItem(int nIdx, CString &rItem);
	CString GetItem(int nIdx);

	int GetCount() const 
	{
		return (int)m_sl.GetCount();
	}

	void Split(LPCTSTR lpszData, LPCTSTR lpszToken);

private:
	CStringList m_sl;
};
