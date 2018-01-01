#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <functional>
#include <map>
#include <algorithm>
#include <memory>
#include <Uxtheme.h>
#include <vsstyle.h>
#include <chrono>
#include <thread>
#include <Psapi.h>
#include <TlHelp32.h>
#include <fstream>

//#include "resource.h"
#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "UxTheme.lib")
//applies current windows theme to controls.
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifdef _UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

typedef UINT uint;

#define DEFLANGID MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL)
#define OpenFile(s) CreateFile(s, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)
#define ErrorMsgBox(s) MessageBox(NULL, s, _T("Error!"), MB_ICONERROR | MB_OK)

#include "..\Utility.h"