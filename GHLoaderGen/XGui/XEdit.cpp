#include "XEdit.h"

LRESULT CALLBACK EditCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (msg)
	{
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_TAB:
					//MessageBox(NULL, L"Pressed TAB", L"Test", MB_OK);
					break;
				case 0x41:
					//MessageBox(NULL, L"Pressed A", L"Lol", MB_OK);
					break;
				case VK_ESCAPE:
					return TRUE;
			}
			break;
		}
	}
	return DefSubclassProc(hWnd, msg, wParam, lParam);
}


CXEdit::CXEdit(CXWindow * pOwner, XID xID, int x, int y, int w, int h, tstring szText, CXFont * pFont)
	: CXControl(pOwner, xID, EDIT, x, y, w, h, szText, pFont)
{
	szClass = _T("EDIT");
	SetStyle(WS_XEDITML);
	this->bMultiLine = true;
}

CXEdit::~CXEdit()
{
}

bool CXEdit::Create()
{
	if (!bMultiLine && GetHeight() == 0)
	{
		TEXTMETRIC tm;
		GetTextMetrics(GetDC(hWnd), &tm);
		this->SetHeight(tm.tmHeight + 2);
	}
	if (!CreateXWindow())
		return false;

	SetWindowSubclass(hWnd, EditCallback, 2, (DWORD_PTR)this);

	if (pFont)
		SetFont(pFont);
	
	return true;
}

int CXEdit::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
		case VK_LBUTTON:
				return TRUE;
		case VK_ESCAPE:
			return TRUE;			
	}
	return TRUE;
}

int CXEdit::OnKey(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case VK_LBUTTON:
			return TRUE;
		case VK_ESCAPE:
			return TRUE;
	}
	return FALSE;
}

tstring CXEdit::GetText()
{
	szText.clear();
	szText.resize(Edit_GetTextLength(hWnd) + 1);
	Edit_GetText(hWnd, &szText[0], szText.size());
	return szText;
}
