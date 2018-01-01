#include "XLabel.h"



CXLabel::CXLabel(CXWindow * pOwner, XID xID, int x, int y, int w, int h, tstring szText, CXFont * pFont)
	: CXControl(pOwner, xID, LABEL, x, y, w, h, szText, pFont)
{
	szClass = _T("STATIC");
	SetStyle(WS_XLABEL);
	dwFlags = DT_LEFT;
	rcRect.left = x;
	rcRect.right = x + w;
	rcRect.top = y;
	rcRect.bottom = y + h;
}


CXLabel::~CXLabel()
{
}
bool CXLabel::Create()
{
	bool bRet = CXControl::Create();
	if (bRet)
	{
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)this);
		SetWindowSubclass(hWnd, OwnerDrawProc, 0, 0);
		SetText(szText);
	}
	return bRet;
}
/*

bool CXLabel::Create()
{
	bool bRet = CXControl::Create();
	if (bRet)
	{
		SetTxtColor(RGB(255, 0, 0));
		Static_SetText(hWnd, szText.c_str());
	}
	return bRet;
}*/

LRESULT CXLabel::OnDrawItem(WPARAM wParam, LPARAM lParam)
{
	DRAWITEMSTRUCT* pDi = (DRAWITEMSTRUCT*)lParam;
	RECT rc = { 0, 0, uWidth, uHeight };
	COLORREF bgr = GetBackgroundColor();
	HBRUSH hb = CreateSolidBrush(bgr);
	FillRect(pDi->hDC, &GetRect(), hb);
	SetTextColor(pDi->hDC, clText);
	SetBkMode(pDi->hDC, TRANSPARENT);
	if (pFont)
		pFont->SetFont(pDi->hDC);
	DrawText(pDi->hDC, szText.c_str(), szText.length(), &rcRect, dwFlags);
	return 0;
}

void CXLabel::SetDrawFlags(DWORD dtFlags)
{
	this->dwFlags = dtFlags;
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
}
