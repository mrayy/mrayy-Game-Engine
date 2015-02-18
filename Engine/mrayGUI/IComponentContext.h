


/********************************************************************
	created:	2011/11/27
	created:	27:11:2011   18:07
	filename: 	d:\Development\mrayEngine\Engine\mrayGUI\IComponentContext.h
	file path:	d:\Development\mrayEngine\Engine\mrayGUI
	file base:	IComponentContext
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IComponentContext__
#define __IComponentContext__


namespace mray
{
namespace GUI
{

class IComponentContext
{
private:
protected:
	IGUIElement* m_owner;
	uint m_id;
public:
	IComponentContext(IGUIElement* o):m_owner(o)
	{}
	virtual~IComponentContext()
	{}

	void SetID(uint id){m_id=id;}
	uint GetID(){return m_id;}
	IGUIElement* GetOwner(){return m_owner;}
};

}
}

#endif
