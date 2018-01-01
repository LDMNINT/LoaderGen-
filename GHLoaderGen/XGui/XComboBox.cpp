#include "XComboBox.h"

LRESULT CALLBACK ComboBoxCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (msg)
	{
		case WM_COMMAND:
		{
			((CXComboBox*)dwRefData)->OnCommand(wParam, lParam);
			break;
		}
	}
	return DefSubclassProc(hWnd, msg, wParam, lParam);
}

CXComboBox::CXComboBox(CXWindow * pOwner, XID xID, int x, int y, int w, int h, tstring szText, CXFont * pFont)
	: CXControl(pOwner, xID, COMBOBOX, x, y, w, h, szText, pFont)
{
	szClass = WC_COMBOBOX;
	SetStyle(WS_XCOMBOBOXDROPLIST); // can't change this after creation :'( //would probably need to re-create the window which would prolly flicka	
}

CXComboBox::~CXComboBox()
{
}

bool CXComboBox::Create()
{
	bool bRet = CXControl::Create();
	SetWindowSubclass(hWnd, ComboBoxCallback, 2, (DWORD_PTR)this);
	AddString(szText);
	ComboBox_SetCurSel(hWnd, 0);
	return bRet;
}

void CXComboBox::AddString(tstring szText)
{
	this->szText = szText;
	ComboBox_AddString(hWnd, szText.c_str());
}

int CXComboBox::GetSelectedIndex()
{
	return ComboBox_GetCurSel(hWnd);
}

int CXComboBox::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) == CBN_SELCHANGE)
	{
		if (fnSelectionChangeCallback)
			fnSelectionChangeCallback(GetSelectedIndex());
	}
	return 0;
}

void CXComboBox::SetSelectionChangeCB(std::function<void(uintptr_t)> fnSelectionChangeCallback, uintptr_t pArg)
{
	this->fnSelectionChangeCallback = fnSelectionChangeCallback;
	this->pSelectionChangeArg = pArg;
}
