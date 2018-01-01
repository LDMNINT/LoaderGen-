#include "UPX.h"



CUPX::CUPX(tstring szFilepath)
	: resWriter(GetModuleHandle(0)),
	binUPX(szFilepath)
{
	this->szFilepath = szFilepath;
}


CUPX::~CUPX()
{
}

void CUPX::Compress(tstring szTarget)
{
	if (!binUPX.Exists())
		Write();

	//i'd like to just use the code... but for now this'll work lol.
	tstring buffer;
	buffer.resize(MAX_PATH);
	//buffer[0] = _T('\"');
	DWORD dwLen = GetCurrentDirectory(MAX_PATH, &buffer[0]);
	if (!dwLen)
	{
		return; //error
	}
	buffer.resize(dwLen);
	buffer += _T("\\");
	buffer += szFilepath;
	//buffer += _T("\"");
	size_t pathLen = szTarget.find_last_of(_T("\\")) + 1;
	tstring buff2 = szTarget.substr(pathLen, szTarget.length() - pathLen);
	ShellExecute(NULL, NULL, buffer.c_str(), buff2.c_str(), NULL, SW_HIDE);
	/*CProcess upx = pm.FindProcess(_T("upx.exe"));*/
	while (pm.IsProcessAlive(_T("upx.exe")));
}

void CUPX::Cleanup()
{
	DeleteFile(szFilepath.c_str());
}

void CUPX::Write()
{
	resWriter.Write(szFilepath, IDUPX, BINARY);
}