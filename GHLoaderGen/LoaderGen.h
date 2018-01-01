#pragma once
#include <stdafx.h>
#include "XResource.h"
#include "resource.h"
#include "UPX.h"

#define BINARY                          1337
#define IDLOADERINJ                     109
#define IDLOADERDLL                     110
#define IDLOADER32						IDR_BINARY1
#define IDLOADER64						IDR_BINARY2
#define IDINJ64							IDR_BINARY3
#define IDINJ32							IDR_BINARY4
#define LOADERINFO						420
#define IDR_LOADERINFO1                 111
#define IDR_TEXTFILE1                   118
#define TEXTFILE						6969

struct LoaderInfo
{
	struct FormInfo
	{
		uint uFormTitleLen = 0;
		std::string szFormTitle;
		uint uGameTitleLen = 0;
		std::string szGameTitle;
		uint uAuthorLen = 0;
		std::string szAuthor;
	}formInfo;

	struct LoadSettings
	{
		uint uProcNameLen = 0;
		std::string szProcessName;
		BOOL bRandomNames = FALSE;
		BOOL bUPX = FALSE;
	}loadSettings;

	struct InjSettings
	{
		uint iInjMethod = 0;
		uint iLaunchMethod = 0;
		BOOL bHideDebug = FALSE;
		uint iPEHMethod = 0;
		BOOL bShiftMod = FALSE;
		BOOL bCleanDD = FALSE;
		BOOL bUnlinkPEB = FALSE;
		uint iDelay = 0;
	}injSettings;
};

class CLoaderGen
{
public:
	CLoaderGen(tstring * szDllPath, tstring * szReadme);
	~CLoaderGen();

	int Generate(tstring &szFilepath, LoaderInfo * loaderInfo, bool b64);

private:
	int GetLoaderInfoSize(LoaderInfo * loaderInfo);	
	void WriteLoaderBin(bool b64);
	std::vector<byte> LoadDLL(HANDLE & hDll);
	std::vector<byte> LoadInjector(bool b64);
	std::vector<byte> LoadReadme();
private:
	tstring szFilepath;
	tstring szDllPath;
	tstring szReadme;
	CUPX upx;
};

