#include "GHLProcPicker.h"
#include "XGui\XControls.h"

void BtnCallback_SelectProcess(uintptr_t pProcPicker)
{
	reinterpret_cast<CGHLProcPicker*>(pProcPicker)->SelectProcess();
}

CGHLProcPicker::CGHLProcPicker(CXWindow* pMainWindow, CProcess * procTarget)
	: CXDialog(pMainWindow, 0x0990, pMainWindow->GetRect().right, pMainWindow->GetRect().top, 600,350, _T("Proc'Picker"))
{
	this->procTarget = &procTarget;
}


CGHLProcPicker::~CGHLProcPicker()
{
}

void CGHLProcPicker::Show()
{
	if (!hWnd)
		Init();

	auto xLV = xControls->GetControl<CXListView>(IDLIST1);
	ListView_DeleteAllItems(xLV->GetHandle());
	FillList();
	ShowWindow(hWnd, SW_SHOW);
	bShown = true;
}

void CGHLProcPicker::FillList()
{
	//to do
	// enumerate open processes, should probably spawn processes to do this (oooorrrr NOT XD)
	// load binary icon from resource, also implement reading resource from binary in resmanager class
	// list sorting, by arch, name (A-Z || Z-A), pid, process start time or duration running might be nice too

	procManager.LoadProcs();
	int x = 0;
	tstring pid(22, 0);
	ProcVec pv = procManager.GetProcs();
	//HICON hApp = LoadIcon(NULL, IDI_APPLICATION);
	auto xLV = xControls->GetControl<CXListView>(IDLIST1);
	xLV->Clear();
	for (auto p : pv)
	{
		//HICON i = ExtractIcon(hInstance, p->GetProcPath().c_str(), 0);
		//i = p->GetIcon();
		//vIconsSmall.push_back(p->GetIcon());
		CXListViewItem * listItem = new CXListViewItem(x, 0, L"", p->GetIconSm());
		listItem->AddSubItem(x, 1, (p->Is64Bit()) ? L"x64" : L"x86");		
		listItem->AddSubItem(x, 2, p->GetProcName());
		_itow_s(p->GetPID(), &pid[0], 22, 10);
		listItem->AddSubItem(x, 3, pid);
		listItem->AddSubItem(x, 4, p->GetProcPath());
		xLV->AddItem(*listItem);
		x++;
	}
	xLV->SetIconList();
	//DestroyIcon(hApp);
}

void CGHLProcPicker::Init()
{
	Create();
	GetRect();
	xControls = new CXControls(this);
	
	xControls->AddControl<CXListView>(IDLIST1, 15, 15, 550, 250, _T(" "));
	CXListView* list = xControls->GetControl<CXListView>(IDLIST1);
	list->SetColumnWidth(0, 32);
	list->InsertColumn(_T("Arch"), 48, LVCFMT_CENTER);
	list->InsertColumn(_T("Process Name"), 100, LVCFMT_LEFT);
	list->InsertColumn(_T("PID"), 48, LVCFMT_CENTER);
	list->InsertColumn(_T("Path"), uWidth/2);

	//xControls->AddControl<CXButton>(IDLBL_REFRESH, 15, 240, 120, 25, L"Refresh");

	RECT rc = list->GetRect();

	CXButton * pBtn = xControls->AddControl<CXButton>(IDBUTTON1, 15, rc.bottom + 10, 550, 25, _T("Select Process"));
	std::function<void(uintptr_t)> bc = BtnCallback_SelectProcess;
	pBtn->SetAction(bc);
	pBtn->SetCommandArgs((uintptr_t)this);

	procManager.LoadProcs();
}

void CGHLProcPicker::SelectProcess()
{
	int iSelected = xControls->GetControl<CXListView>(IDLIST1)->GetSelectedIndex();
	if (iSelected < 0)
		return;
	*procTarget = procManager.GetProcs().at(iSelected);
	ppc();
	this->Destroy();
}

void CGHLProcPicker::Destroy()
{
	CXDialog::Destroy();
	ppc();
}

void CGHLProcPicker::SetProcPickerCallback(std::function<void()> ppc)
{
	this->ppc = ppc;
}

CProcess * CGHLProcPicker::GetSelectedProcess()
{
	return *procTarget;
}
