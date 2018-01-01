#pragma once
#include "XGui\XMainWindow.h"
#include "GHLProcPicker.h"
#include "resource.h"
#include "LoaderGen.h"

//form properties
#define SZWND_TITLE _T("GH-Loader Generator")
#define GHL_WND_W 600
#define GHL_WND_H 400
#define GHL_WND_BGCLR RGB(25,25,25)

//control id's
#define IDBTN_PROCSELECT	0x5001
#define IDBTN_DLLSELECT		0x5002
#define IDCHK_RANDNAMES		0x5003
#define IDCHK_UPXFILES		0x5004
#define IDBTN_SELECTDLL		0x5005
#define IDEDT_DLLPATH		0x5006
#define IDBTN_README		0x5007
#define IDEDT_README		0x5008
#define IDBTN_BUILD			0x5009
#define IDBTN_GH			0x5010

#define IDGRP_LOADSET		0x6000
#define IDLBL_FORMNAME		0x6001
#define IDEDT_FORMNAME		0x6002
#define IDLBL_GAMENAME		0x6003
#define IDEDT_GAMENAME		0x6004
#define IDLBL_AUTHOR		0x6005
#define IDEDT_AUTHOR		0x6006

#define IDGRP_PROCESS		0x7000
#define IDLBL_PROCNAME		0x7001
#define IDEDT_PROCNAME		0x7002
#define IDLBL_PID			0x7003
#define IDLBL_ARCH			0x7004	

#define IDGRP_INJSET		0x8000
#define IDRDO_LOADLIB		0x8001
#define IDRDO_LDRLOAD		0x8002
#define IDRDO_MANMAP		0x8003
#define IDCBX_LAUNCH		0x8004
#define IDCHK_HIDEDBG		0x8005
#define IDLBL_DELAY			0x8006
#define IDEDT_DELAY			0x8007
#define IDCBX_PEH			0x8008
#define IDCHK_SHIFT			0x8009
#define IDCHK_CDDIR			0x8010
#define IDCHK_ULNKPEB		0x8011

enum EBTN_COMMANDS : uintptr_t
{
	SELECT_PROCESS,
	SELECT_DLL,
	SELECT_README,
	SELECT_BUILD,
	SELECT_GH
};
typedef EBTN_COMMANDS ebc;

class GHLMainWindow : public CXMainWindow
{
public:
	GHLMainWindow(HINSTANCE hInstance, int x, int y);
	~GHLMainWindow();
	void SelectProcess();
	void ProcessSelected();
	void SelectDll();
	void SelectReadme();
	void LaunchGH();
	void GenerateLoader();
	void InjMethodRestrictions(uintptr_t iMethod);
	void LaunchMethodRestrictions(uintptr_t iMethod);
private:
	int CreateControls();
	void GetLoaderInfo(LoaderInfo & li);
private:
	CGHLProcPicker * pProcPicker = nullptr;
	CProcess *targetProc = nullptr;
	CLoaderGen * pGen;
};

void ProcPickerCallback();