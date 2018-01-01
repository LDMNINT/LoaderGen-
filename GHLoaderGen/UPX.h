#pragma once
#include "resource.h"
#include "ProcManager.h"
#include "Binary.h"
#include "XResource.h"

#define IDUPX							IDR_BINARY5

class CUPX
{
public:
	CUPX(tstring szFilepath);
	~CUPX();

	void Compress(tstring szTarget);
	void Cleanup();

private:
	void Write();

private:
	ProcManager pm;
	CBinary binUPX;
	tstring szFilepath;
	CResourceWriter resWriter;
};

