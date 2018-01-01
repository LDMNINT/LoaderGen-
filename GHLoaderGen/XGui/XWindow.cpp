#include "XWindow.h"

CXWindow::CXWindow(HINSTANCE hInstance)
{
	Init();
	this->hInstance = hInstance;
}

CXWindow::CXWindow(HINSTANCE hInstance, WNDPROC WndProc)
{
	Init();
	this->hInstance = hInstance;
	this->WndProc = WndProc;
}

CXWindow::CXWindow(HINSTANCE hInstance, WNDPROC WndProc, RECT rcRect)
{
	Init();
	this->hInstance = hInstance;
	this->WndProc = WndProc;
	this->rcRect = rcRect;
}

CXWindow::CXWindow(HINSTANCE hInstance, HWND hOwner, XID xID)
{
	Init();
	this->hInstance = hInstance;
	this->hOwner = hOwner;
	this->xID = xID;
}

CXWindow::CXWindow(HINSTANCE hInstance, HWND hOwner, XID xID, WNDPROC WndProc)
{
	Init();
	this->hInstance = hInstance;
	this->hOwner = hOwner;
	this->xID = xID;
	this->WndProc = WndProc;
}


CXWindow::~CXWindow()
{
	DestroyWindow(hWnd);
}

void CXWindow::Init()
{
	hOwner = NULL;
	hWnd = NULL;
	WndProc = NULL;
	xID = 0;
	hBgr = NULL;
	clBgr = 0;
	rcRect = { 0 };
	uWidth = 0;
	uHeight = 0;
	dwExStyle = NULL;
	dwStyle = NULL;
	szClass = _T("");
	szText = _T("");
}

WNDCLASSEX CXWindow::CreateBasicClass(tstring &szClassName)
{
	WNDCLASSEX wcx;
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WndProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = hBgr;
	wcx.lpszMenuName = szMenu.c_str();
	wcx.lpszClassName = szClassName.c_str();
	hIcon = wcx.hIcon = (hIcon) ? hIcon : LoadIcon(NULL,IDI_APPLICATION);
	hIconSm = wcx.hIconSm = (hIconSm) ? hIconSm : LoadIcon(NULL, IDI_APPLICATION);
	return wcx;
}

bool CXWindow::CreateXWindow()
{
	hWnd = CreateWindowEx(
		dwExStyle,
		szClass.c_str(),
		szText.c_str(),
		dwStyle, 
		wpPos.x,
		wpPos.y,
		uWidth,
		uHeight,
		hOwner,
		(HMENU)xID,
		hInstance,
		NULL);

	if (!hWnd)
		return false;
	UpdateWindow(hWnd);
	ShowWindow(hWnd, SW_SHOW);
	GetRect();
	return true;
}

CXWindow * CXWindow::GetOwnerWindow()
{
	return pOwnerWindow;
}

HWND CXWindow::GetOwner()
{
	return hOwner;
}

HINSTANCE CXWindow::GetInstance()
{
	return hInstance;
}

HWND CXWindow::GetHandle()
{
	return hWnd;
}

WNDPROC CXWindow::GetWndProc()
{
	return WndProc;
}

XID CXWindow::GetID()
{
	return xID;
}

COLORREF CXWindow::GetBackgroundColor()
{
	return clBgr;
}

RECT CXWindow::GetRect()
{
	GetPos();
	rcRect = { wpPos.x, wpPos.y, wpPos.x + wpPos.cx, wpPos.y + wpPos.cy };
	/*GetClientRect(hWnd, &rcRect);*/
	return rcRect;
}

RECT CXWindow::GetAdjRect()
{
	GetPos();
	//rcRect = { wpPos.x, wpPos.y, (uint)wpPos.x + uWidth, (uint)wpPos.y + uHeight };
	return rcRect;
}

UINT CXWindow::GetWidth()
{
	GetRect();
	return rcRect.right - rcRect.left;
}

UINT CXWindow::GetHeight()
{
	GetRect();
	return rcRect.bottom - rcRect.top;
}

HICON CXWindow::GetIcon()
{
	return hIcon;
}

HICON CXWindow::GetIconSm()
{
	return hIconSm;
}

WINDOWPOS CXWindow::GetPos()
{
	//GetWindowRect(hWnd, &rcRect);
	//wpPos.x = rcRect.left;
	//wpPos.y = rcRect.top;
	//wpPos.cx = rcRect.right - rcRect.left;
	//wpPos.cy = rcRect.bottom - rcRect.top;
	return wpPos;
}

DWORD CXWindow::GetStyle()
{
	return dwStyle;
}

RECT & CXWindow::GetWndRect()
{
	GetWindowRect(hWnd, &rcWindow);
	return rcWindow;
}

HANDLE CXWindow::GetBackground()
{
	return hBgr;
}

tstring CXWindow::GetText()
{
	return this->szText;
}

void CXWindow::SetOwnerWindow(CXWindow * pWindow)
{
	this->pOwnerWindow = pWindow;
}

void CXWindow::SetBgColor(COLORREF clColor)
{
	this->clBgr = clColor;
	if (hBgr)
		DeleteObject(hBgr);

	hBgr = CreateSolidBrush(clColor);
	SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG)hBgr);
	InvalidateRect(hWnd, NULL, TRUE);
}

void CXWindow::SetStyle(DWORD dwStyle)
{
	this->dwStyle = dwStyle;
	SetWindowLongPtr(hWnd, GWL_STYLE, dwStyle);
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
}

void CXWindow::SetClass(tstring szClass)
{
	this->szClass = szClass;
}

void CXWindow::SetPos(WINDOWPOS wpPos)
{
	this->wpPos = wpPos;
}

void CXWindow::SetRect(RECT rcRect)
{
	this->rcRect = rcRect;
	this->uWidth = rcRect.right - rcRect.left;
	this->uHeight = rcRect.bottom - rcRect.top;
	WINDOWPOS pos;
	pos.x = rcRect.left;
	pos.y = rcRect.top;
	pos.cx = uWidth;
	pos.cy = uHeight;
	SetPos(pos);
}

void CXWindow::SetWidth(UINT uWidth)
{
	this->uWidth = uWidth;
}

void CXWindow::SetHeight(UINT uHeight)
{
	this->uHeight = uHeight;
}

void CXWindow::SetBrush(HBRUSH hBrush)
{
	this->hBgr = hBrush;
}

void CXWindow::SetWndProc(WNDPROC WndProc)
{
	SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG)WndProc);
}

void CXWindow::Clear()
{//paint background color to window
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	FillRect(hdc, &rcRect, hBgr);
	EndPaint(hWnd, &ps);
}

void CXWindow::SetText(tstring szText)
{
	//Clear();
	this->szText = szText;
	SendMessage(this->hWnd, WM_CLEAR, 0, 0);
	SendMessage(this->hWnd, WM_SETTEXT, 0, (LPARAM)szText.c_str());
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
}

void CXWindow::SetPos(int x, int y)
{
	WINDOWPOS pos{ 0 };
	pos.x = x;
	pos.y = y;
	this->wpPos = pos;
	SetWindowPos(hWnd, NULL, pos.x, pos.y, 0, 0, SWP_NOSIZE);
}

