

#include "GUIFileBox.h"
#include "GUIManager.h"
#include "IFileSystem.h"
#include "IOSystem.h"
#include "IDirOS.h"
#include "MouseEvent.h"
#include "GUIEvent.h"
#include "KeyboardEvent.h"
#include "EventMemoryManager.h"

namespace mray{
namespace GUI{

	
class GUIFileBox::CDrvListEvent:public IEventReciver{
	GUIFileBox*fileBox;
public:
	CDrvListEvent(GUIFileBox*fbox){
		fileBox=fbox;
	}
	virtual void onEvent(const EventPtr& e){
		if(e->getType()!=ET_GuiEvent)
			return;
		GCPtr<GUIEvent> event=e;
		if(event->event==GET_SELECT_ITEM){
			core::UTFString text;
			 text=(mchar)fileBox->drvLstIndex[fileBox->drvList->getSelectedItem()];
			 text+=mT(":\\");
			fileBox->fileOS->changeDir(text.c_str());
			fileBox->pathEdt->Caption=(text.c_str());
			fileBox->refreshDirList();
		}
	}
};
class GUIFileBox::CDirsListEvent:public IEventReciver{
	GUIFileBox*fileBox;
public:
	CDirsListEvent(GUIFileBox*fbox){
		fileBox=fbox;
	}
	virtual void onEvent(const EventPtr& e){
		if(e->getType()!=ET_GuiEvent)
			return;
		GCPtr<GUIEvent> event=e;
		if(event->event==GET_Mouse_DOWN_AGAIN){
			GUIList::listItem* item= fileBox->dirsList->getItem(fileBox->dirsList->getSelectedItem());
			if(!item)return;
			//str[str.length()-1]=0;
			 
			 core::UTFString t=fileBox->fileOS->getFullFileName(fileBox->fileOS->getFileIndex(item->item));
			//strCopy(txt,t.c_str());
			fileBox->pathEdt->Caption=t;
			item=fileBox->dirsList->getItem(fileBox->dirsList->getSelectedItem());
			if(item)
				fileBox->fileOS->changeDir(item->item);
			fileBox->refreshDirList();
		}
	}
};
class GUIFileBox::COkBtnEvent:public IEventReciver{
	GUIFileBox*fileBox;
public:
	COkBtnEvent(GUIFileBox*fbox){
		fileBox=fbox;
	}
	virtual void onEvent(const EventPtr& e){
		if(e->getType()!=ET_GuiEvent)
			return;
		GCPtr<GUIEvent> event=e;
		if(event->event==GET_Mouse_UP){
			if(fileBox->state==EB_OPEN)
			{
				if(fileBox->pathEdt->Caption()==mT(""))return;
				if(!gFileSystem.fileExist(fileBox->pathEdt->Caption()))
				{
					fileBox->statusTxt->Caption=mT("Invaild file name!");
					return;
				}
			}
			(*fileBox->outString)=fileBox->pathEdt->Caption();
			fileBox->closeState=EBC_OK;
			fileBox->isOpen=0;
			GCPtr<GUIEvent> event1=EventMemoryManager::getInstance().createEvent(ET_GuiEvent);
			event1->Caller=fileBox;
			event1->event=GET_DIALOG_CLOSE;
		
			fileBox->getManager()->onEvent(event1);
			if(fileBox->getParent())
				fileBox->getParent()->setActiveElem(0);
		}
	}
};
class GUIFileBox::CCancelBtnEvent:public IEventReciver{
	GUIFileBox*fileBox;
public:
	CCancelBtnEvent(GUIFileBox*fbox){
		fileBox=fbox;
	}
	virtual void onEvent(const EventPtr& e){
		if(e->getType()!=ET_GuiEvent)
			return;
		GCPtr<GUIEvent> event=e;
		if(event->event==GET_Mouse_UP){
			(*fileBox->outString)="";
			fileBox->closeState=EBC_CANCEL;
			fileBox->isOpen=0;
			GCPtr<GUIEvent> event1=EventMemoryManager::getInstance().createEvent(ET_GuiEvent);
			event1->Caller=fileBox;
			event1->event=GET_DIALOG_CLOSE;
			fileBox->getManager()->onEvent(event1);
			if(fileBox->getParent())
				fileBox->getParent()->setActiveElem(0);
		}
	}
};

class GUIFileBox::CFilesListEvent:public IEventReciver{
	GUIFileBox*fileBox;
public:
	CFilesListEvent(GUIFileBox*fbox){
		fileBox=fbox;
	}
	virtual void onEvent(const EventPtr& e){
		switch(e->getType()){
		case ET_GuiEvent:
		{

			GCPtr<GUIEvent> event=e;
			switch(event->event){
			case GET_SELECT_ITEM:{
				GUIList::listItem* item= fileBox->filesList->getItem(fileBox->filesList->getSelectedItem());
				if(!item)break;
				fileBox->pathEdt->Caption=fileBox->fileOS->getFullFileName(fileBox->fileOS->getFileIndex(item->item));
			}break;
			case GET_Mouse_DOWN_AGAIN:{
				GCPtr<GUIEvent> e1=EventMemoryManager::getInstance().createEvent(ET_GuiEvent);
				e1->Caller=event->Caller;
				e1->event=GET_Mouse_UP;
				fileBox->okBtnEvent->onEvent(e1);
			}break;
			case GET_Mouse_MOVE:{
			}break;
			}
			
		}break;
		case ET_Mouse:
		{
			GCPtr<MouseEvent> event=e;
			int p=fileBox->filesList->getItemFromPos(event->x,event->y);
			if(p!=-1)
			{
				GUIList::listItem* item= fileBox->filesList->getItem(p);
				if(!item)break;
				int i=fileBox->fileOS->getFileIndex(item->item);
				float sz=(float)fileBox->fileOS->getFileSize(i);
				core::UTFString str=mT("Bytes");
				if(sz>1024){
					sz/=1024.0f;
					str=mT("KBytes");
				}
				if(sz>1024){
					sz/=1024.0f;
					str=mT("MBytes");
				}
				core::UTFString str2=mT("fileName:");
				str2+=fileBox->filesList->getItem(p)->item;
				str2+=mT(" ,size:");
				str2+=sz;
				str2+=str.c_str();
				fileBox->filesList->HintPos=math::vector2d(event->x,event->y);
				fileBox->filesList->HintStr=str2;
				fileBox->filesList->showHint(1);
			}else{
				//	filesList->showHint(0);
			}
		}break;
		}
	}
};

class GUIFileBox::CFiltersListEvent:public IEventReciver{
	GUIFileBox*fileBox;
public:
	CFiltersListEvent(GUIFileBox*fbox){
		fileBox=fbox;
	}
	virtual void onEvent(const EventPtr& e){
		if(e->getType()!=ET_GuiEvent)return;
		GCPtr<GUIEvent> event=e;
		if(event->event==GET_SELECT_ITEM){
			if(fileBox->filterList->getSelectedItem()==-1)return;
			fileBox->currFilter=fileBox->filtersArr[fileBox->filterList->getSelectedItem()];
			fileBox->refreshFileList();
		}
	}
};


void GUIFileBox::clearFilters(){
	filterList->clear();
	filtersInfoArr.clear();
	filtersArr.clear();
}



GUIFileBox::GUIFileBox(GUIManager* mngr,math::vector2d pos,GUIElement*parent):
GUIWindow(mngr,mT(""),pos,math::vector2d(380,380),0,parent)
{
#ifdef ___DEBUG___
	setDebugName("GUIFileBox");
#endif

	sParent=parent;
	SetManager(mngr);
	initializeDialog();
}
GUIFileBox::~GUIFileBox()
{


	filtersInfoArr.clear();
	filtersArr.clear();
	drvLstIndex.clear();
	drvIndex.clear();


	dirsList=0;
	filesList=0;
	filterList=0;
	drvList=0;
	pathEdt=0;
	dirsTxt=0;
	filesTxt=0;
	drvTxt=0;
	pathTxt=0;
	statusTxt=0;
	filterTxt=0;
	okBtn=0;
	cancelBtn=0;
	outString=0;
	currFilter=0;
}


void GUIFileBox::initializeDialog()
{
	if(!guiMngr)return;

	fileOS=OS::IOSystem::getInstance().createDirSystem();

	dirsList=0;
	filesList=0;
	filterList=0;
	drvList=0;
	pathEdt=0;
	dirsTxt=0;
	filesTxt=0;
	drvTxt=0;
	pathTxt=0;
	statusTxt=0;
	filterTxt=0;
	okBtn=0;
	cancelBtn=0;
	outString=0;
	currFilter=0;

	sParent=0;


	closeState=EBC_CANCEL;
	state=EB_OPEN;
	isOpen=0;


	drvListEvent=new CDrvListEvent(this);
	dirsListEevnt=new CDirsListEvent(this);
	filesListEvent=new CFilesListEvent(this);
	filterListEvent=new CFiltersListEvent(this);
	okBtnEvent=new COkBtnEvent(this);
	cancelBtnEvent=new CCancelBtnEvent(this);

	//Func=GUIFileBox::drvListFun;
	drvList		=guiMngr->addComboList(mT(""),math::vector2d(20,40),math::vector2d(140,20),drvListEvent,this,240,0);
//	drvList->loadTextures();
	dirsList	=guiMngr->addList(math::vector2d(20,80),math::vector2d(140,200),dirsListEevnt,this,240,0);
	filesList	=guiMngr->addList(math::vector2d(180,40),math::vector2d(180,240),filesListEvent,this,240,0);

	drvTxt=guiMngr->addStaticText(mT("Drive:"),math::vector2d(20,20),math::vector2d(100,20),NULL,this,240,0);
	drvTxt->Background=(0);
	dirsTxt		=guiMngr->addStaticText(mT("Directoris:"),math::vector2d(20,60),math::vector2d(100,20),NULL,this,240,0);
	dirsTxt->Background=(0);
	filesTxt	=guiMngr->addStaticText(mT("Files:"),math::vector2d(180,20),math::vector2d(100,20),NULL,this,240,0);
	filesTxt->Background=(0);
	pathTxt		=guiMngr->addStaticText(mT("Path:"),math::vector2d(20,300),math::vector2d(40,20),NULL,this,240,0);
	pathTxt->Background=(0);

	pathEdt		=guiMngr->addEditBox(mT(""),math::vector2d(60,300),math::vector2d(300,20),NULL,	this,240,0);

	okBtn		=guiMngr->addButton(mT("OK"),math::vector2d(280,330),math::vector2d(80,20),okBtnEvent,this,240,0);
	cancelBtn	=guiMngr->addButton(mT("Cancel"),math::vector2d(280,355),math::vector2d(80,20),cancelBtnEvent,this,240,0);

	filterTxt	=guiMngr->addStaticText(mT("types:"),math::vector2d(20,330),math::vector2d(60,20),NULL,this,240,0);
	filterTxt->Background=(0);

	filterList		=guiMngr->addComboList(mT(""),math::vector2d(85,330),math::vector2d(190,20),filterListEvent,this,240,0);
//	filterList->loadTextures();
	filterList->setMaxShown(5);

	statusTxt=guiMngr->addStaticText(mT(""),math::vector2d(20,355),math::vector2d(255,20),0,this);
	if(statusTxt->getFontAttributes())
		statusTxt->getFontAttributes()->fontColor=video::SColor(255,0,0,255);

	filterList->addItem(mT("All Files( *.* )"));
	filterList->setSelectedItem(0);
	filtersInfoArr.push_back(mT("All Files( *.* )"));
	filtersArr.push_back(mT("."));
	currFilter=".";

	drvIndex.resize(26);
	drvLstIndex.resize(26);
	
	std::vector<char> drvLetters;
	gFileSystem.getAllDrivesLetters(drvLetters);
	core::UTFString text;
	for(int i=0;i<drvLetters.size();++i){
		drvLstIndex[i]=drvLetters[i];
		drvIndex[(int)drvLetters[i]-'A']=i;
		
		text=gFileSystem.getDriveVolume(drvLetters[i]);
		text+=mT("(");
		text+=(mchar)drvLetters[i];
		text+=mT(":)");
		drvList->addItem(text.c_str());
	}


	dirsList->getFontAttributes()->fontSize=12;
	dirsList->getFontAttributes()->hasShadow=false;
	dirsList->getFontAttributes()->fontColor.Set(0,0,0,255);

	filesList->getFontAttributes()->fontSize=12;
	filesList->getFontAttributes()->hasShadow=false;
	filesList->getFontAttributes()->fontColor.Set(0,0,0,255);

	drvList->getFontAttributes()->fontSize=12;
	drvList->getFontAttributes()->hasShadow=false;
	drvList->getFontAttributes()->fontColor.Set(0,0,0,255);

	filterList->getFontAttributes()->fontSize=12;
	filterList->getFontAttributes()->hasShadow=false;
	filterList->getFontAttributes()->fontColor.Set(0,0,0,255);

	pathEdt->getFontAttributes()->fontSize=12;
	pathEdt->getFontAttributes()->hasShadow=false;
	pathEdt->getFontAttributes()->fontColor.Set(0,0,0,255);



	okBtn->getFontAttributes()->fontSize=12;
	cancelBtn->getFontAttributes()->fontSize=12;
	filesTxt->getFontAttributes()->fontSize=12;
	drvTxt->getFontAttributes()->fontSize=12;
	pathTxt->getFontAttributes()->fontSize=12;
}

void GUIFileBox::openDialog(EBOX_STATE s,core::UTFString*pathOut)
{
	if(!Visible){
		isOpen=false;
	}
	if(isOpen)return;
	if(!pathOut)
		return;
	isOpen=1;
	if(s==EB_OPEN){
		setCaption(mT("Open File"));
	}
	else setCaption(mT("Save File"));

	outString=pathOut;

	state=s;

	refreshDirList();
	setVisible(1);

	int indx=fileOS->getCurrDriveLetter()-'A';
	indx=drvIndex[indx];
	drvList->setSelectedItem(indx);

	if(parent){
		parent->bringToFront(this);
		parent->setActiveElem(this);
	}else{
		getManager()->bringToFront(this);
		getManager()->setActiveElement(this);
	}



}


void GUIFileBox::draw(float dt)
{
	if(!isOpen){
		setVisible(0);
		return;
	}
	/*video::TextureBank*bank=guiMngr->getSkin()->getTextureBank();
	if(filesList->getTextureBank().pointer()!=bank)
		filesList->setTextureBank(bank);
	if(dirsList->getTextureBank().pointer()!=bank)
		dirsList->setTextureBank(bank);
	*/
	GUIWindow::draw(dt);
}


void GUIFileBox::refreshDirList(){
	core::UTFString fName;
	dirsList->clear();
	for(int i=0;i<fileOS->getFilesCount();++i){
		if(fileOS->isFileDirectory(i)){
			fName=fileOS->getShortFileName(i);
			if(compareStr(fileOS->getShortFileName(i).c_str(),mT("."))!=0 && 
			compareStr(fileOS->getShortFileName(i).c_str(),mT(".."))!=0)
				fName+=core::UTFString(mT("\\"));
			dirsList->addItem(fName.c_str(),video::ImageSetTexture(m_imageSetName,"Folder"));
		}
	}
	dirsList->setSelectedItem(-1);
	dirsList->sortItems();
	refreshFileList();
}

void GUIFileBox::refreshFileList(){
	filesList->clear();

	int p=currFilter.findlast('.');
	if(p!=-1)
		currFilter.erase(0,p);
	currFilter.make_lower();
	for(int i=0;i<fileOS->getFilesCount();++i){
		if(!fileOS->isFileDirectory(i) ){
			if(currFilter!=mT(".")){
				core::UTFString str=fileOS->getShortFileName(i);
				str.make_lower();
				if(str.find(currFilter.c_str())!=-1)
					filesList->addItem(fileOS->getShortFileName(i),video::ImageSetTexture(m_imageSetName,"File"));
			}
			else
				filesList->addItem(fileOS->getShortFileName(i),video::ImageSetTexture(m_imageSetName,"File"));
		}
	}
	filesList->setSelectedItem(-1);
	filesList->sortItems();
}

void GUIFileBox::onEvent(const EventPtr& e)
{
	if(!isOpen)return;
	GUIWindow::onEvent(e);
	switch(e->getType())
	{
	case ET_Keyboard:
		{
			GCPtr<KeyboardEvent> event=e;
		if(event->key==KEY_ESCAPE &&
			event->press==1){
			(*outString)="";
			closeState=EBC_CANCEL;
			isOpen=0;
			GCPtr<GUIEvent> event1=EventMemoryManager::getInstance().createEvent(ET_GuiEvent);
			event1->Caller=this;
			event1->event=GET_DIALOG_CLOSE;
			if(sParent)
				sParent->setActiveElem(0);
			cancelBtn->getManager()->onEvent(event1);
		}
		}
		break;
	}
}

void GUIFileBox::addFilter(const  core::UTFString&filter,const  core::UTFString&info){
	core::UTFString str(info);
	str+=mT("( ");
	str+=filter;
	str+=mT(" )");
	filterList->addItem(str);
	filtersInfoArr.push_back(info);
	filtersArr.push_back(filter);
	if(filtersArr.size()==1){
		filterList->setSelectedItem(0);
		currFilter=filter;
		refreshFileList();
	}
}

std::vector<core::UTFString>* GUIFileBox::getFilters(){
	return &filtersArr;
}



}
}

