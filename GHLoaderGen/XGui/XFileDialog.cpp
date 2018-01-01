#include "XFileDialog.h"

CXFileDialog::CXFileDialog(CXWindow * pOwner, EXFILEDIALOGMODE dialogMode, CXFileDialogFilterEntry * pFilter)
{
	this->pOwner = pOwner;
	this->fdMode = dialogMode;
	this->szFilter.resize(pFilter->GetData().size());
	this->szExt = pFilter->GetExt();
	memcpy(&this->szFilter[0], &szFilter[0], szFilter.size());
	szFileName.resize(MAX_PATH);
	ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	//ofn.lpstrFilter = /*static_cast<TCHAR*>(&this->szFilter[0]);*/ _T("All Files (*.*)\0*.*\0");
	ofn.lpstrFilter = (LPCTSTR)pFilter->GetData().data();
	ofn.lpstrFile = &szFileName[0];
	ofn.nMaxFile = MAX_PATH;	
	ofn.Flags = (fdMode) ? XFDSAVEFLAG : XFDOPENFLAG;
	ofn.lpstrDefExt = pFilter->GetExt().substr(1, pFilter->GetExt().length() - 1).c_str();
	ofn.hwndOwner = pOwner->GetHandle();//pOwner->GetHandle();
	
}

CXFileDialog::~CXFileDialog()
{
	szFileName.clear();
}

bool CXFileDialog::Show(tstring & szPath)
{
	switch (fdMode)
	{
		case SAVE:
			if (!GetSaveFileName(&ofn))
				return false;			
			break;
		case OPEN:
			if (!GetOpenFileName(&ofn))
				return false;
			break;
	}
	szPath = ofn.lpstrFile;
	/*if(fdMode == SAVE)
		szPath.append((TCHAR*)&szExt[1]);*/
	
	return true;
}

tstring CXFileDialog::GetFileName()
{
	return szFileName;
}

CXFileDialogFilterEntry::CXFileDialogFilterEntry(tstring szDesc, tstring szExt)
{
	AddEntry(szDesc, szExt);
}

std::vector<TCHAR>& CXFileDialogFilterEntry::GetData()
{
	return vData;
}

void CXFileDialogFilterEntry::AddEntry(tstring szDesc, tstring szExt)
{
	this->szDesc = szDesc;
	this->szExt = szExt;
	BuildVector();
}

tstring CXFileDialogFilterEntry::GetDesc()
{
	return szDesc;
}

tstring CXFileDialogFilterEntry::GetExt()
{
	return szExt;
}

void CXFileDialogFilterEntry::BuildVector()
{
	if (!vData.empty())
		vData.resize(vData.size() - 1);

	 //"All Files(*.*)"
	for (auto c : szDesc)
		vData.push_back(c);

	// "All Files(*.*)\0"
	vData.push_back(0);

	// "All Files(*.*)\0*.*"
	for (auto c : szExt)
		vData.push_back(c);

	// "All Files(*.*)\0*.*\0\0"
	for(int x = 0; x < 2; x++)
		vData.push_back(0);
}
