#include "LoaderGen.h"


#define CreateNewFile(s) CreateFile(s, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

CLoaderGen::CLoaderGen(tstring * szDllPath, tstring * szReadme)
	: upx(_T("upx.exe"))
{
	this->szDllPath = szDllPath->c_str();
	this->szReadme = szReadme->c_str();
}

CLoaderGen::~CLoaderGen()
{
}

template <class T>
void WriteToResBuffer(std::vector<byte> & vResource, T tData, size_t szSize)
{
	byte * pByte = (byte*)&tData;
	for (uint x = 0; x < szSize; x++)
		vResource.push_back(pByte[x]);
}

void WriteStringToResBuffer(std::vector<byte> & vResource, std::string szString)
{
	WriteToResBuffer<uint>(vResource, szString.size() + 1, sizeof(uint));
	
	for (int x = 0; x < szString.size(); x++)
	{
		vResource.push_back(szString[x]);
	}
	vResource.push_back(0);
}

void InitResBuffer(std::vector<byte>& vResource, LoaderInfo * loaderInfo)
{
	// ---------------- form settings ----------------
	//form title
	WriteStringToResBuffer(vResource, loaderInfo->formInfo.szFormTitle);

	//game title
 	WriteStringToResBuffer(vResource, loaderInfo->formInfo.szGameTitle);

	//author
	WriteStringToResBuffer(vResource, loaderInfo->formInfo.szAuthor);

	// ---------------- loader settings ----------------
	
	//process name
	WriteStringToResBuffer(vResource, loaderInfo->loadSettings.szProcessName);

	//random file names & upx
	WriteToResBuffer<BOOL>(vResource, loaderInfo->loadSettings.bRandomNames, sizeof(BOOL));
	WriteToResBuffer<BOOL>(vResource, loaderInfo->loadSettings.bUPX, sizeof(BOOL));

	// ---------------- injector settings ----------------
	WriteToResBuffer<uint>(vResource, loaderInfo->injSettings.iInjMethod, sizeof(uint));
	WriteToResBuffer<BOOL>(vResource, loaderInfo->injSettings.iLaunchMethod, sizeof(uint));
	WriteToResBuffer<BOOL>(vResource, loaderInfo->injSettings.bHideDebug, sizeof(BOOL));
	WriteToResBuffer<uint>(vResource, loaderInfo->injSettings.iPEHMethod, sizeof(uint));
	WriteToResBuffer<BOOL>(vResource, loaderInfo->injSettings.bShiftMod, sizeof(BOOL));
	WriteToResBuffer<BOOL>(vResource, loaderInfo->injSettings.bCleanDD, sizeof(BOOL));
	WriteToResBuffer<BOOL>(vResource, loaderInfo->injSettings.bUnlinkPEB, sizeof(BOOL));
	WriteToResBuffer<uint>(vResource, loaderInfo->injSettings.iDelay, sizeof(uint));
}


int CLoaderGen::Generate(tstring &szFilepath, LoaderInfo * loaderInfo, bool b64)
{
	this->szFilepath = szFilepath;
	//we have the info, we have where we're writing our loader
	//first thing is write loader to disk
	WriteLoaderBin(b64);

	//load loader config data
	std::vector<byte> vResource; //resource data
	InitResBuffer(vResource, loaderInfo);
	CResource resLoaderConfig(IDR_LOADERINFO1, LOADERINFO, vResource);
	
	CResourceManager rm;
	rm.BeginUpdate(szFilepath);
	
	//write config data to laoder
	rm.AddResource(&resLoaderConfig);
	
	//read dll off disk
	HANDLE hDll = NULL;
	std::vector<byte> vDll = LoadDLL(hDll);

	//write dll to loader
	CResource resDll(IDLOADERDLL, BINARY, vDll);
	rm.AddResource(&resDll);
	
	//write injector to loader
	std::vector<byte> vInj = LoadInjector(b64);
	CResource resInj(IDLOADERINJ, BINARY, vInj);
	rm.AddResource(&resInj);

	//write readme to loader if there is one
	std::vector<byte> vReadme;
	if (!this->szReadme.empty())
	{
		vReadme = LoadReadme();
		if (!vReadme.empty())
		{
			CResource resReadme(IDR_TEXTFILE1, TEXTFILE, vReadme); //IMPLEMENT ME!!! :(
			rm.AddResource(&resReadme);
		}
	}
	
	rm.UpdateResources();
	
	if (loaderInfo->loadSettings.bUPX)
	{
		upx.Compress(szFilepath);
		upx.Cleanup();
	}
	MessageBox(NULL, _T("AND we're done."), _T("COMPLEET SUCKSHESS!"), MB_ICONASTERISK | MB_OK);
	return 0;
}

int CLoaderGen::GetLoaderInfoSize(LoaderInfo * li)
{
	int size = li->formInfo.uFormTitleLen + 
		li->formInfo.uGameTitleLen + 
		li->formInfo.uAuthorLen + 
		(3 * sizeof(int));

	//size += li->loadSettings.uDllPathLen + 
	//	li->loadSettings.uReadmePathLen + 
	//	(4 * sizeof(int));
	//
	size += (5 * sizeof(int));

	return size;
}

void CLoaderGen::WriteLoaderBin(bool b64)
{
	CResourceWriter rw(GetModuleHandle(0));
	if (!b64)
	{
		rw.Write(szFilepath, IDLOADER32, BINARY);
	}
	else
	{
		rw.Write(szFilepath, IDLOADER64, BINARY);
	}
}

std::vector<byte> CLoaderGen::LoadDLL(HANDLE & hDll)
{
	std::vector<byte> vDll;
//	
//	hDll = OpenFile(szDllPath.c_str());
//	if (!hDll)
//	{
//		ErrorMsgBox(_T("Couldn't open dll for writing."));
//		CloseHandle(hDll);
//		return vDll;
//	}
//
//	LARGE_INTEGER largeInt{ 0 };
//	if (!GetFileSizeEx(hDll, &largeInt))
//	{
//		ErrorMsgBox(_T("Failed to get size of dll..."));
//		CloseHandle(hDll);
//		return vDll;
//	}
//	CloseHandle(hDll);
//
//	if (largeInt.HighPart)
//		vDll.resize(largeInt.HighPart); //can this potentially be too big???
//
//	vDll.resize(vDll.size() + largeInt.LowPart); //lol :s
////	if (!ReadFileEx(hDll, vDll.data(), (DWORD)vDll.size(), NULL, NULL))
//	//std::ifstream isDll(szDllPath.c_str());
//	//if(!isDll.is_open())
//	//{
//	//	ErrorMsgBox(_T("Failed to read dll..."));
//	//	//CloseHandle(hDll);
//	//	return vDll;
//	//}
//	//vDll.resize(vDll.size(), 0);
//	//isDll.read((char*)vDll.data(), vDll.size());
	ReadFileIntoVec(szDllPath, 0, vDll);
	return vDll;
}

std::vector<byte> CLoaderGen::LoadInjector(bool b64)
{
	std::vector<byte> vInj;
	CResourceReader rr(GetModuleHandle(0));
	vInj = rr.ReadIntoVector(b64 ? IDINJ64 : IDINJ32, BINARY);
	CResource resInj(IDLOADERINJ, BINARY, vInj);
	return vInj;
}

std::vector<byte> CLoaderGen::LoadReadme()
{
	//HANDLE hFile = CreateFile(szReadme.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//if (!hFile || hFile == INVALID_HANDLE_VALUE)
	//{
	//	CloseHandle(hFile);
	//	return std::vector<byte>(0);
	//}

	//DWORD dwSize = GetFileSize(hFile, nullptr);
	//if (!dwSize)
	//{
	//	CloseHandle(hFile);
	//	return std::vector<byte>(0);
	//}
	//CloseHandle(hFile);

	std::vector<byte> vBuffer;
	ReadFileIntoVec(szReadme, 0, vBuffer);
	if (vBuffer.back() != 0)
		vBuffer.push_back(0);
	return vBuffer;
}
