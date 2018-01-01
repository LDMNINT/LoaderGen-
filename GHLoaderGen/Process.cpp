#include "Process.h"
#include "XResource.h"
#include <ShlObj.h>
#define _WIN32_WINNT

CProcess::CProcess()
{
}

CProcess::CProcess(PROCESSENTRY32 pe32)
{
	this->pe32 = pe32;
	this->dwPID = pe32.th32ProcessID;
	this->szExe = pe32.szExeFile;
	Start();
}

CProcess::CProcess(DWORD dwPID, tstring szExe)
{
	this->dwPID = dwPID;
	this->szExe = szExe;
	Start();
}


CProcess::~CProcess()
{
	if (hHandle)
	{
		CloseHandle(hHandle);
	}

	if (hIcon)
		DestroyIcon(hIcon);
}

HANDLE CProcess::Open()
{
	return Open(PROCESS_QUERY_INFORMATION);
}

HANDLE CProcess::Open(DWORD dwAccess)
{
	hHandle = OpenProcess(dwAccess, FALSE, dwPID);
	if (!hHandle)
	{
		//GetLastError();
		return 0;
	}
	return hHandle;
}

tstring CProcess::GetProcName()
{
	return szExe;
}

tstring CProcess::GetProcPath()
{
	return szPath;
}

DWORD CProcess::GetPID()
{
	return dwPID;
}

HICON CProcess::GetIcon()
{	
	FindIcon();
	return hIcon;
}

HICON CProcess::GetIconSm()
{
	FindIcon();
	if (!hIconSm)
		hIconSm = hIcon;
	return hIconSm;
}

typedef BOOL(WINAPI * _IsWow64Process)(HANDLE, PBOOL);
_IsWow64Process  _IsWow64;
bool CProcess::Is64Bit()
{
	if (!_IsWow64)
	{
		HMODULE hMod = GetModuleHandle(_T("kernel32"));
		_IsWow64 = (_IsWow64Process)GetProcAddress(hMod, "IsWow64Process");
	}

	BOOL b64;
	if (_IsWow64)
	{
		if (Open())
		{
			_IsWow64(hHandle, &b64);
			return !b64;
		}
		return true;
	}
	return false;
}

void CProcess::Start()
{
	Open();
	FindProcPath();
	FindIcon();
}

void CProcess::FindProcPath()
{
	if (szPath.empty())
	{
		if (!hHandle)
			Open();

		if (hHandle)
		{
			szPath.resize(MAX_PATH);
			//fucker wasn't working on win7
			//GetModuleFileNameEx(hHandle, NULL, &szPath[0], MAX_PATH);
			DWORD dwLen = MAX_PATH;
			if(QueryFullProcessImageName(hHandle, 0, &szPath[0], &dwLen))
				szPath.resize(dwLen + 1);
		}
		else
			szPath = L"";
	}
}

void CProcess::FindIcon()
{
	HRSRC hRes;
	GetProcPath();
	if (!hIcon && !this->szPath.empty())
	{
		//if (!hHandle)
		//	Open();
		//if (!hHandle)
		//	return;

		//int iconSize = GetSystemMetrics(SM_CXSMICON);
		//HICON* pIcons = (HICON*)LocalAlloc(LPTR, 500 * sizeof(HICON));
		//int iIcons = ExtractIcons::Get(szPath.c_str(), 0, iconSize, iconSize, pIcons, NULL, 500, LR_COLOR);
		//hIcon = CopyIcon(pIcons[0]);
		//LocalFree(HLOCAL(pIcons));
		//ICONINFO oInfo;
		//HICON hSmall, hLarge[2];
		
		ExtractIconEx(szPath.c_str(), 0, &hIcon, &hIconSm, 1);
		////GetIconInfo(hSmall, &oInfo);
		////hIcon = CreateIconIndirect(&oInfo);
		//hIcon = CopyIcon(hLarge[0]);
		//hIconSm = CopyIcon(hSmall[0]);
		//if (hSmall[1])
		//	DestroyIcon(hSmall[1]);
		//DestroyIcon(hSmall[0]);
		//if (hLarge[1])
		//	DestroyIcon(hLarge[1]);
		//DestroyIcon(hLarge[0]);

	/*	HMODULE hMod = LoadLibrary(szPath.c_str());
		ExtractAssociatedIcon(GetModuleHandle(0), szPath)*/


		//SHFILEINFO sfi;
		////CoInitializeEx(NULL, COINIT::COINIT_APARTMENTTHREADED);
		//CoInitialize(NULL);
		//if (SHGetFileInfo(szPath.c_str(), 0, &sfi, sizeof(sfi), SHGFI_ICON | SHGFI_SMALLICON))
		//{
		//	//hIcon = sfi.hIcon;
		//	hIcon = CopyIcon(sfi.hIcon);
		//	hIconSm = hIcon;
		//	GetIconInfo(sfi.hIcon, &oInfo);
		//	hIcon = CreateIconIndirect(&oInfo);
		//	DestroyIcon(sfi.hIcon);
		//	hIconSm = hIcon;
		//	//hIcon = ExtractIcon(GetModuleHandle(0), &szPath[0], 1);
		//}
	}
}