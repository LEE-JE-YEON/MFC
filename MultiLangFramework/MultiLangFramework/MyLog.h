#pragma once
#define LOGGING_MAX_LEN 2049
//---------pantheios 관련 해더----------------//
#include <stlsoft/stlsoft.h>
#include <pantheios/pantheios.h>
//---------------------------------------------//

PANTHEIOS_EXTERN_C const PAN_CHAR_T PANTHEIOS_FE_PROCESS_IDENTITY[] = PANTHEIOS_LITERAL_STRING("ID2013");

#include <pantheios/pantheios.hpp>            // Pantheios C++ main header
#include <pantheios/backends/bec.file.h>      // be.file header
#include <pantheios/backends/bec.WindowsConsole.h> // 콘솔 출력을 위해서리
#include <pantheios/inserters/args.hpp>       // for pantheios::args
#include <pantheios/inserters/exception.hpp>  // for pantheios::exception


/*-------call back 함수를 윈도우에 설정하는 헤더---------*/
#include <pantheios/implicit_link/core.h>
#include <pantheios/implicit_link/fe.simple.h>
#include <pantheios/implicit_link/be.file.WithCallback.h> ///< pantheios_be_file_getAppInit
#include <pantheios/implicit_link/be.WindowsConsole.WithCallback.h> ///< pantheios_be_WindowsConsole_getAppInit

class CMyLog
{
};

