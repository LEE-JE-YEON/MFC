#pragma once
#define LOGGING_MAX_LEN 2049

//---------pantheios ���� �ش�----------------//
#include <stlsoft/stlsoft.h>
#include <pantheios/pantheios.h>
//---------------------------------------------//

PANTHEIOS_EXTERN_C const PAN_CHAR_T PANTHEIOS_FE_PROCESS_IDENTITY[] = PANTHEIOS_LITERAL_STRING("MyFramework");

#include <pantheios/pantheios.hpp>            // Pantheios C++ main header
#include <pantheios/backends/bec.file.h>      // be.file header
#include <pantheios/backends/bec.WindowsConsole.h> // �ܼ� ����� ���ؼ���
#include <pantheios/inserters/args.hpp>       // for pantheios::args
#include <pantheios/inserters/exception.hpp>  // for pantheios::exception


///*-------call back �Լ��� �����쿡 �����ϴ� ���---------*/
#include <pantheios/implicit_link/core.h>
#include <pantheios/implicit_link/fe.simple.h>
#include <pantheios/implicit_link/be.file.WithCallback.h> ///< pantheios_be_file_getAppInit
#include <pantheios/implicit_link/be.WindowsConsole.WithCallback.h> ///< pantheios_be_WindowsConsole_getAppInit

class CMyLog
{
private:
	BOOL m_bFileDel;	///< �α� ������ ������ ����
	BOOL m_bInfo;		///< �������̼� ������ �α��� ������ ����
	BOOL m_bNoti;		///< �˸� ������ �α��� ������ ����
	BOOL m_bErr;		///< ���� ������ �α��� ������ ����
	BOOL m_bDebug;		///< ����� ������ �α��� ������ ����
	const CString m_strLogFolderPath;		///< ���α׷� ����\\Log\\

public:
	CMyLog();
	virtual ~CMyLog();

	void Initialize(BOOL bFileDel = TRUE, BOOL bInfo = FALSE, BOOL bNoti = FALSE, BOOL bErr = TRUE);
	void Close();

	void Info(LPCTSTR lpszFormat, ...);
	void Info(DWORD dwBeginTick, LPCTSTR lpszFormat, ...);
	void Noti(CString strFuncName, LPCTSTR lpszFormat, ...);
	DWORD Noti_Begin(CString strFuncName, LPCTSTR lpszFormat, ...);
	void Noti_End(CString strFuncName, CString strEx, DWORD dwBeginTick, DWORD dwEndTick = 0);
	void Noti_End(CString strFuncName, CString strRet, CString strEx, DWORD dwBeginTick, DWORD dwEndTick = 0);
	void Noti_End(CString strFuncName, LONG nRet, CString strEx, DWORD dwBeginTick, DWORD dwEndTick = 0);
	void Noti_End(CString strFuncName, DOUBLE nRet, CString strEx, DWORD dwBeginTick, DWORD dwEndTick = 0);
	void Noti_End(CString strFuncName, BOOL bRet, CString strEx, DWORD dwBeginTick, DWORD dwEndTick = 0);
	void Noti_End(CString strFuncName, COLORREF nRet, CString strEx, DWORD dwBeginTick, DWORD dwEndTick = 0);
	void Error(CString strFuncName, LPCTSTR lpszFormat, ...);
	void Error(CString strFuncName, CException* e);
	void Debug(LPCTSTR lpszFormat, ...);
	void DeleteLogFiles();
};

