#include "Utility.h"
//#include <vector>
std::wstring MBS2WCS(std::string szString)
{
	size_t cc;
	std::wstring szBuf(szString.length() * 2, 0);
	mbstowcs_s(&cc, &szBuf[0], szBuf.size(), szString.c_str(), szString.length());
	return szBuf.c_str();
}

std::string WCS2MBS(std::wstring szString)
{
	size_t cc;
	std::string szBuf(szString.length() * 2, 0);
	wcstombs_s(&cc, &szBuf[0], szBuf.size(), szString.c_str(), szString.length());
	return szBuf.c_str();
}

bool ReadFileIntoVec(tstring szFilepath, size_t szSize, std::vector<byte> & vData)
{
	std::ifstream in(szFilepath.c_str(), std::ifstream::binary | std::ifstream::ate);
	size_t size = szSize > 0 ? szSize : in.tellg();

	if (!in.is_open() || in.bad())
	{
		tstring error = _T("Failed to open:\n");
		error.append(szFilepath);
		ErrorMsgBox(error.c_str());
		return false;
	}

	in.seekg(std::ifstream::beg);
	vData.resize(size, 0);
	in.read((char*)vData.data(), size);
	in.close();
	return true;
}
