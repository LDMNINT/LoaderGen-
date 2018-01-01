#pragma once
#include "XControl.h"
#define WS_XEDIT	WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL
#define WS_XEDITML	WS_CHILD | WS_TABSTOP | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL

class CXEdit :
	public CXControl
{
public:
	CXEdit(CXWindow * pOwner, XID xID, int x, int y, int w, int h, tstring szText, CXFont * pFont = nullptr);
	~CXEdit();

	virtual bool Create();
	virtual int OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int OnKey(WPARAM wParam, LPARAM lParam);
	tstring GetText();

private:
	bool bMultiLine = false;
};

