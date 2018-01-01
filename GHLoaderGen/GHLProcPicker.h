#pragma once
#include "XGui\XDialog.h"
#include "ProcManager.h"

#define IDLIST1				0x1000
#define IDBUTTON1			0x1001
#define IDLBL_REFRESH		0x1002

class CGHLProcPicker : public CXDialog
{
public:
	CGHLProcPicker(CXWindow* pMainWindow, CProcess * procTarget);
	~CGHLProcPicker();
	void Show();
	void FillList();
	void Init();
	void SelectProcess();
	void Destroy();
	void SetProcPickerCallback(std::function<void()> ppc);
	CProcess* GetSelectedProcess();
private:
	ProcManager procManager;
	CProcess ** procTarget;
	std::function<void()> ppc;
	std::vector<HICON> vIconsSmall;
};

