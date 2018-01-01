#pragma once
#include "stdafx.h"
class ExtractIcons
{
public:
    ExtractIcons(void);
    ~ExtractIcons(void);

    // http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winui/winui/windowsuserinterface/resources/icons/iconreference/iconfunctions/extracticonex.asp
    //iconIndex
    //[in] Specifies the zero-based index of the first icon to extract. 
    //      For example, if this value is zero, the function extracts the first icon in the specified file. 
    //
    //If this value is –1 and phicon is NULL, 
    //      the function returns the total number of icons in the specified file. 
    //
    // If the file is an executable file or DLL, 
    // the return value is the number of RT_GROUP_ICON resources. 
    // If the file is an .ico file, the return value is 1. 
    //
    //If this value is a negative number and either phiconLarge or phiconSmall is not NULL, 
    //the function begins by extracting the icon whose 
    //resource identifier is equal to the absolute value of nIconIndex. 
    //For example, use -3 to extract the icon whose resource identifier is 3. 
    static UINT Get( LPCTSTR pFileName, int iconIndex, 
                     int cxIcon, int cyIcon, 
                     HICON* phicon, UINT* piconid, 
                     UINT maxIcons, UINT flags /* LR_COLOR ( for full color )*/ );
private:
    // http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winui/winui/windowsuserinterface/resources/introductiontoresources/resourcereference/resourcestructures/newheader.asp
    struct NEWHEADER
    {
        WORD    Reserved;
        WORD    ResType;
        WORD    ResCount;
    };

    enum
    {
        COM_FILE = 1,
        BAT_FILE,
        CMD_FILE,
        PIF_FILE,
        LNK_FILE,
        ICO_FILE,
        EXE_FILE,
    };

    static UINT _ExtractIcons( LPCTSTR pFileName, int iconIndex, 
                               int cxIcon, int cyIcon, 
                               HICON* phicon, UINT* piconid, 
                               UINT maxIcons, UINT flags );

    static UINT _ExtractFromExe( HANDLE hFile, int iconIndex, 
                                 int cxIconSize, int cyIconSize, 
                                 HICON *phicon, UINT *piconid, 
                                 UINT maxIcons, UINT flags );
    static UINT _ExtractFromBMP( LPCTSTR pFileName, int iconIndex, 
                                 int cxIcon, int cyIcon, 
                                 HICON* phicon, UINT flags );
    static UINT _ExtractFromICO( LPCTSTR pFileName, int iconIndex, 
                                 int cxIcon, int cyIcon, 
                                 HICON* phicon, UINT flags );

    static void* _FindResource( IMAGE_DOS_HEADER* pDosHeader, void* prt, int resIndex, int resType, DWORD* pcbSize );
    static int _FindResourceCount( void* prt, int resType );
    static IMAGE_RESOURCE_DIRECTORY_ENTRY* _FindResourceBase( void* prt, int resType, int* pCount );
    static void* _GetResourceTable( IMAGE_DOS_HEADER* pDosHeader );
    static void* _RelativeVirtualAddresstoPtr( IMAGE_DOS_HEADER* pDosHeader, DWORD rva );
    static int _HasExtension( LPCTSTR pPath );
};
