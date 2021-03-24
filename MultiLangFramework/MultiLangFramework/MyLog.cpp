#include "pch.h"
#include "MyLog.h"
#include <Windows.h>

#pragma comment(lib, "version.lib")

CMyLog::CMyLog()
{
	m_bFileDel = TRUE;
	m_bInfo = FALSE; ///< 인포메이션 레벨의 로깅을 남길지 말지
	m_bNoti = FALSE; ///< 알림 레벨의 로깅을 남길지 말지
	m_bErr = FALSE;  ///< 에러 레벨의 로깅을 남길지 말지
	m_bDebug = FALSE; ///< 디버깅 레벨의 로깅을 남길지 말지
}

CMyLog::~CMyLog()
{
	Close();
}

void CMyLog::DeleteLogFiles()
{
	CString strFullPath = m_strLogFolderPath + _T("*.*");
	CFileFind ff;
	if (ff.FindFile(strFullPath))
	{
		BOOL bRet = TRUE;
		while (bRet)
		{
			bRet = ff.FindNextFile();
			if (!ff.IsDirectory() && !ff.IsDots())
			{
				DeleteFile(m_strLogFolderPath + ff.GetFileName());
			}
		}
	}
	ff.Close();
}

void CMyLog::Initialize(BOOL bFileDel, BOOL bInfo, BOOL bNoti, BOOL bErr)
{
	// 경로 취득
	CString strExePath;
	TCHAR szPath[MAX_PATH];
	memset(szPath, 0, MAX_PATH);
	GetModuleFileName(AfxGetInstanceHandle(), szPath, MAX_PATH);
	strExePath = szPath;
	PathRemoveFileSpec(szPath);
	_tcscat_s(szPath, _T("\\Log\\"));

	auto strTemp = const_cast<CString*>(&m_strLogFolderPath);
	*strTemp = szPath;

	m_bFileDel = bFileDel;
	m_bInfo = bInfo; ///< 인포메이션 레벨의 로깅을 남길지 말지
	m_bNoti = bNoti; ///< 알림 레벨의 로깅을 남길지 말지
	m_bErr = bErr;;  ///< 에러 레벨의 로깅을 남길지 말지

#ifdef _DEBUG
	m_bDebug = TRUE; ///< 디버깅 레벨의 로깅을 남길지 말지
#else
	m_bDebug = FALSE; ///< 디버깅 레벨의 로깅을 남길지 말지
#endif

	if (m_bFileDel)
	{
		DeleteLogFiles();
	}

	CFileFind ff;

	if (ff.FindFile(m_strLogFolderPath) == 0)
	{
		CreateDirectory(m_strLogFolderPath, NULL);
	}

	if (m_bInfo || m_bNoti || m_bErr || m_bDebug)
	{
		pantheios::pantheios_init();
		pantheios_be_file_setFilePath(m_strLogFolderPath + _T("\\MyFramework_%D_%T.log"));
	}

	TCHAR szFullPath[MAX_PATH];
	CString strVersion;
	auto infoSize = GetFileVersionInfoSize(strExePath, 0);
	auto buff = new char[infoSize];
	if (GetFileVersionInfo(strExePath, NULL, infoSize, buff) != 0)
	{
		VS_FIXEDFILEINFO* pvsf = NULL;
		UINT nBuffLen = 0;
		if (VerQueryValue(buff, _T("\\"), (LPVOID*)&pvsf, &nBuffLen) != 0)
		{
			strVersion.Format(_T("MyFramework %d.%d.%d.%d, Copyright 2021-2021 All Rights Reserved."),
				HIWORD(pvsf->dwFileVersionMS),
				LOWORD(pvsf->dwFileVersionMS),
				HIWORD(pvsf->dwFileVersionLS),
				LOWORD(pvsf->dwFileVersionLS));
		}
	}
	delete[] buff;

	Info(strVersion);
	Info(_T("<<<<<========================LOGGING START========================>>>>>"));
}

void CMyLog::Close()
{
	Info(_T("<<<<<=========================LOGGING END=========================>>>>>"));
	if (m_bInfo || m_bNoti || m_bErr || m_bDebug)
	{
		pantheios::pantheios_uninit();
		pantheios_be_file_setFilePath(NULL);
	}

	if (m_bFileDel)
		DeleteLogFiles();
}

void CMyLog::Info(LPCTSTR lpszFormat, ...)
{
	if (m_bInfo == FALSE)
		return;

	TCHAR lpszBuffer[LOGGING_MAX_LEN];
	va_list fmtList;

	ZeroMemory(lpszBuffer, LOGGING_MAX_LEN);
	va_start(fmtList, lpszFormat);
	_vstprintf_s(lpszBuffer, _countof(lpszBuffer), lpszFormat, fmtList);
	va_end(fmtList);

	pantheios::log_ALERT(lpszBuffer);
}

void CMyLog::Info(DWORD dwBeginTick, LPCTSTR lpszFormat, ...)
{
	if (m_bInfo == FALSE)
		return;

	DWORD dwEndTick = GetTickCount64();
	double nEndTime = (double)((dwEndTick - dwBeginTick) / 1000.0);
	if (dwBeginTick == 0)
		nEndTime = 0;

	TCHAR    lpszBuffer[LOGGING_MAX_LEN];
	va_list  fmtList;

	ZeroMemory(lpszBuffer, LOGGING_MAX_LEN);
	va_start(fmtList, lpszFormat);
	_vstprintf_s(lpszBuffer, _countof(lpszBuffer), lpszFormat, fmtList);
	va_end(fmtList);


	CString strLog;
	if (nEndTime < 0.1)
	{
		strLog.Format(_T("%s"), lpszBuffer);
	}
	else
	{
		strLog.Format(_T("%s-%.1f"), lpszBuffer, nEndTime);
	}
	pantheios::log_ALERT(strLog);
}


void CMyLog::Noti(CString strFuncName, LPCTSTR lpszFormat, ...)
{
	if (m_bNoti == FALSE)
		return;

	CString strLog;

	TCHAR    lpszBuffer[LOGGING_MAX_LEN];
	va_list  fmtList;

	ZeroMemory(lpszBuffer, LOGGING_MAX_LEN);
	va_start(fmtList, lpszFormat);
	_vstprintf_s(lpszBuffer, _countof(lpszBuffer), lpszFormat, fmtList);
	va_end(fmtList);

	strLog.Format(_T("%s=>%s"), (LPCTSTR)strFuncName, (LPCTSTR)lpszBuffer);
	pantheios::log_NOTICE(strLog);
}

DWORD CMyLog::Noti_Begin(CString strFuncName, LPCTSTR lpszFormat, ...)
{
	if (m_bNoti == FALSE)
		return 0;

	DWORD dwBeginTick = ::GetTickCount64();
	CString strLog;

	TCHAR    lpszBuffer[LOGGING_MAX_LEN];
	va_list  fmtList;

	INT nLen = _tcslen(lpszFormat);
	if (nLen)
	{
		ZeroMemory(lpszBuffer, LOGGING_MAX_LEN);
		va_start(fmtList, lpszFormat);
		_vstprintf_s(lpszBuffer, _countof(lpszBuffer), lpszFormat, fmtList);
		va_end(fmtList);
		strLog.Format(_T("%s START(%s)"), (LPCTSTR)strFuncName, (LPCTSTR)lpszBuffer);
	}
	else
	{
		strLog.Format(_T("%s START"), (LPCTSTR)strFuncName);
	}
	pantheios::log_NOTICE(strLog);
	return dwBeginTick;
}

void CMyLog::Noti_End(CString strFuncName, CString strEx, DWORD dwBeginTick, DWORD dwEndTick)
{
	if (m_bNoti == FALSE)
		return;

	CString strLog;
	if (dwBeginTick == 0)
	{
		strLog.Format(_T("%s FINISH:%s"), (LPCTSTR)strFuncName, (LPCTSTR)strEx);
		pantheios::log_NOTICE(strLog);
		return;
	}
	if (dwEndTick == 0)
	{
		dwEndTick = ::GetTickCount64();
	}
	double nEndTime = (double)((dwEndTick - dwBeginTick) / 1000.0);
	if (nEndTime < 0.1)
	{
		if (strEx.GetLength())
			strLog.Format(_T("%s FINISH:%s"), (LPCTSTR)strFuncName, (LPCTSTR)strEx);
		else
			strLog.Format(_T("%s FINISH"), (LPCTSTR)strFuncName);
	}
	else
	{
		if (strEx.GetLength())
			strLog.Format(_T("%s FINISH:%s-%.1f"), (LPCTSTR)strFuncName, (LPCTSTR)strEx, nEndTime);
		else
			strLog.Format(_T("%s FINISH-%.1f"), (LPCTSTR)strFuncName, nEndTime);
	}
	pantheios::log_NOTICE(strLog);
}

void CMyLog::Noti_End(CString strFuncName, CString strRet, CString strEx, DWORD dwBeginTick, DWORD dwEndTick)
{
	if (m_bNoti == FALSE)
		return;

	CString strLog;
	if (dwBeginTick == 0)
	{
		strLog.Format(_T("%s FINISH(%s):%s"), (LPCTSTR)strFuncName, (LPCTSTR)strRet, (LPCTSTR)strEx);
		pantheios::log_NOTICE(strLog);
		return;
	}
	if (dwEndTick == 0)
	{
		dwEndTick = ::GetTickCount64();
	}
	double nEndTime = (double)((dwEndTick - dwBeginTick) / 1000.0);
	if (nEndTime < 0.1)
	{
		if (strEx.GetLength())
			strLog.Format(_T("%s FINISH(%s):%s"), (LPCTSTR)strFuncName, (LPCTSTR)strRet, (LPCTSTR)strEx);
		else
			strLog.Format(_T("%s FINISH(%s)"), (LPCTSTR)strFuncName, (LPCTSTR)strRet);
	}
	else
	{
		if (strEx.GetLength())
			strLog.Format(_T("%s FINISH(%s):%s-%.1f"), (LPCTSTR)strFuncName, (LPCTSTR)strRet, (LPCTSTR)strEx, nEndTime);
		else
			strLog.Format(_T("%s FINISH(%s)-%.1f"), (LPCTSTR)strFuncName, (LPCTSTR)strRet, nEndTime);
	}
	pantheios::log_NOTICE(strLog);
}

void CMyLog::Noti_End(CString strFuncName, LONG nRet, CString strEx, DWORD dwBeginTick, DWORD dwEndTick)
{
	if (m_bNoti == FALSE)
		return;

	CString strLog;
	if (dwBeginTick == 0)
	{
		strLog.Format(_T("%s FINISH(%d):%s"), (LPCTSTR)strFuncName, nRet, (LPCTSTR)strEx);
		pantheios::log_NOTICE(strLog);
		return;
	}
	if (dwEndTick == 0)
	{
		dwEndTick = ::GetTickCount64();
	}
	double nEndTime = (double)((dwEndTick - dwBeginTick) / 1000.0);
	if (nEndTime < 0.1)
	{
		if (strEx.GetLength())
			strLog.Format(_T("%s FINISH(%d):%s"), (LPCTSTR)strFuncName, nRet, (LPCTSTR)strEx);
		else
			strLog.Format(_T("%s FINISH(%d)"), (LPCTSTR)strFuncName, nRet);

	}
	else
	{
		if (strEx.GetLength())
			strLog.Format(_T("%s FINISH(%d):%s-%.1f"), (LPCTSTR)strFuncName, nRet, (LPCTSTR)strEx, nEndTime);
		else
			strLog.Format(_T("%s FINISH(%d)-%.1f"), (LPCTSTR)strFuncName, nRet, nEndTime);
	}

	pantheios::log_NOTICE(strLog);
}

void CMyLog::Noti_End(CString strFuncName, DOUBLE nRet, CString strEx, DWORD dwBeginTick, DWORD dwEndTick)
{
	if (m_bNoti == FALSE)
		return;

	CString strLog;
	if (dwBeginTick == 0)
	{
		strLog.Format(_T("%s FINISH(%f):%s"), (LPCTSTR)strFuncName, nRet, (LPCTSTR)strEx);
		pantheios::log_NOTICE(strLog);
		return;
	}
	if (dwEndTick == 0)
	{
		dwEndTick = ::GetTickCount64();
	}
	double nEndTime = (double)((dwEndTick - dwBeginTick) / 1000.0);
	if (nEndTime < 0.1)
	{
		if (strEx.GetLength())
			strLog.Format(_T("%s FINISH(%f):%s"), (LPCTSTR)strFuncName, nRet, (LPCTSTR)strEx);
		else
			strLog.Format(_T("%s FINISH(%f)"), (LPCTSTR)strFuncName, nRet);
	}
	else
	{
		if (strEx.GetLength())
			strLog.Format(_T("%s FINISH(%f):%s-%.1f"), (LPCTSTR)strFuncName, nRet, (LPCTSTR)strEx, nEndTime);
		else
			strLog.Format(_T("%s FINISH(%f)-%.1f"), (LPCTSTR)strFuncName, nRet, nEndTime);
	}

	pantheios::log_NOTICE(strLog);
}

void CMyLog::Noti_End(CString strFuncName, BOOL bRet, CString strEx, DWORD dwBeginTick, DWORD dwEndTick)
{
	if (m_bNoti == FALSE)
		return;

	CString strLog;
	if (dwBeginTick == 0)
	{
		strLog.Format(_T("%s FINISH(%d):%s"), (LPCTSTR)strFuncName, bRet, (LPCTSTR)strEx);
		pantheios::log_NOTICE(strLog);
		return;
	}
	if (dwEndTick == 0)
	{
		dwEndTick = ::GetTickCount64();
	}
	double nEndTime = (double)((dwEndTick - dwBeginTick) / 1000.0);
	if (nEndTime < 0.1)
	{
		if (strEx.GetLength())
			strLog.Format(_T("%s FINISH(%d):%s"), (LPCTSTR)strFuncName, bRet, (LPCTSTR)strEx);
		else
			strLog.Format(_T("%s FINISH(%d)"), (LPCTSTR)strFuncName, bRet);
	}
	else
	{
		if (strEx.GetLength())
			strLog.Format(_T("%s FINISH(%d):%s-%.1f"), (LPCTSTR)strFuncName, bRet, (LPCTSTR)strEx, nEndTime);
		else
			strLog.Format(_T("%s FINISH(%d)-%.1f"), (LPCTSTR)strFuncName, bRet, nEndTime);
	}
	pantheios::log_NOTICE(strLog);
}

void CMyLog::Noti_End(CString strFuncName, COLORREF nRet, CString strEx, DWORD dwBeginTick, DWORD dwEndTick)
{
	if (m_bNoti == FALSE)
		return;

	CString strLog;
	if (dwBeginTick == 0)
	{
		strLog.Format(_T("%s FINISH(%X):%s"), (LPCTSTR)strFuncName, nRet, (LPCTSTR)strEx);
		pantheios::log_NOTICE(strLog);
		return;
	}
	if (dwEndTick == 0)
	{
		dwEndTick = ::GetTickCount64();
	}
	double nEndTime = (double)((dwEndTick - dwBeginTick) / 1000.0);
	if (nEndTime < 0.1)
	{
		if (strEx.GetLength())
			strLog.Format(_T("%s FINISH(%X):%s"), (LPCTSTR)strFuncName, nRet, (LPCTSTR)strEx);
		else
			strLog.Format(_T("%s FINISH(%X)"), (LPCTSTR)strFuncName, nRet);

	}
	else
	{
		if (strEx.GetLength())
			strLog.Format(_T("%s FINISH(%X):%s-%.1f"), (LPCTSTR)strFuncName, nRet, (LPCTSTR)strEx, nEndTime);
		else
			strLog.Format(_T("%s FINISH(%X)-%.1f"), (LPCTSTR)strFuncName, nRet, nEndTime);
	}
	pantheios::log_NOTICE(strLog);
}



void CMyLog::Error(CString strFuncName, LPCTSTR lpszFormat, ...)
{
	if (m_bErr == FALSE)
		return;

	CString strLog;
	TCHAR    lpszBuffer[LOGGING_MAX_LEN];
	va_list  fmtList;

	ZeroMemory(lpszBuffer, LOGGING_MAX_LEN);
	va_start(fmtList, lpszFormat);
	_vstprintf_s(lpszBuffer, _countof(lpszBuffer), lpszFormat, fmtList);
	va_end(fmtList);

	strLog.Format(_T("%s=>%s"), (LPCTSTR)strFuncName, (LPCTSTR)lpszBuffer);
	pantheios::log_ERROR(strLog);
}

void CMyLog::Error(CString strFuncName, CException* e)
{
	if (m_bErr == FALSE)
		return;

	CString strLog;
	TCHAR lpszErrMsg[LOGGING_MAX_LEN];
	ZeroMemory(lpszErrMsg, LOGGING_MAX_LEN);
	e->GetErrorMessage(lpszErrMsg, LOGGING_MAX_LEN);
	pantheios::log_ERROR(lpszErrMsg);
	e->Delete();
	strLog.Format(_T("%s ERROR"), (LPCTSTR)strFuncName);
	pantheios::log_ERROR(strLog);
}

void CMyLog::Debug(LPCTSTR lpszFormat, ...)
{
	if (m_bDebug == FALSE)
		return;

	TCHAR    lpszBuffer[LOGGING_MAX_LEN];
	va_list  fmtList;

	ZeroMemory(lpszBuffer, LOGGING_MAX_LEN);
	va_start(fmtList, lpszFormat);
	_vstprintf_s(lpszBuffer, _countof(lpszBuffer), lpszFormat, fmtList);
	va_end(fmtList);

	pantheios::log_DEBUG(lpszBuffer);
};

PANTHEIOS_CALL(void) pantheios_be_file_getAppInit(int backEndId, pan_be_file_init_t* init) /* throw() */
{
	init->flags = PANTHEIOS_BE_FILE_F_TRUNCATE        // Truncate the contents
		| PANTHEIOS_BE_FILE_F_DELETE_IF_EMPTY // Delete the file if nothing written
		| PANTHEIOS_BE_INIT_F_NO_PROCESS_ID
		| PANTHEIOS_BE_INIT_F_USE_UNIX_FORMAT;
}

PANTHEIOS_CALL(void) pantheios_be_WindowsConsole_getAppInit(int backEndId, pan_be_WindowsConsole_init_t* init) /* throw() */
{
	init->flags = PANTHEIOS_BE_INIT_F_NO_PROCESS_ID
		| PANTHEIOS_BE_INIT_F_USE_UNIX_FORMAT
		| PANTHEIOS_BE_INIT_F_HIGH_RESOLUTION;

	init->colours[PANTHEIOS_SEV_DEBUG] = init->colours[PANTHEIOS_SEV_INFORMATIONAL];
	init->colours[PANTHEIOS_SEV_NOTICE] = init->colours[PANTHEIOS_SEV_INFORMATIONAL];
}