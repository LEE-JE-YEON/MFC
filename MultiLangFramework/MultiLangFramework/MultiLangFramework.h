
// MultiLangFramework.h: MultiLangFramework 응용 프로그램의 기본 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // 주 기호입니다.


// CMyFrameWork2App:
// 이 클래스의 구현에 대해서는 MultiLangFramework.cpp을(를) 참조하세요.
//

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};


class CMyFrameWork2App : public CWinApp
{
public:
	CMyFrameWork2App() noexcept;


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

private:
	BOOL _initLanguage();

public:
	DECLARE_MESSAGE_MAP()
};

extern CMyFrameWork2App theApp;