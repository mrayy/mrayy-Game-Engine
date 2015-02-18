
/********************************************************************
	created:	2013/01/31
	created:	31:1:2013   16:04
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\IMapController.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	IMapController
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __IMapController__
#define __IMapController__




namespace mray
{
namespace TBee
{
	class MapObject;

class IMapController
{
protected:
public:
	IMapController(){}
	virtual~IMapController(){}

	virtual void SetMapObject(MapObject* map)=0;
	virtual MapObject* GetMapObject()=0;

	virtual bool OnEvent(Event* e)=0;

	virtual void Update(float dt)=0;

};

}
}


#endif
