#include "Binary.h"
#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp.lib")



CBinary::CBinary(tstring & szPath)
{
	this->szPath = szPath;
}

CBinary::~CBinary()
{
}

bool CBinary::Is64BitImage()
{
	ReadPEHeader();
	PIMAGE_NT_HEADERS pNT = ImageNtHeader(&vHeader[0]);
	if (pNT->FileHeader.Machine == IMAGE_FILE_MACHINE_I386)
		return false;

	return true;
}

bool CBinary::Exists()
{
	bool exists = false;
	HANDLE hFile = OpenFile(szPath.c_str());
	if (hFile != INVALID_HANDLE_VALUE)
		exists = true;
	CloseHandle(hFile);
	return exists;
}

tstring CBinary::GetPath()
{
	return szPath;
}

std::vector<byte>* CBinary::Read()
{
	if (Exists())
	{
		//GetFileSize()
	}
	return nullptr;
}


void CBinary::ReadPEHeader()
{
	////HANDLE hFile = CreateFile(szPath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//HANDLE hFile = OpenFile(szPath.c_str());
	//
	
	//char mem[0x1000]{ 0 };

	//std::ifstream in(szPath.c_str(), std::ifstream::binary | std::ifstream::ate);
	//size_t size = in.tellg();

	//if (!in.is_open() || in.bad() || size <= 0x1000)
	//{
	//	ErrorMsgBox(_T("Invalid PE file..."));
	//}
	//
	//in.seekg(std::ifstream::beg);

	//in.read(mem, 0x1000);
	//
	//in.close();
	//vHeader.resize(0x1000, 0);
	//memcpy_s(vHeader.data(), 0x1000, mem, 0x1000);
	//CloseHandle(hFile);
	ReadFileIntoVec(szPath, 0x1000, vHeader);
}
