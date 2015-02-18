

#ifndef ___GUIFileBox___
#define ___GUIFileBox___

#include "GUIWindow.h"
#include "GUIList.h"
#include "GUIComboList.h"
#include "GUIEditBox.h"
#include "GUIStaticText.h"
#include "GUIButton.h"
#include "IOSClipboard.h"
#include "IDirOS.h"

namespace mray{
namespace GUI{

enum EBOX_STATE{
	EB_OPEN=0,
	EB_SAVE
};
enum EBOX_CLOSE_STATE{
	EBC_OK,
	EBC_CANCEL
};

class MRAY_DLL GUIFileBox:public GUIWindow
{
	GCPtr<OS::IDirOS>fileOS;
	GCPtr<GUIElement> sParent;

	//components
	GCPtr<GUIList>dirsList, filesList;
	GCPtr<GUIComboList>drvList, filterList;
	GCPtr<GUIEditBox>pathEdt;
	GCPtr<GUIStaticText>dirsTxt, filesTxt, drvTxt, pathTxt,statusTxt,filterTxt;
	GCPtr<GUIButton>okBtn, cancelBtn;
	
	EBOX_STATE state;
	EBOX_CLOSE_STATE closeState;

	std::vector<int> drvIndex;
	std::vector<int> drvLstIndex;
	std::vector<core::UTFString> filtersArr;
	std::vector<core::UTFString> filtersInfoArr;

	core::UTFString currFilter;
	core::UTFString m_imageSetName;

	bool isOpen;

	class CDrvListEvent;
	class CDirsListEvent;
	class CFilesListEvent;
	class CFiltersListEvent;
	class COkBtnEvent;
	class CCancelBtnEvent;

	CDrvListEvent *drvListEvent;
	CDirsListEvent *dirsListEevnt;
	CFilesListEvent *filesListEvent;
	CFiltersListEvent *filterListEvent;
	COkBtnEvent *okBtnEvent;
	CCancelBtnEvent *cancelBtnEvent;

	void refreshDirList();
	void refreshFileList();

	core::UTFString*outString;
	void initializeDialog();
public:
	DECLARE_RTTI;

	GUIFileBox(GUIManager* mngr,math::vector2d pos,GUIElement*parent);
	virtual~GUIFileBox();

	EBOX_CLOSE_STATE getCloseStatus(){return closeState;}

	virtual void draw(float dt);
	virtual void onEvent(const EventPtr& event);

	void openDialog(EBOX_STATE s,core::UTFString*pathOut);
	EBOX_STATE getState(){return state;}
	bool isDialogOpen(){return isOpen;}
	void clearFilters();
	void addFilter(const  core::UTFString&filter,const  core::UTFString&info);
	std::vector<core::UTFString>* getFilters();
};

}
}


#endif






