#include "XControl.h"

CXControl::CXControl(CXWindow * pOwner, XID xID, ECXControl ControlType, RECT rcRect, tstring szText, CXFont * pFont)
	: CXWindow(pOwner->GetInstance(), pOwner->GetHandle(), xID)
{
	this->ControlType = ControlType;
	SetRect(rcRect);
	SetText(szText);
}

CXControl::CXControl(CXWindow * pOwner, XID xID, ECXControl ControlType, int x, int y, int w, int h, tstring szText, CXFont * pFont)
	: CXWindow(pOwner->GetInstance(), pOwner->GetHandle(), xID)
{
	this->ControlType = ControlType;
	this->pFont = pFont;
	this->pOwnerWindow = pOwner;
	hOwner = pOwner->GetHandle();
	SetText(szText);
	RECT rc;
	rc.left = x;
	rc.right = x + w;
	rc.top = y;
	rc.bottom = y + h;
	SetRect(rc);
	hBgr = (HBRUSH)pOwner->GetBackground();
	clBgr = pOwner->GetBackgroundColor();
}

CXControl::~CXControl()
{
}

bool CXControl::Create()
{
	if (!CreateXWindow())
		return false;
	if (pFont)
		SetFont(pFont);
	return true;
}

ECXControl CXControl::GetControlType()
{
	return this->ControlType;
}

RECT CXControl::GetAdjustedRect()
{
	RECT rc;
	rc.left = GetPos().x;
	rc.top = GetPos().y;
	rc.right = rc.left + GetWidth();
	rc.bottom = rc.top + uHeight;
	return rc;
}

void CXControl::SetAction(std::function<void(uintptr_t)> fnAction)
{
	this->fnAction = fnAction;
}

void CXControl::SetCommandArgs(uintptr_t pArgs)
{
	this->pCommandArgs = pArgs;
}

int CXControl::OnCommand(WPARAM wParam, LPARAM, uintptr_t pArg1)
{
	return 0;
}

int CXControl::OnCommand(WPARAM wParam, LPARAM)
{
	return 0;
}

void CXControl::SetFont(CXFont * pFont)
{
	HDC hDC = GetDC(hWnd);
	SetTextColor(hDC, clText);
	this->pFont = pFont;
	SendMessage(hWnd, WM_SETFONT, (WPARAM)pFont->GetHandle(), (LPARAM)MAKELONG(TRUE, 0));
}

void CXControl::SetTxtColor(COLORREF clText)
{
	this->clText = clText;
	SendMessage(hWnd, WM_CTLCOLOR, (WPARAM)GetDC(hWnd), 0);
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
}

LRESULT CXControl::OnDrawItem(WPARAM wParam, LPARAM lParam)
{
	return LRESULT(0);
}

LRESULT CXControl::OnCtlColor(WPARAM wParam, LPARAM lParam)
{
	if (!hBgr)
		hBgr = CreateSolidBrush(pOwnerWindow->GetBackgroundColor());
	return (LRESULT)hBgr;
}

void CXControl::OnHover(POINT p)
{
}

void CXControl::OnHScroll(WPARAM wParam, LPARAM lParam)
{
}

void CXControl::OnPaint(WPARAM wParam, LPARAM lParam)
{
}

int CXControl::OnKey(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CXControl::OnNotify(UINT code, LPARAM lParam)
{
}
