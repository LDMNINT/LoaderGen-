#pragma once
#include "XGui\XControl.h"
class CXIcon :
	public CXControl
{
public:
	CXIcon(CXWindow * pOwner, XID xID, int x, int y, int w, int h, tstring szText, CXFont * pFont = nullptr);
	~CXIcon();

	bool Load(HICON hIcon);
	virtual LRESULT OnDrawItem(WPARAM wParam, LPARAM lParam);

private:
	void DrawIco(HWND hWnd, HDC hdcDest, HICON hIcon, RECT & pRect);

private:
	HICON hIcon;
};

