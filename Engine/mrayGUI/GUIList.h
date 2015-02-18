

#ifndef ___GUILIST___
#define ___GUILIST___

#include "GUIElement.h"
#include "SColor.h"
#include "IFont.h"
#include "mArray.h"
#include "mString.h"
#include "ImageSetTexture.h"


namespace mray{
namespace GUI{

class MRAY_DLL GUIList:public GUIElement
{
public:
	struct listItem
	{
		core::UTFString item;
		video::ImageSetTexture icon;
		void* userData;
		listItem():item(mT("")),userData(0)
		{}
		listItem(const  core::UTFString&str,const video::ImageSetTexture& iconID,void* ud):item(str),icon(iconID),userData(ud)
		{}
	};
protected:

	GCPtr<video::TextureUnit> m_texture;

	core::array<listItem> itemList;
	//GCPtr<video::TextureBank>bank;
	int selectedItem;
	bool drawBackgrnd;
	int startItem;
	FontAttributes m_FontAttributs;
public:
	DECLARE_RTTI;

	GUIList();
	GUIList(GUIManager* manager,const core::UTFString& _text,const math::Point2df& pos,const math::Point2df& size,IEventReciver* event=0,GUIElement* parent=0,const video::SColor& color=240,int id=0);
	virtual~GUIList();
	virtual void draw(float dt);
	virtual void onEvent(const EventPtr&  event);

	int getItemsCount(){return itemList.size();}

	listItem* getItem(int index);
	void* getUserData(int index);
	int getItemIndex(const core::UTFString& item);

	int addItem(const core::UTFString& item,const video::ImageSetTexture& icon,void*userData=0);
	void setItem(int id,const core::UTFString& item,const video::ImageSetTexture& icon,void*userData=0);
	void deleteItem(int index);
	void deleteItem(const  core::UTFString& item);
	void sortItems();
	void setBKVisible(bool visible){drawBackgrnd=visible;}
/*
	void setTextureBank(GCPtr<video::TextureBank>bank);
	GCPtr<video::TextureBank>getTextureBank();
	void setIconTexture(video::ITexturePtr  tex,int index);*/

	void clear();

	const core::array<listItem>& getItems(){return itemList;}

	int getSelectedItem(){return selectedItem;}
	void setSelectedItem(int i){
		if(i>=-1 && i<itemList.size())
			selectedItem=i;
		else
			selectedItem=-1;
	}

	int getItemFromPos(float x,float y);
	
	virtual GCPtr<GUIElement> getDuplicate();
	virtual FontAttributes* getFontAttributes();
};


}
}

#endif









