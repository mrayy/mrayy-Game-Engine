

#ifndef ___WinErrorDialog___
#define ___WinErrorDialog___

#include "CompileConfig.h"
#include <IErrorDialog.h>
#include <set>
#include <Windows.h>

namespace mray{


class MRAY_Win32_DLL WinErrorDialog:public IErrorDialog
{
	static BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	core::string m_errorMsg;
	
	HINSTANCE m_hInstance;
public:

	WinErrorDialog();
	virtual ~WinErrorDialog();

	bool run(const core::string&msg);
};

}


#endif



