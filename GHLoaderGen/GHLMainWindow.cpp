#include "GHLMainWindow.h"
#include "Binary.h"

GHLMainWindow * g_pMain = nullptr;

void BtnCallback(uintptr_t iCommand)
{
	switch (iCommand)
	{
		case SELECT_PROCESS:
			g_pMain->SelectProcess();
			break;
		case SELECT_DLL:
			g_pMain->SelectDll();
			break;
		case SELECT_README:
			g_pMain->SelectReadme();
			break;
		case SELECT_GH:
			g_pMain->LaunchGH();
			break;
		case SELECT_BUILD:
			g_pMain->GenerateLoader();
			break;
	}
}

void InjMethodCallback(uintptr_t iMethod)
{
	g_pMain->InjMethodRestrictions(iMethod);
}

void LaunchMethodCallback(uintptr_t iMethod)
{
	g_pMain->LaunchMethodRestrictions(iMethod);
}

GHLMainWindow::GHLMainWindow(HINSTANCE hInstance, int x, int y)
	: CXMainWindow(hInstance, SZWND_TITLE, XMAINWNDCLS, x, y, GHL_WND_W, GHL_WND_H)
{
	g_pMainWindow = this;
	g_pMain = this;
	SetBgColor(GHL_WND_BGCLR);
	SetStyle(WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX);

	this->hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	this->hIconSm = hIcon;

	if (!Create())
	{
		//error ;P
		exit(-1);
	}

	
	CreateControls();
}


GHLMainWindow::~GHLMainWindow()
{
}

void GHLMainWindow::SelectProcess()
{
	pProcPicker->Show();
}

void GHLMainWindow::ProcessSelected()
{
	targetProc = this->pProcPicker->GetSelectedProcess();
	if (!targetProc)
	{
		CXControls* pC = pControls->GetControl<CXGroupBox>(IDGRP_PROCESS)->pControls;

		pC->GetControl<CXEdit>(IDEDT_PROCNAME)->SetText(targetProc->GetProcName());
		CXLabel * pL = pC->GetControl<CXLabel>(IDLBL_PID);
		tstring buf(22, 0);
		_itot_s(targetProc->GetPID(), &buf[0], 22, 10);
		tstring pid = _T("PID: ");
		pid += buf;
		pid.resize(lstrlen(pid.c_str()));
		pL->SetText(pid);
		bool b64 = targetProc->Is64Bit();
		pL = pC->GetControl<CXLabel>(IDLBL_ARCH);
		tstring arch = _T("Arch: ");
		arch += (b64) ? _T("x64") : _T("x86");
		pL->SetText(arch);

		pC->GetControl<CXIcon>(0x1111)->Load(targetProc->GetIcon());
	}
	SetFocus(hWnd);
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
}

void GHLMainWindow::SelectDll()
{
	CXFileDialogFilterEntry fe(_T("Dll file | (*.dll)"), _T("*.dll"));
	CXFileDialog fd(this, OPEN, &fe);
	tstring szPath;
	if (fd.Show(szPath))
		pControls->GetControl<CXEdit>(IDEDT_DLLPATH)->SetText(szPath);
}

void GHLMainWindow::SelectReadme()
{
	CXFileDialogFilterEntry fe(_T("Text file | (*.txt)"), _T("*.txt"));
	CXFileDialog fd(this, OPEN, &fe); 
	tstring szPath;
	if (fd.Show(szPath))
		pControls->GetControl<CXEdit>(IDEDT_README)->SetText(szPath);
}

void GHLMainWindow::LaunchGH()
{
	MessageBox(hWnd, _T("Thanks for using the official GuidedHacking loader!\n-GH Team"), _T("Heyo!"), MB_ICONINFORMATION | MB_OK);
	ShellExecute(hWnd, _T("open"), _T("http://guidedhacking.com"), NULL, NULL, SW_SHOW);
}

void GHLMainWindow::GenerateLoader()
{
	//first we need to check if everything is good to go
	//check if there's a process selected and a dll ready to load
	if (!targetProc)
	{
		MessageBox(hWnd, _T("Please select a process..."), _T("Yo dumbass!"), MB_ICONERROR | MB_OK);
		return;
	}


	if (!lstrcmp(pControls->GetControl<CXEdit>(IDEDT_DLLPATH)->GetText().c_str(), _T("")))
	{
		MessageBox(hWnd, _T("Might help if you select a dll to load..."), _T("Yo dumbass!"), MB_ICONERROR | MB_OK);
		return;
	}

	//setup struct that'll basically be our resource that allows us to configure our loader however we please
	LoaderInfo li;
	GetLoaderInfo(li);

	CBinary dll(pControls->GetControl<CXEdit>(IDEDT_DLLPATH)->GetText());

	//check if dll even exists...
	if (!dll.Exists())
	{
		MessageBox(NULL, _T("Invalid path to dll!"), _T("Error!"), MB_ICONERROR | MB_OK);
		return;
	}

	//check if dll matches arch of selected process
	if (targetProc->Is64Bit() != dll.Is64BitImage())
	{
		MessageBox(NULL, _T("Architecture of DLL does not match that of the game!"), _T("Error!"), MB_ICONERROR | MB_OK);
		return;
	}
	
	CXFileDialogFilterEntry fdf(_T("Executable | (*.exe)"), _T("*.exe"));
	CXFileDialog fd(this, SAVE, &fdf);
	tstring filepath;
	fd.Show(filepath);

	//generate loader :D
	pGen = new CLoaderGen(&dll.GetPath(), &pControls->GetControl<CXEdit>(IDEDT_README)->GetText());
	pGen->Generate(filepath, &li, dll.Is64BitImage());
}

void GHLMainWindow::InjMethodRestrictions(uintptr_t iMethod)
{
	auto pInjGrp = pControls->GetControl<CXGroupBox>(IDGRP_INJSET)->pControls;
	auto pChkUnlinkPEB = pInjGrp->vGroups[IDGRP_INJSET]->GetControl<CXCheckBox>(IDCHK_ULNKPEB);
	auto pChkShift = pInjGrp->vGroups[IDGRP_INJSET]->GetControl<CXCheckBox>(IDCHK_SHIFT);
	auto pChkCleanDir = pInjGrp->vGroups[IDGRP_INJSET]->GetControl<CXCheckBox>(IDCHK_CDDIR);
	if (iMethod != 2)
	{
		pChkShift->SetCheck(false);
		pChkShift->Disable(true);
		pChkCleanDir->SetCheck(false);
		pChkCleanDir->Disable(true);
		pChkUnlinkPEB->Disable(false);
	}
	else
	{	//manual mapping
		pChkUnlinkPEB->SetCheck(true);
		pChkUnlinkPEB->Disable(true);
		pChkShift->Disable(false);
		pChkCleanDir->Disable(false);
	}
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
}

void GHLMainWindow::LaunchMethodRestrictions(uintptr_t iMethod)
{
	auto pInjGrp = pControls->GetControl<CXGroupBox>(IDGRP_INJSET)->pControls;
	auto pChkHide = pInjGrp->vGroups[IDGRP_INJSET]->GetControl<CXCheckBox>(IDCHK_HIDEDBG);
	switch (iMethod)
	{
		case 0:
		{
			pChkHide->Disable(false);
			break;
		}
		default:
		{
			pChkHide->SetCheck(false);
			pChkHide->Disable(true);
		}
	}
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
}

int GHLMainWindow::CreateControls()
{
	CXControls* pC;
	CXControl* pCtrl;
	CXGroupBox * pGb;
	CXButton * pBtn;
	std::function<void(uintptr_t)> bc = BtnCallback;

	// ============================================= LOADER SETTINGS =============================================
	pGb = pControls->AddControl<CXGroupBox>(IDGRP_LOADSET, 10, 5, 270, 115, _T("Loader Settings"));
	pGb->SetTxtColor(RGB(200, 200, 200));
	pGb->SetControlManager(new CXControls(pGb));
	pC = pGb->pControls;
	
	pC->AddControl<CXLabel>(IDLBL_FORMNAME, 10, 23, 90, 25, _T("Window Title:"));
	pC->AddControl<CXEdit>(IDEDT_FORMNAME, 110, 20, 150, 25, _T("SupaDupaHax"));
	pC->AddControl<CXLabel>(IDLBL_GAMENAME, 10, 53, 90, 25, _T("Game Title:"));
	pC->AddControl<CXEdit>(IDEDT_GAMENAME, 110, 50, 150, 25, _T("AssaultCube"));
	pC->AddControl<CXLabel>(IDLBL_AUTHOR, 10, 83, 90, 25, _T("Author:"));
	pC->AddControl<CXEdit>(IDEDT_AUTHOR, 110, 80, 150, 25, _T("Traxin"));

	pControls->AddControl<CXEdit>(IDEDT_DLLPATH, 290, 45, 275, 25, _T("C:\\hax.dll"));
	pBtn = pControls->AddControl<CXButton>(IDBTN_DLLSELECT, 290, 10, 125, 25, _T("Select DLL"));
	pBtn->SetAction(bc);
	pBtn->SetCommandArgs(SELECT_DLL);
		
	pControls->AddControl<CXEdit>(IDEDT_README, 290, 80, 275, 25, _T("C:\\readme.txt"));
	pBtn = pControls->AddControl<CXButton>(IDBTN_README, 440, 10, 125, 25, _T("Select Readme"));
	pBtn->SetAction(bc);
	pBtn->SetCommandArgs(SELECT_README);
	
#define DIV 115

	// ============================================= PROCESS =============================================
	pGb = pControls->AddControl<CXGroupBox>(IDGRP_PROCESS, 10, DIV+10, 200, 115, _T("Process"));
	pGb->SetTxtColor(RGB(200, 200, 200));
	RECT rc = pGb->GetAdjustedRect();
	pGb->SetControlManager(new CXControls(pGb));

	pC = pGb->pControls;
	pCtrl = pC->AddControl<CXLabel>(IDLBL_PROCNAME, 10, 23, 50, 25, _T("Name:"));

	//pC->AddGroup(IDGRP_PROCESS);
	//pC->AddControlToGroup<CXLabel>(IDGRP_PROCESS, IDLBL_PROCNAME, 10, 23, 40, 25, L"Name:");
	pC->AddControl<CXEdit>(IDEDT_PROCNAME, 65, 20, 120, 25, _T("ac_client.exe"));
	pCtrl = pC->AddControl<CXLabel>(IDLBL_PID, 10, 55, 100, 25, _T("PID: "));
	pCtrl = pC->AddControl<CXLabel>(IDLBL_ARCH, 10, 85, 100, 25, _T("Arch: "));
	auto pIco = pC->AddControl<CXIcon>(0x1111, 120, 60, 32, 32, _T(" "));
	pIco->Load(LoadIcon(NULL, MAKEINTRESOURCE(IDI_ERROR)));

	pBtn = pControls->AddControl<CXButton>(IDBTN_PROCSELECT, 14, DIV + 130, 190, 25, _T("Select Process"));
	pProcPicker = new CGHLProcPicker(this, targetProc);
	std::function<void()> ppc = ProcPickerCallback;
	pProcPicker->SetProcPickerCallback(ppc);
	pBtn->SetAction(bc);
	pBtn->SetCommandArgs(SELECT_PROCESS);

	// ============================================= INJECTOR SETTINGS =============================================
	pGb = pControls->AddControl<CXGroupBox>(IDGRP_INJSET, 220, DIV + 10, 350, 190, _T("Injection Settings"));
	pGb->SetTxtColor(RGB(200, 200, 200));
	pGb->SetControlManager(new CXControls(pGb));
	pC = pGb->pControls;

	std::function<void(uintptr_t)> imc = InjMethodCallback;
	pC->AddGroup(IDGRP_INJSET);
	pC->AddControlToGroup<CXRadioButton>(IDGRP_INJSET, IDRDO_LOADLIB, 10, 20, 125, 25, _T("LoadLibrary"));
	auto pCheck = pC->vGroups[IDGRP_INJSET]->GetControl<CXRadioButton>(IDRDO_LOADLIB);
	pCheck->SetAction(imc);
	pCheck->SetCommandArgs(0);
	pCheck->SetCheck(true);

	pC->AddControlToGroup<CXRadioButton>(IDGRP_INJSET, IDRDO_LDRLOAD, 125, 20, 125, 25, _T("LdrLoadDll"));
	pCheck = pC->vGroups[IDGRP_INJSET]->GetControl<CXRadioButton>(IDRDO_LDRLOAD);
	pCheck->SetAction(imc);
	pCheck->SetCommandArgs(1);

	pC->AddControlToGroup<CXRadioButton>(IDGRP_INJSET, IDRDO_MANMAP, 235, 20, 125, 25, _T("Manual Map"));
	pCheck = pC->vGroups[IDGRP_INJSET]->GetControl<CXRadioButton>(IDRDO_MANMAP);
	pCheck->SetAction(imc);
	pCheck->SetCommandArgs(2);

	//pC->AddControlToGroup<CXCheckBox>(IDGRP_INJSET, IDCHK_THIJACK, 10, 50, 150, 25, L"NtCreateThreadEx");
	CXComboBox* pCB = pC->AddControl<CXComboBox>(IDCBX_LAUNCH, 10, 50, 160, 25, _T("NtCreateThreadEx"));
	pCB->AddString(_T("Thread Hijacking"));
	pCB->AddString(_T("SetWindowsHookEx"));
	pCB->AddString(_T("QueueUserAPC"));

	pCB->SetSelectionChangeCB(LaunchMethodCallback, 0);

	pC->AddControlToGroup<CXCheckBox>(IDGRP_INJSET, IDCHK_HIDEDBG, 170, 50, 175, 25, _T("Hide from Debugger"));

	pCB = pC->AddControl<CXComboBox>(IDCBX_PEH, 10, 90, 120, 25, _T("Keep PEH"));
	pCB->AddString(_T("Erase PEH"));
	pCB->AddString(_T("Fake PEH"));

	pC->AddControlToGroup<CXCheckBox>(IDGRP_INJSET, IDCHK_ULNKPEB, 10, 115, 150, 25, _T("Unlink from PEB"));
	pCheck = pC->vGroups[IDGRP_INJSET]->GetControl<CXRadioButton>(IDCHK_ULNKPEB);
	pCheck->SetCheck(true);

	pC->AddControlToGroup<CXCheckBox>(IDGRP_INJSET, IDCHK_SHIFT, 170, 90, 175, 25, _T("Shift Module"));
	pCheck = pC->vGroups[IDGRP_INJSET]->GetControl<CXRadioButton>(IDCHK_SHIFT);
	pCheck->Disable(true);

	pC->AddControlToGroup<CXCheckBox>(IDGRP_INJSET, IDCHK_CDDIR, 170, 115, 175, 25, _T("Clean Data Directories"));
	pCheck = pC->vGroups[IDGRP_INJSET]->GetControl<CXRadioButton>(IDCHK_CDDIR);
	pCheck->Disable(true);
	
	pCtrl = pC->AddControl<CXLabel>(IDLBL_DELAY, 50, 148, 100, 25, _T("Delay: "));
	pCtrl->SetTxtColor(RGB(200, 200, 200));
	pCtrl = pC->AddControl<CXEdit>(IDEDT_DELAY, 120, 145, 200, 25, _T("0"));
	pCtrl->SetStyle(pCtrl->GetStyle() | ES_NUMBER);

	// ============================================= FINALE =============================================
	pControls->AddControl<CXCheckBox>(IDCHK_RANDNAMES, 10, DIV + 160, 180, 25, _T("Randomize File Names"));
	pControls->AddControl<CXCheckBox>(IDCHK_UPXFILES, 10, DIV + 180, 180, 25, _T("UPX Loader"));

	pBtn = pControls->AddControl<CXButton>(IDBTN_BUILD, 10, DIV + 210, 510, 25, _T("Generate Loader!"));
	pBtn->SetAction(bc);
	pBtn->SetCommandArgs(SELECT_BUILD);
	pBtn = pControls->AddControl<CXButton>(IDBTN_GH, 530, DIV + 210, 40, 25, _T(""));
	pBtn->SetIcon(hIcon);
	pBtn->SetAction(bc);
	pBtn->SetCommandArgs(SELECT_GH);

	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);

	return 0;
}

void GHLMainWindow::GetLoaderInfo(LoaderInfo & li)
{
	//form info
	CXGroupBox* pGB = pControls->GetControl<CXGroupBox>(IDGRP_LOADSET);

#if _UNICODE
	li.formInfo.szFormTitle = WCS2MBS(pGB->pControls->GetControl<CXEdit>(IDEDT_FORMNAME)->GetText());
	li.formInfo.szGameTitle = WCS2MBS(pGB->pControls->GetControl<CXEdit>(IDEDT_GAMENAME)->GetText());
	li.formInfo.szAuthor = WCS2MBS(pGB->pControls->GetControl<CXEdit>(IDEDT_AUTHOR)->GetText());
#else
	li.formInfo.szFormTitle = pGB->pControls->GetControl<CXEdit>(IDEDT_FORMNAME)->GetText();
	li.formInfo.szGameTitle = pGB->pControls->GetControl<CXEdit>(IDEDT_GAMENAME)->GetText();
	li.formInfo.szAuthor = pGB->pControls->GetControl<CXEdit>(IDEDT_AUTHOR)->GetText();
#endif
	li.formInfo.uFormTitleLen = li.formInfo.szFormTitle.length();
	li.formInfo.uGameTitleLen = li.formInfo.szGameTitle.length();
	li.formInfo.uAuthorLen = li.formInfo.szAuthor.length();

	// loader settings
	pGB = pControls->GetControl<CXGroupBox>(IDGRP_PROCESS);

#if _UNICODE
	li.loadSettings.szProcessName = WCS2MBS(pGB->pControls->GetControl<CXEdit>(IDEDT_PROCNAME)->GetText());
#else
	li.loadSettings.szProcessName = pGB->pControls->GetControl<CXEdit>(IDEDT_PROCNAME)->GetText();
#endif
	li.loadSettings.uProcNameLen = li.loadSettings.szProcessName.length();
	li.loadSettings.bRandomNames = pControls->GetControl<CXCheckBox>(IDCHK_RANDNAMES)->GetCheck();
	li.loadSettings.bUPX = pControls->GetControl<CXCheckBox>(IDCHK_UPXFILES)->GetCheck();

	//inj settings
	pGB = pControls->GetControl<CXGroupBox>(IDGRP_INJSET);
	CXControls* pC = pGB->pControls->vGroups[IDGRP_INJSET];
	
	int injMethod = 0;
	
	if (pC->GetControl<CXRadioButton>(IDRDO_LDRLOAD)->GetCheck())
		injMethod = 1;
	else if (pC->GetControl<CXRadioButton>(IDRDO_MANMAP)->GetCheck())
		injMethod = 2;

	li.injSettings.iInjMethod = injMethod;
	pGB->pControls->vGroups[IDGRP_INJSET]->GetControl<CXCheckBox>(IDRDO_LOADLIB);
	// todo: need to get figure out which radio button in the group is selected
	int pehMethod = pGB->pControls->GetControl<CXComboBox>(IDCBX_PEH)->GetSelectedIndex();
	
	pC = pGB->pControls;
	li.injSettings.iPEHMethod = pehMethod;
	li.injSettings.iLaunchMethod = pC->GetControl<CXComboBox>(IDCBX_LAUNCH)->GetSelectedIndex();
	li.injSettings.bUnlinkPEB = pC->vGroups[IDGRP_INJSET]->GetControl<CXCheckBox>(IDCHK_ULNKPEB)->GetCheck();


	tstring buf = pGB->pControls->GetControl<CXEdit>(IDEDT_DELAY)->GetText();
	li.injSettings.iDelay = _ttoi(buf.c_str());
}

void ProcPickerCallback()
{
	g_pMain->ProcessSelected();
}
