#include "XListView.h"



CXListView::CXListView(CXWindow * pOwner, XID xID, int x, int y, int w, int h, tstring szText, CXFont * pFont)
	: CXControl(pOwner, xID, LISTVIEW, x, y, w, h, szText, pFont)
{
	szClass = WC_LISTVIEW;
	SetStyle(WS_XLISTVIEW);
	hSmall = NULL;
}


CXListView::~CXListView()
{
}

bool CXListView::Create()
{
	bool bRet = CXControl::Create();
	SendMessage(hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	if (szText != TEXT(""))
		InsertColumn(szText);
	return bRet;
}

void CXListView::OnNotify(UINT code, LPARAM lParam)
{
	static tstring szText;
	NMLVDISPINFO* plvdi;
	switch (((LPNMHDR)lParam)->code)
	{
		case LVN_GETDISPINFO:
		{
			plvdi = (NMLVDISPINFO*)lParam;
			CXListViewItem* pSubItem = mListItems[plvdi->item.iItem].GetSubItem(plvdi->item.iSubItem);
			if (pSubItem)
			{
				szText = pSubItem->GetText();
				plvdi->item.pszText = &szText[0];
			}
			break;
		}
		break;
	}
}

bool CXListView::InsertColumn(tstring szText)
{
	return InsertColumn(szText, 100);
}

bool CXListView::InsertColumn(tstring szText, int width)
{
	//LVCOLUMN lvc;
	//lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	//lvc.iSubItem = iCol;
	//lvc.cx = 100;
	//lvc.pszText = &szText[0];
	//lvc.fmt = LVCFMT_LEFT;
	//if(ListView_InsertColumn(hWnd, iCol, &lvc) == -1)
	//	return false;
	//iCol++;
	//return true;
	return InsertColumn(szText, width, LVCFMT_LEFT);
}

bool CXListView::InsertColumn(tstring szText, int width, DWORD lvcfFmt)
{
	LVCOLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.iSubItem = iCol;
	lvc.cx = width;
	lvc.pszText = &szText[0];
	lvc.fmt = lvcfFmt;
	if (ListView_InsertColumn(hWnd, iCol, &lvc) == -1)
		return false;
	iCol++;
	return true;
}

void CXListView::SetColumnWidth(int iColumn, int width)
{
	ListView_SetColumnWidth(hWnd, iColumn, width);
}

bool CXListView::AddItem(int iItem, int iSubItem)
{
	LVITEM lvi;
	lvi.pszText = L"semen";
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvi.state = 0;
	lvi.stateMask = 0;
	lvi.iSubItem = iSubItem;
	lvi.iItem = iItem;
	if (iSubItem == 0)
		lvi.iImage = iItem;

	if (!ListView_InsertItem(hWnd, &lvi))
		return false;
	return true;
}

bool CXListView::AddItem(CXListViewItem & xListViewItem)
{
	if (!ListView_InsertItem(hWnd, &xListViewItem.GetItem()) == -1)
		return false;
	//usually 
	for (auto si : xListViewItem.GetSubItems())
		ListView_SetItemText(hWnd, si->GetItemIndex(), si->GetItem().iSubItem, si->GetItem().pszText);


	mListItems[xListViewItem.GetItemIndex()] = xListViewItem;
	//if (xListViewItem.GetIconHandle())
	//{
		if (!hSmall)
		{
			hSmall = ImageList_Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CXSMICON), ILC_MASK, 1, 1);
			HICON tmp = xListViewItem.GetIconHandle();
			ImageList_AddIcon(hSmall, tmp ? tmp : LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION)));
		}
		else
		{
			HICON tmp = xListViewItem.GetIconHandle();
			ImageList_AddIcon(hSmall, tmp ? tmp : LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION)));
			//ListView_SetImageList(hWnd, hSmall, LVSIL_SMALL);
		}
//	}
	return true;
}

int CXListView::GetSelectedIndex()
{
	return ListView_GetSelectionMark(hWnd);
}

void CXListView::SetIconList()
{
	ListView_SetImageList(hWnd, hSmall, LVSIL_SMALL);
}

void CXListView::Clear()
{
	if (hSmall)
	{
		ImageList_RemoveAll(hSmall);
	}
	int count = ListView_GetItemCount(hWnd);
	for (int i = 0; i < count; i++)
		ListView_DeleteItem(hWnd, i);
}

CXListViewItem::CXListViewItem(int iItem, int iSubItem, tstring szText)
{
	this->iItem = iItem;
	this->iSubItem = iSubItem;
	this->szText = szText;
}

CXListViewItem::CXListViewItem(int iItem, int iSubItem, tstring szText, HICON hIcon)
{
	this->iItem = iItem;
	this->iSubItem = iSubItem;
	this->szText = szText;
	this->hIcon = hIcon;
}

CXListViewItem::~CXListViewItem()
{
	if (this->hIcon)
		DestroyIcon(hIcon);
}

LVITEM CXListViewItem::GetItem()
{
	lvi.pszText = &szText[0];
	//lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvi.mask = LVIF_IMAGE | LVIF_TEXT;
	lvi.state = 0;
	lvi.stateMask = 0;
	lvi.iSubItem = iSubItem;
	lvi.iItem = iItem;
	lvi.iImage = iItem;
	return lvi;
}

std::vector<CXListViewItem*> CXListViewItem::GetSubItems()
{
	return vSubItems;
}

void CXListViewItem::AddSubItem(int iItem, int iSubItem, tstring szText)
{
	vSubItems.push_back(new CXListViewItem(iItem, iSubItem, szText));
}

CXListViewItem * CXListViewItem::GetSubItem(int iSubItem)
{
	if (vSubItems.size() > iSubItem - 1)
		return vSubItems[iSubItem - 1];
	return nullptr;
}

int CXListViewItem::GetItemIndex()
{
	return iItem;
}

void CXListViewItem::SetText(tstring szText)
{
	this->szText = szText;
}

tstring CXListViewItem::GetText()
{
	return szText;
}

HICON CXListViewItem::GetIconHandle()
{
	return hIcon;
}
