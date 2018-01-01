#include "GHLGen.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	GHLGen gen(hInstance);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		if (!gen.IsDlgMessage(&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}