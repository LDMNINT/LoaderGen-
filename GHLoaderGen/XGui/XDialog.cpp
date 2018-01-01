#include "XDialog.h"
#include "XControls.h"

std::map<XDialogID, CXDialog*> mDialogs;
std::map<HWND, CXDialog*> mDialogsH;
CXDialog * g_pDialog = nullptr;

LRESULT CALLBACK DialogWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_KEYUP:
		{
			switch (wParam)
			{
				case VK_ESCAPE:
				{
					mDialogsH[hWnd]->Destroy();
					break;
				}
			}
		}
		case WM_COMMAND:
		{
			int id = LOWORD(wParam);
			if (mDialogsH[hWnd])
				if(mDialogsH[hWnd]->GetControls()->mControls[id])
					mDialogsH[hWnd]->GetControls()->mControls[id]->OnCommand(wParam, lParam);
			return 0;
		}
		case WM_DRAWITEM:
		{		
			if (mDialogsH[hWnd]->GetControls())
			{
				if (mDialogsH[hWnd]->GetControls()->mControls[wParam])
					return mDialogsH[hWnd]->GetControls()->mControls[wParam]->OnDrawItem(wParam, lParam);
			}
			break;
		}
		case WM_DESTROY:
		case WM_CLOSE:
		{
			mDialogsH[hWnd]->Destroy();
			return 1;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

CXDialog::CXDialog(CXWindow * pOwner, XID xID, int x, int y, int w, int h, tstring szText)
	: CXWindow(pOwner->GetInstance(), pOwner->GetHandle(), xID, DialogWndProc)
{
	RECT rect;
	GetWindowRect(pOwner->GetHandle(), &rect);
	SetPos(x,y);
	SetText(szText);
	this->pOwnerWindow = pOwner;
	SetClass(_T("XDialog"));
	SetStyle(WS_XDIALOG);
	SetWidth(w);
	SetHeight(h);
	SetBgColor(pOwner->GetBackgroundColor());
	this->hIcon = pOwnerWindow->GetIcon();
	this->hIconSm = pOwnerWindow->GetIconSm();
}

CXDialog::~CXDialog()
{
}

bool CXDialog::Create()
{
	auto wcx = CreateBasicClass(szClass);
	wcx.hIcon = pOwnerWindow->GetIcon();
	wcx.hIconSm = pOwnerWindow->GetIconSm();
	RegisterClassEx(&wcx);
	xDialogID = xID;
	xID = 0;
	mDialogs[xDialogID] = this;
	bool bRet = CreateXWindow();
	if (!bRet)
		return false;
	mDialogsH[hWnd] = this;
	xControls = new CXControls(this);
	return true;
}

void CXDialog::Show()
{
	if (!hWnd)
		Create();
	ShowWindow(hWnd, SW_SHOW);
	bShown = true;
}

void CXDialog::Destroy()
{
	ShowWindow(hWnd, SW_HIDE);
	bShown = false;
}

CXControls* CXDialog::GetControls()
{
	return xControls;
}
