#include "XIcon.h"



CXIcon::CXIcon(CXWindow * pOwner, XID xID, int x, int y, int w, int h, tstring szText, CXFont * pFont)
	: CXControl(pOwner, xID, IMAGE, x, y, w, h, szText, nullptr)
{
	SetStyle(WS_CHILD | WS_VISIBLE | SS_OWNERDRAW);
	SetClass(_T("STATIC"));
}

CXIcon::~CXIcon()
{
}

bool CXIcon::Load(HICON hIcon)
{
	bool bRet = true;
	if (!hWnd)
		bRet = Create();
	this->hIcon = hIcon;
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
	return bRet;
}


LRESULT CXIcon::OnDrawItem(WPARAM wParam, LPARAM lParam)
{
	DRAWITEMSTRUCT* pDi = (DRAWITEMSTRUCT*)lParam;
	FillRect(pDi->hDC, &GetRect(), hBgr);
	DrawIcon(pDi->hDC, rcRect.left, rcRect.top, hIcon);
	return LRESULT(1);
}

void CXIcon::DrawIco(HWND hWnd, HDC hdcDest, HICON hIcon, RECT & pRect)
{
	DrawIcon(hdcDest, pRect.left, pRect.top, hIcon);
}
