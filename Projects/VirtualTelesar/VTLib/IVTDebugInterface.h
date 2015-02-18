

/********************************************************************
	created:	2013/03/04
	created:	4:3:2013   13:45
	filename: 	D:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\IVTDebugInterface.h
	file path:	D:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	IVTDebugInterface
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _IVTDebugInterface_
#define _IVTDebugInterface_



namespace mray
{
namespace VT
{

	class IVTDebugObject
	{
	public:
		virtual core::string GetName()=0;
		virtual core::string GetDebugString()=0;
	};
class IVTDebugInterface
{
protected:
	typedef std::list<IVTDebugObject*> ObjectList;
	ObjectList m_objects;
public:
	IVTDebugInterface(){}
	virtual~IVTDebugInterface(){}

	virtual void ClearDebugObjects(){m_objects.clear();}
	virtual void AddDebugObject(IVTDebugObject* obj){m_objects.push_back(obj);}
	virtual void RemoveDebugObject(IVTDebugObject* obj)
	{
		m_objects.remove(obj);
	}
};


}
}

#endif
