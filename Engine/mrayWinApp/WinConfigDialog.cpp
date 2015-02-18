#include "stdafx.h"

#include "WinConfigDialog.h"
#include "resource.h"
#include "OptionContainer.h"

namespace mray{



BOOL CALLBACK WinConfigDialog::DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	core::string str;
	HWND dlgItem ;
	WinConfigDialog*dialog=dynamic_cast<WinConfigDialog*>(WinConfigDialog::getInstancePtr());

	OptionContainer*options=dialog->options;
	OptionContainer::OptionsMap& opList=options->GetOptions();
	switch(iMsg){
		case WM_INITDIALOG:
		{
			OptionContainer::OptionsMap::iterator it=opList.begin();
			dlgItem=GetDlgItem(hDlg,IDC_LIST_OPTIONS);
			for(;it!=opList.end();++it){
				str=it->second.name+mT(" = ")+it->second.getValue();
				SendMessage(dlgItem,LB_ADDSTRING,0,(LPARAM)(str.c_str()));
			}

			
            int x, y, screenWidth, screenHeight;
            RECT rcDlg;
            GetWindowRect(hDlg, &rcDlg);
            screenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
            screenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

            x = (screenWidth / 2) - ((rcDlg.right - rcDlg.left) / 2);
            y = (screenHeight / 2) - ((rcDlg.bottom - rcDlg.top) / 2);

            MoveWindow(hDlg, x, y, (rcDlg.right - rcDlg.left),
                (rcDlg.bottom - rcDlg.top), TRUE);

			return true;
		}
		
        case WM_COMMAND:
		{
            switch (LOWORD(wParam))
			{
            case IDC_LIST_OPTIONS:
                if (HIWORD(wParam) == LBN_SELCHANGE)
                {
                    dlgItem = GetDlgItem(hDlg, IDC_LIST_OPTIONS);
                    int sel = SendMessage(dlgItem, LB_GETCURSEL, 0, 0);
                    if (sel != -1)
                    {
                        dlgItem = GetDlgItem(hDlg, IDC_COMBO_Values);
						SendMessage(dlgItem, CB_RESETCONTENT, 0, 0);
						OptionContainer::OptionsMap::iterator itOpt=opList.begin();
						std::advance(itOpt,sel);
						SOptionElement&elem=itOpt->second;
						SOptionElement::ValueSet::iterator it=elem.valueSet.begin();
						int i=0;
						for(;it!=elem.valueSet.end();++it,i++)
                        {
                            SendMessage(dlgItem, CB_ADDSTRING, 0, (LPARAM)it->c_str());
							if(elem.value.equals_ignore_case(*it))
								SendMessage(dlgItem, CB_SETCURSEL, (WPARAM)i, 0);
							
                        }
                        
						if(elem.valueSet.size()==0){
							ShowWindow(dlgItem, SW_HIDE);
							dlgItem = GetDlgItem(hDlg, IDC_STATIC_Options);
							ShowWindow(dlgItem,SW_HIDE);
							dlgItem = GetDlgItem(hDlg, IDC_EDIT_NewValue);
							ShowWindow(dlgItem,SW_SHOW);
							dlgItem = GetDlgItem(hDlg, IDC_STATIC_NewValue);
							ShowWindow(dlgItem,SW_SHOW);
						}
						else {
							ShowWindow(dlgItem, SW_SHOW);
							dlgItem = GetDlgItem(hDlg, IDC_STATIC_Options);
							ShowWindow(dlgItem,SW_SHOW);
							dlgItem = GetDlgItem(hDlg, IDC_EDIT_NewValue);
							ShowWindow(dlgItem,SW_HIDE);
							dlgItem = GetDlgItem(hDlg, IDC_STATIC_NewValue);
							ShowWindow(dlgItem,SW_HIDE);
						}
                    }
                }

            return TRUE;
            case IDC_COMBO_Values:
				
                if (HIWORD(wParam) == LBN_SELCHANGE)
				{
					
                    dlgItem = GetDlgItem(hDlg, IDC_LIST_OPTIONS);
                    int sel = SendMessage(dlgItem, LB_GETCURSEL, 0, 0);
                    int selOp = sel;

                    dlgItem = GetDlgItem(hDlg, IDC_COMBO_Values);
                    sel = SendMessage(dlgItem, CB_GETCURSEL, 0, 0);

                    if (sel != -1)
					{
						OptionContainer::OptionsMap::iterator itOpt=opList.begin();
						std::advance(itOpt,selOp);
						SOptionElement&elem=itOpt->second;
						elem.setValue(sel);


						dlgItem=GetDlgItem(hDlg,IDC_LIST_OPTIONS);
                        SendMessage(dlgItem, LB_RESETCONTENT, 0, 0);
						for(itOpt=opList.begin();itOpt!=opList.end();++itOpt){
							SOptionElement&elem=itOpt->second; 
							str=elem.name+mT(" = ")+elem.value;
							SendMessage(dlgItem,LB_ADDSTRING,0,(LPARAM)(str.c_str()));
						}
                        SendMessage(dlgItem, LB_SETCURSEL, selOp, 0);
                    }
				   return true;
                }break;
				
            case IDC_EDIT_NewValue:
				
                if (HIWORD(wParam) == EN_CHANGE)
				{
					 mchar buffer[200];
					buffer[0]=0;

                    dlgItem = GetDlgItem(hDlg, IDC_LIST_OPTIONS);
                    int sel = SendMessage(dlgItem, LB_GETCURSEL, 0, 0);

					if(sel!=-1){
					//	int sizeSel = SendDlgItemMessage(hDlg, IDC_EDIT_NewValue, LB_GETCURSEL, 0, 0);
						dlgItem=GetDlgItem(hDlg,IDC_EDIT_NewValue);
						int len=SendMessage(dlgItem,EM_LINELENGTH,0,0);
						SendMessage(dlgItem,WM_GETTEXT,200,(LPARAM)buffer);
						buffer[len]=0;

						OptionContainer::OptionsMap::iterator itOpt=opList.begin();
						std::advance(itOpt,sel);
						itOpt->second.setValue(buffer);


						dlgItem=GetDlgItem(hDlg,IDC_LIST_OPTIONS);
						SendMessage(dlgItem, LB_RESETCONTENT, 0, 0);
						itOpt=opList.begin();
						for(;itOpt!=opList.end();++itOpt){
							SOptionElement&elem=itOpt->second; 
							str=elem.name+mT(" = ")+elem.value;
							SendMessage(dlgItem,LB_ADDSTRING,0,(LPARAM)(str.c_str()));
						}
						SendMessage(dlgItem, LB_SETCURSEL, sel, 0);
					}
                    
				   return true;
                }break;
				case IDCANCEL:
				case IDCLOSE:
				{
					EndDialog(hDlg,0);
					return true;
				}
				case IDOK:
				{
					EndDialog(hDlg,1);
					return true;
				}

			}break;
		}break;


	}
	return false;
}

WinConfigDialog::WinConfigDialog(bool internalRes){
	if(internalRes)
	{
#ifndef MRAY_WinApp_DLL
	m_hInstance=GetModuleHandle(0);
#else 
	m_hInstance=GetModuleHandle(mT("mrayWinApp.dll"));
#endif
	}else
	{
		m_hInstance=GetModuleHandle(0);
	}
}
WinConfigDialog::~WinConfigDialog(){
}

bool WinConfigDialog::run(OptionContainer&options){
	this->options=&options;
	int res=DialogBox(m_hInstance,MAKEINTRESOURCE(IDD_DIALOG_CONFIG),0,DlgProc);

	return res>0;
}


}

