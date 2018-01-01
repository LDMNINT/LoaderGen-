#pragma once
#include <stdafx.h>

class CBinary
{
public:
	CBinary(tstring & szPath);
	~CBinary();
	bool Is64BitImage();
	bool Exists();
	tstring GetPath();
	std::vector<byte> * Read();
private:
	void ReadPEHeader();
	tstring szPath;
	std::vector<byte> vHeader;
	std::vector<byte> vImage;
};

