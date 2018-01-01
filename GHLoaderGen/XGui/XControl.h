#pragma once
#include "XWindow.h"
#include "XFont.h"

enum ECXControl
{
	LABEL,
	BUTTON,
	EDIT,
	COMBOBOX,
	LISTBOX,
	GROUPBOX,
	RADIO,
	CHECKBOX,
	TABCONTROL,
	LISTVIEW,
	RICHTEXT,
	TRACKBAR,
	SYSLINK,
	IMAGE
};

class CXControl : public CXWindow
{
public:
	CXControl(CXWindow * pOwner, XID xID, ECXControl ControlType, int x, int y, int w, int h, tstring szText, CXFont * pFont = nullptr);
	CXControl(CXWindow * pOwner, XID xID, ECXControl ControlType, RECT rcRect, tstring szText, CXFont * pFont = nullptr);
	~CXControl();
	virtual bool Create();
	virtual ECXControl GetControlType();
	virtual RECT GetAdjustedRect();
	virtual void SetFont(CXFont * pFont);
	virtual void SetTxtColor(COLORREF clText);
	virtual void SetAction(std::function<void(uintptr_t)> fnAction);
	virtual void SetCommandArgs(uintptr_t pArgs);
	virtual int OnCommand(WPARAM wParam, LPARAM, uintptr_t pArgs);
	virtual int OnCommand(WPARAM wParam, LPARAM);
	virtual void OnNotify(UINT code, LPARAM lParam);
	virtual LRESULT OnDrawItem(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnCtlColor(WPARAM wParam, LPARAM lParam);
	virtual void OnHover(POINT p);
	virtual void OnHScroll(WPARAM wParam, LPARAM lParam);
	virtual void OnPaint(WPARAM wParam, LPARAM lParam);
	virtual int OnKey(WPARAM wParam, LPARAM lParam);

protected:
	ECXControl ControlType;
	CXFont * pFont = nullptr;
	std::function<void(uintptr_t)> fnAction;
	uintptr_t pCommandArgs = 0;
	COLORREF clText = RGB(255,0,0);
};

