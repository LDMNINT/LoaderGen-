#include "XTrackBar.h"

#define WS_XTRACKBAR WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE

CXTrackBar::CXTrackBar(CXWindow * pOwner, XID xID, int x, int y, int w, int h, tstring szText, CXFont * pFont)
	: CXControl(pOwner, xID, TRACKBAR, x, y, w, h, szText, pFont)
{
	szClass = TRACKBAR_CLASS;
	SetStyle(WS_XTRACKBAR);
}


CXTrackBar::~CXTrackBar()
{
}

bool CXTrackBar::Create()
{
	INITCOMMONCONTROLSEX icex;

	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	bool bRet = CXControl::Create();
	SendMessage(hWnd, TBM_SETRANGE, TRUE, MAKELONG(iMin, iMax));
	SendMessage(hWnd, TBM_SETPAGESIZE, 0, 4);
	SendMessage(hWnd, TBM_SETTICFREQ, 10, 0);
	SendMessage(hWnd, TBM_SETPOS, TRUE, iSelMin);
	
	return bRet;
}

void CXTrackBar::OnNotify(UINT code, LPARAM lParam)
{
	return;
}

void CXTrackBar::OnHScroll(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
		case TB_THUMBTRACK:
		{
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
			break;
		}
	}
}

LRESULT CXTrackBar::OnCtlColor(WPARAM wParam, LPARAM lParam)
{
	if (!hBgr)
		hBgr = CreateSolidBrush(pOwnerWindow->GetBackgroundColor());
	return (LRESULT)hBgr;
}
