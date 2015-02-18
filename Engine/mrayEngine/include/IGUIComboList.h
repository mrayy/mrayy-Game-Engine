

/********************************************************************
	created:	2011/11/22
	created:	22:11:2011   15:28
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IGUIComboList.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IGUIComboList
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IGUIComboList__
#define __IGUIComboList__

#include "IGUIElement.h"
#include "IListItem.h"

namespace mray
{
namespace GUI
{

class MRAY_DLL IGUIComboList:public IGUIElement
{
private:
protected:
public:
	static const GUID ElementType;

	ObjectEvent OnSelectChange;//args=Index
public:

	IGUIComboList(IGUIManager* manager);
	virtual~IGUIComboList();

	virtual int GetItemsCount()const=0;
	virtual IListItem* GetItem(int index)const=0;
	virtual int FindItem(const core::UTFString& txt)const=0;
	virtual int AddItem(IListItem* item)=0;
	virtual int InsertItem(int index,IListItem* item)=0;
	virtual void DeleteItem(int index)=0;
	virtual const ItemList& GetItems()const=0;
	virtual void SortItems()=0;
	virtual void ClearItems()=0;
	virtual void SetShowItemsCount(int c)=0;
	virtual int GetShowItemsCount()=0;

	virtual int GetSelectedItem()=0;
	virtual void SetSelectedItem(int i)=0;

	virtual int GetItemFromPos(const math::vector2d& y)=0;
};

}
}

#endif
