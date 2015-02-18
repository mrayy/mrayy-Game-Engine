

/********************************************************************
	created:	2013/05/07
	created:	7:5:2013   20:59
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\IGUITreeBox.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine
	file base:	IGUITreeBox
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __IGUITreeBox__
#define __IGUITreeBox__



#include "IGUIElement.h"
#include "GUITreeNode.h"

namespace mray
{
namespace GUI
{

class MRAY_DLL IGUITreeBox:public IGUIElement
{
private:
protected:
	
public:

	ObjectEvent OnExpanded; //args=TreeNode
	ObjectEvent OnCollapsed;//args=TreeNode

public:
	static const GUID ElementType;

	IGUITreeBox(IGUIManager*mngr);
	virtual~IGUITreeBox();

	virtual GUITreeNode* AddItem(const core::string& text)=0;
	virtual void RemoveItem(GUITreeNode* node)=0;
	virtual void ClearItems()=0;

	virtual void ExpandAll()=0;
	virtual void CollapseAll()=0;

	virtual GUITreeNodeList& GetNodes()=0;

};


}
}



#endif