#pragma once
#include "XControl.h"

#define WS_XCOMBOBOX			CBS_SIMPLE | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE
#define WS_XCOMBOBOXDROP		CBS_DROPDOWN| CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE
#define WS_XCOMBOBOXDROPLIST	CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE

class CXComboBox : public CXControl
{
public:
	CXComboBox(CXWindow * pOwner, XID xID, int x, int y, int w, int h, tstring szText, CXFont * pFont = nullptr);
	~CXComboBox();
	virtual bool Create();
	virtual void AddString(tstring szText);
	virtual int GetSelectedIndex();
	virtual int OnCommand(WPARAM wParam, LPARAM lParam);
	void SetSelectionChangeCB(std::function<void(uintptr_t)> fnSelectionChangeCallback, uintptr_t pArg);

private:
	std::function<void(uintptr_t)> fnSelectionChangeCallback;
	uintptr_t pSelectionChangeArg;
};

