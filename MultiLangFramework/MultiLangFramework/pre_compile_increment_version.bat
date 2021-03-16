@echo off

set "fileNM=version.h"

set num=0

:: 헤더파일의 define 값을 취득한다.
for /f "eol=; tokens=2,3 delims=	" %%i in (%fileNM%) do (
	if "%%i"=="VERSION_YYYY" (
		echo %%i
		set "yyyy=%%j"
	)
	if "%%i"=="VERSION_MM" (
		echo %%i
		set "mm=%%j"
	)
	if "%%i"=="VERSION_DD" (
		echo %%i
		set "dd=%%j"
	)
	if "%%i"=="VERSION_NUM" (
		echo %%i %%j
		set num=%%j
	)
)

:: 날짜계산
set yyyy=%date:~0,4%
if %date:~5,1%==0 (
	set mm=%date:~6,1%
) else (
	set mm=%date:~5,2%
)
set dd=%date:~8,2%
set /a num+=1

echo %yyyy%
echo %mm%
echo %dd%
echo %num%

:: 파일 출력
echo #pragma once>%fileNM%
echo.>>%fileNM%
echo.#define	VERSION_YYYY				%yyyy% >>%fileNM%
echo.#define	VERSION_MM					%mm% >>%fileNM%
echo.#define	VERSION_DD					%dd% >>%fileNM%
echo.#define	VERSION_NUM					%num% >>%fileNM%
echo.#define	VERSION_YYYY_MM_DD_NUM		"%yyyy%.%mm%.%dd%.%num%" >>%fileNM%


