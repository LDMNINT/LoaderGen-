#pragma once
#include "XControl.h"
class CXTrackBar :	public CXControl
{
	int iMin = 0, iMax = 100;
	int iSelMin = 25, iSelMax = 75;

public:
	CXTrackBar(CXWindow * pOwner, XID xID, int x, int y, int w, int h, tstring szText, CXFont * pFont = nullptr);
	~CXTrackBar();

	virtual bool Create();
	virtual void OnNotify(UINT code, LPARAM lParam);
	virtual void OnHScroll(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnCtlColor(WPARAM wParam, LPARAM lParam);
};

