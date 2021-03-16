// Registry.h: interface for the CRegistry class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define ID2STR(a)	(L#a)

class CRegistry
{
public:
	enum cregRestoreEnum
	{
		regVolatile = REG_WHOLE_HIVE_VOLATILE,
		regRefresh = REG_REFRESH_HIVE,
		regNoLazy = REG_NO_LAZY_FLUSH
	};

	//enum Keys
	//{
	//	classesRoot		=	(UINT)HKEY_CLASSES_ROOT,			// RM 20030930 added (int)
	//	currentUser		=	(UINT)HKEY_CURRENT_USER,
	//	localMachine	=	(UINT)HKEY_LOCAL_MACHINE,
	//	currentConfig	=	(UINT)HKEY_CURRENT_CONFIG,
	//	users			=	(UINT)HKEY_USERS,
	//	performanceData =	(UINT)HKEY_PERFORMANCE_DATA,		//Windows NT/2000
	//	dynData			=	(UINT)HKEY_DYN_DATA,				//Windows 95/98
	//	KeyCount
	//};

	CRegistry();
	virtual ~CRegistry();

	static HKEY ConvertRootKey(CString strRootKey);
	static BOOL Tokenize(CString strRegPath, HKEY &rRootKey, CString &rSubKey, CString &rEntryName);

	BOOL OpenKey(HKEY hKey, LPCTSTR szKey);
	BOOL CreateKey(HKEY hKey, LPCTSTR szKey);
	BOOL DeleteKey(HKEY hKey, LPCTSTR szKey);
	DWORD DeleteKeyAll(HKEY hStartKey, LPTSTR pKeyName );
	BOOL DeleteValue(LPCTSTR lpValueName);
	BOOL GetValue(LPCTSTR lpValueName, CString& pstrValue);
	BOOL GetValue(LPCTSTR lpValueName, DWORD& dwValue);
	BOOL SetValue(LPCTSTR lpValueName, LPCTSTR lpData);
	BOOL SetValue(LPCTSTR lpValueName, DWORD dwValue);
	BOOL SaveKey(LPCTSTR lpszFileName);
	BOOL RestoreKey(LPCTSTR lpszFileName, DWORD dwFlag);			// RM 20030930 LPCTSTR lpszFileName
	BOOL LoadKey(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR lpszFileName);	// RM 20030930 LPCTSTR lpszFileName
	BOOL EnumKey(DWORD index, LPTSTR lpKey);
	BOOL EnumValue(DWORD index, LPTSTR lpKey);
	void CloseKey();

protected:
	HKEY m_hKey;
};
