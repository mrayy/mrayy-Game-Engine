

#ifndef ___WinConfigDialog___
#define ___WinConfigDialog___

#include "CompileConfig.h"
#include <ISingleton.h>
#include <mString.h>
#include <mArray.h>
#include <Windows.h>
#include <IConfigDialog.h>

namespace mray{


class MRAY_WinApp_DLL WinConfigDialog:public IConfigDialog
{
	static BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	OptionContainer*options;
	
	HINSTANCE m_hInstance;
public:

	WinConfigDialog(bool internalRes);
	virtual ~WinConfigDialog();

	bool run(OptionContainer& options);
};

}


#endif

