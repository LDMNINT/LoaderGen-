#pragma once
//#include "stdafx.h"
#include "ExtractIcon.h"
class CProcess
{
	struct CProcessTimes
	{
		FILETIME ftCreation;
		FILETIME ftExit;
		FILETIME ftKernel;
		FILETIME ftUser;
	};

public:
	CProcess();
	CProcess(PROCESSENTRY32 pe32);
	CProcess(DWORD dwPID, tstring szExe);
	~CProcess();
	HANDLE Open();
	HANDLE Open(DWORD dwAccess);
	tstring GetProcName();
	tstring GetProcPath();
	DWORD GetPID();
	HICON GetIcon();
	HICON GetIconSm();
	bool Is64Bit();

private:
	void Start();
	void FindProcPath();
	void FindIcon();

private:
	PROCESSENTRY32 pe32;
	HANDLE hHandle = NULL;
	tstring szPath;
	tstring szExe;
	DWORD dwPID = -1;
	bool is32bit;
	CProcessTimes cpTimes{ 0 };
	HICON hIcon = NULL;
	HICON hIconSm = NULL;

};

