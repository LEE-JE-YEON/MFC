#include "pch.h"
#include "registry.h"
#include "Shlwapi.h"
#include "StringTokenizerEx.h"

#define		MAX_KEY_LENGTH	MAX_PATH

CRegistry::CRegistry()
{
	m_hKey = NULL;
}

CRegistry::~CRegistry()
{
	CloseKey();
	m_hKey = NULL;
}

HKEY CRegistry::ConvertRootKey(CString strRootKey)
{
	typedef struct tagRootKeyInfo
	{
		HKEY	hKey;
		CString strKey;
	} RootKeyInfo;

	static RootKeyInfo s_RootKeyInfo[] = 
	{
		{ HKEY_CLASSES_ROOT,		ID2STR(HKEY_CLASSES_ROOT) }
		, { HKEY_CURRENT_USER,		ID2STR(HKEY_CURRENT_USER) }
		, { HKEY_LOCAL_MACHINE,		ID2STR(HKEY_LOCAL_MACHINE) }
		, { HKEY_CURRENT_CONFIG,	ID2STR(HKEY_CURRENT_CONFIG) }
		, { HKEY_USERS,				ID2STR(HKEY_USERS) }
		, { HKEY_PERFORMANCE_DATA,	ID2STR(HKEY_PERFORMANCE_DATA) }
		, { HKEY_DYN_DATA,			ID2STR(HKEY_DYN_DATA) }
	};
	const int c_nRootKeyCount = _countof(s_RootKeyInfo);

	for (int i = 0; i < c_nRootKeyCount; ++i)
	{
		if (0 == strRootKey.CompareNoCase(s_RootKeyInfo[i].strKey))
		{
			return s_RootKeyInfo[i].hKey;
		}
	}

	return NULL;
}

BOOL CRegistry::Tokenize(CString strRegPath, HKEY &rRootKey, CString &rSubKey, CString &rEntryName)
{
	const LPCTSTR c_lpszToken = _T("\\");
	int i, nTokenCount;
	CString strRootKey, strSubKey, strEntryName;

	strRegPath.Replace(_T("/"), c_lpszToken);

	CStringTokenizerEx tok(strRegPath, c_lpszToken);
	nTokenCount = tok.GetCount();

	strRootKey = tok.GetItem(0);
	strEntryName = tok.GetItem(nTokenCount - 1);
	for (i = 1; i < nTokenCount - 1; ++i)
	{
		strSubKey += tok.GetItem(i) + c_lpszToken;
	}
	strSubKey.TrimRight(c_lpszToken);

	rRootKey = ConvertRootKey(strRootKey);
	rSubKey = strSubKey;
	rEntryName = strEntryName;

	return (NULL != rRootKey);
}

BOOL CRegistry::OpenKey(HKEY hKey, LPCTSTR szKey)
{	
	if(RegOpenKeyEx((HKEY)hKey, (LPCTSTR)szKey, 0, KEY_ALL_ACCESS, &m_hKey) == ERROR_SUCCESS)
	{
		return TRUE;
	}
	else	{
		m_hKey = NULL;
		return FALSE;
	}
}

BOOL CRegistry::CreateKey(HKEY hKey, LPCTSTR szKey)
{	
	if(RegCreateKeyEx((HKEY)hKey, (LPCTSTR)szKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, NULL) == ERROR_SUCCESS)
	{
		return TRUE;
	}
	else	{
		m_hKey = NULL;
		return FALSE;
	}
}

BOOL CRegistry::SetValue(LPCTSTR lpValueName, LPCTSTR lpData)
{
	ASSERT(m_hKey != NULL);

	DWORD dwType = REG_SZ;

	if (::RegSetValueEx(m_hKey, (LPCTSTR)lpValueName, 0, dwType, (LPBYTE)(LPCTSTR)lpData, (DWORD) _tcslen(lpData) * sizeof(TCHAR) + 1) == ERROR_SUCCESS)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CRegistry::SetValue(LPCTSTR lpValueName, DWORD dwValue)
{
	ASSERT(m_hKey != NULL);
	
	DWORD dwType = REG_DWORD;
	
	if(::RegSetValueEx(m_hKey, (LPCTSTR)lpValueName, 0, dwType, (LPBYTE)&dwValue, sizeof(DWORD)) == ERROR_SUCCESS)
	{
		return TRUE;
	}
	return FALSE;
}


BOOL CRegistry::GetValue(LPCTSTR lpValueName, CString& pstrValue)
{
	BOOL bRet = FALSE;
	LPTSTR lpstrValue;
	DWORD dwType = REG_SZ;
	DWORD lpcbData;
	
	if (::RegQueryValueEx(m_hKey, lpValueName, NULL, &dwType, NULL, &lpcbData) == ERROR_SUCCESS)
	{
		if (lpcbData == 0)	{
			bRet = TRUE;
		}
		else	{
			lpstrValue = new TCHAR[lpcbData+1];
			::ZeroMemory(lpstrValue, sizeof(TCHAR) * (lpcbData+1));

			if(::RegQueryValueEx(m_hKey, lpValueName, NULL, &dwType, (BYTE*)(LPCTSTR)lpstrValue, &lpcbData) == ERROR_SUCCESS)
			{
				bRet = TRUE;
				pstrValue = lpstrValue;
				//_tcscpy(pstrValue, lpstrValue);
			}

			delete [] lpstrValue;
		}
	}

	return bRet;
}

BOOL CRegistry::GetValue(LPCTSTR lpValueName, DWORD& dwValue)
{
	BOOL bRet = FALSE;
	DWORD dwType = REG_DWORD;
	DWORD lpcbData = sizeof(DWORD);
	dwValue = 0;
	
	//if(RegQueryValueEx(m_hKey, lpValueName,	NULL, &dwType, (BYTE*)(DWORD)&dwValue, &lpcbData) == ERROR_SUCCESS)
	if (RegQueryValueEx(m_hKey, lpValueName,	NULL, &dwType, (BYTE*)&dwValue, &lpcbData) == ERROR_SUCCESS)
	{
		bRet = TRUE;
	}

	return bRet;
}

BOOL CRegistry::DeleteKey(HKEY hKey, LPCTSTR szKey)
{
	if ( ::SHDeleteKey((HKEY)hKey, (LPCTSTR)szKey) == ERROR_SUCCESS )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CRegistry::DeleteValue(LPCTSTR lpValueName)
{	
	if(::RegDeleteValue(m_hKey,(LPCTSTR)lpValueName) == ERROR_SUCCESS)	
	{
		return TRUE;
	}
	else	
	{
		return FALSE;
	}
}

void CRegistry::CloseKey()
{
	::RegCloseKey(m_hKey);
	m_hKey = NULL;
}

BOOL CRegistry::SaveKey(LPCTSTR lpszFileName)
{
	ASSERT(m_hKey != NULL);
	return ::RegSaveKey(m_hKey, lpszFileName, NULL) == ERROR_SUCCESS;
}

BOOL CRegistry::RestoreKey(LPCTSTR lpszFileName, DWORD dwFlags)
{
	ASSERT(m_hKey != NULL);
	return ::RegRestoreKey(m_hKey, lpszFileName, dwFlags) == ERROR_SUCCESS;
}

BOOL CRegistry::LoadKey(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR lpszFileName)
{
	return ::RegLoadKey(hKey, lpszSubKey, lpszFileName) == ERROR_SUCCESS;
}

BOOL CRegistry::EnumKey(DWORD index, LPTSTR lpKey)
{
	BOOL bRet = FALSE;
	DWORD lenKey;
	
	const int c_nBufSize = sizeof(TCHAR) * MAX_KEY_LENGTH;
	LPTSTR lpstrValue = new TCHAR[c_nBufSize];
	::ZeroMemory(lpstrValue, c_nBufSize);
	
	if(::RegEnumKeyEx(m_hKey, index, lpstrValue, &lenKey, 0, NULL, NULL, NULL) == ERROR_SUCCESS)
	{
		_tcscpy_s(lpKey, c_nBufSize, lpstrValue);
		bRet = TRUE;
	}
	else
	{
//		_tcscpy(lpKey, _T(""));
	}

	delete [] lpstrValue;
	
	return bRet;
}

BOOL CRegistry::EnumValue(DWORD index, LPTSTR lpKey)
{
	BOOL bRet = FALSE;
	DWORD lenValue = MAX_KEY_LENGTH;

	const int c_nBufSize = sizeof(TCHAR) * MAX_KEY_LENGTH;
	LPTSTR lpstrValue = new TCHAR[c_nBufSize];
	::ZeroMemory(lpstrValue, c_nBufSize);

	if(::RegEnumValue((HKEY)m_hKey, index, lpstrValue, &lenValue, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
	{
		_tcscpy_s(lpKey, c_nBufSize, lpstrValue);
		bRet = TRUE;
	}
	else
	{
//		strcpy(lpKey, _T(""));
	}

	delete [] lpstrValue;
	
	return bRet;
}

DWORD CRegistry::DeleteKeyAll(HKEY hKey, LPTSTR pKeyName )
{
   DWORD   dwRtn, dwSubKeyLength;
//   LPTSTR  pSubKey = NULL;
   TCHAR   szSubKey[MAX_KEY_LENGTH]; // (256) this should be dynamic.
   HKEY    hSubKey;
 
	if ( pKeyName &&  lstrlen(pKeyName))
	{
		if( (dwRtn = RegOpenKeyEx(hKey, pKeyName,0, KEY_ENUMERATE_SUB_KEYS | DELETE, &hSubKey)) == ERROR_SUCCESS)
		{
			while (dwRtn == ERROR_SUCCESS )
			{
				dwSubKeyLength = MAX_KEY_LENGTH;
				dwRtn = RegEnumKeyEx(hSubKey, 0, szSubKey, &dwSubKeyLength, NULL, NULL, NULL, NULL);
				if(dwRtn == ERROR_NO_MORE_ITEMS)
				{
					dwRtn = RegDeleteKey(hKey, pKeyName);
					break;
				}
				else if(dwRtn == ERROR_SUCCESS)
				{
					dwRtn = DeleteKeyAll(hSubKey, szSubKey);	// Recursive Function
				}
			}
			RegCloseKey(hSubKey);
		}
	}
	else
		dwRtn = ERROR_BADKEY;
 
   return dwRtn;
}
